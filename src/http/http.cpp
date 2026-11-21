#include "stdio.h"
#include "iostream"
#include <netinet/in.h>
#include <unistd.h>
#include "regex.h"
#include "../urldecode/urldecode.hpp"
#include "../utils/file.hpp"
#include "fstream"
#include "string.h"
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int init_server(int port) {
    int server_fd;
    struct sockaddr_in server_addr;

    // Create server socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to the port
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if(listen(server_fd, 10) < 0) {
        perror("listening failed!");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void build_http_response(const char* file_name, const char* file_ext, char *response, size_t *response_len) {
    MimeTypeEntry mime_type = get_mime_type(file_ext);
    size_t response_capacity = BUFFER_SIZE * 2;

   char* header = (char*)malloc(BUFFER_SIZE * sizeof(char));
   snprintf(header, BUFFER_SIZE, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "\r\n",
        mime_type.file_mime_type
    );

    // File handling, open file
    std::streampos size = 0;
    std::ifstream file(file_name, std::ios::binary);
    std::string output;

    if(!file.is_open()) {
        snprintf(response, BUFFER_SIZE, 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "404 Not Found"
        );

        *response_len = strlen(response);
        std::cout << response << std::endl;
        return;
    }

    size = file.tellg();
    file.seekg(0, std::ios::end);
    size = file.tellg() - size;

    file.seekg(0, std::ios::beg);

    *response_len = 0;
    memcpy(response, header, strlen(header));
    *response_len += strlen(header);

    // Copy contents of file to the response buffer
    while(*response_len < response_capacity) {
        size_t available = response_capacity - *response_len;
        
        file.read(response + *response_len, available);
        size_t bytes_read = file.gcount();
        *response_len += bytes_read;

        if(file.eof()) {
            break;
        }

        if(file.bad()) {
            break;
        }
    }

    file.close();
}

void *handle_client(void *args) {
    int client_fd = *((int *)args);
    std::cout << "Handled the client" << client_fd << std::endl;

    // Get the bytes received
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

    ssize_t bytes_recieved = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if(bytes_recieved > 0) {
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if(regexec(&regex, buffer, 2, matches, 0) == 0) {
            // Extract the filename
            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);
            char *file_name_copy = url_decode(url_encoded_file_name);

            // Get file extension
            char file_extension[32];
        
            const char *token = strtok(file_name, ".");
            while(token != NULL) {
                strcpy(file_extension, token);
                token = strtok(NULL, ".");
            }

            char *response = (char*)malloc(BUFFER_SIZE * 2 * sizeof(char));
            size_t response_len;

            build_http_response(file_name_copy, file_extension, response, &response_len);

            send(client_fd, response, response_len, 0);

            free(response);
            free(file_name);
            free(file_name_copy);
        }
    }

    close(client_fd);
    free(args);
    free(buffer);

    return NULL;
}

