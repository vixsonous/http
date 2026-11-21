#include "file.hpp"
#include "iostream"
#include "string.h"
#include "math.h"

MimeTypeEntry get_mime_type(const char* file_extension) {
    uint16_t mime_type_idx = floor(MIME_TYPES_LENGTH / 2);
    uint16_t prev_mime_type_idx = mime_type_idx;

    uint16_t top_ceiling = MIME_TYPES_LENGTH - 1;
    uint16_t bottom_ceiling = 0;

    uint8_t is_found = 0;

    MimeTypeEntry ret;

    // Perform binary search on the file types
    while(is_found == 0) {
        prev_mime_type_idx = mime_type_idx;
        if(strcmp(file_extension, mime_types[mime_type_idx].file_extension) > 0) {
            bottom_ceiling = mime_type_idx;
            mime_type_idx = floor((bottom_ceiling + top_ceiling) / 2);
        } else if(strcmp(file_extension, mime_types[mime_type_idx].file_extension) < 0) {
            top_ceiling = mime_type_idx;
            mime_type_idx = floor((bottom_ceiling + top_ceiling) / 2);
        }

        if(strcmp(file_extension, mime_types[mime_type_idx].file_extension) == 0) {
            ret = mime_types[mime_type_idx];
            is_found = 1;
        }

        if(prev_mime_type_idx == mime_type_idx) { // no change in mime type idx, meaning it kept going (file extension not in this record)
            ret = {file_extension: "n/a", file_mime_type: "n/a"};
            is_found = 1;
        }
    }
        
    return ret;
}


