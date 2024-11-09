#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_IMAGE_SIZE (1024 * 1024)

// read image and write data to buffer
int read_image_and_write_to_buffer(const char* image_path, uint8_t* buffer, size_t* offset) {
    FILE* file = fopen(image_path, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", image_path);
        return -1;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    size_t image_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // check image size
    if (image_size > MAX_IMAGE_SIZE) {
        fprintf(stderr, "Error: Image %s exceeds maximum size of 1MB\n", image_path);
        fclose(file);
        return -1;
    }

    // write 4 bytes image length
    buffer[*offset] = (image_size >> 24) & 0xFF;
    buffer[*offset + 1] = (image_size >> 16) & 0xFF;
    buffer[*offset + 2] = (image_size >> 8) & 0xFF;
    buffer[*offset + 3] = image_size & 0xFF;
    *offset += 4;

    // read image data to buffer 
    size_t read_size = fread(buffer + *offset, 1, image_size, file);
    if (read_size != image_size) {
        fprintf(stderr, "Error: Failed to read image data from %s\n", image_path);
        fclose(file);
        return -1;
    }
    *offset += image_size;

    fclose(file);
    return 0;
}

// write data to file
void write_to_file(const char* output_file, uint8_t* buffer, size_t buffer_size) {
    FILE* file = fopen(output_file, "wb");
    if (!file) {
        fprintf(stderr, "Error: Could not open output file %s\n", output_file);
        return;
    }

    fwrite(buffer, 1, buffer_size, file);
    fclose(file);
    printf("Data successfully written to %s\n", output_file);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <output_file> <image1> <image2> ... <imageN>\n", argv[0]);
        return 1;
    }

    const char* output_file = argv[1];

    size_t max_total_size = argc * (MAX_IMAGE_SIZE + 4);
    uint8_t* buffer = (uint8_t*)malloc(max_total_size);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    size_t offset = 0;
    for (int i = 2; i < argc; ++i) {
        if (read_image_and_write_to_buffer(argv[i], buffer, &offset) != 0) {
            free(buffer);
            return 1;
        }
    }

    write_to_file(output_file, buffer, offset);

    free(buffer);

    return 0;
}

