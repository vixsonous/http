#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "iostream"
#include "http/http.hpp"

#define PORT 8080


int main() {
    int server_fd = init_server(PORT);

    std::cout << "Hello world" << std::endl;

    // Handle client connections
    while(1) {
        // Client info
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = (int*)malloc(sizeof(int));

        // Accept client connection
        if((*client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("Client accept failed");
            continue;
        }

        // Create new thread to handle the request
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)client_fd);
        pthread_detach(thread_id);

    }
}