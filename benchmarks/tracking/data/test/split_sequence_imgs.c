#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_IMAGE_SIZE (1024 * 1024)

// read file and split image to seperate files
int extract_images(const char* input_file) {
    FILE* file = fopen(input_file, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open input file %s\n", input_file);
        return -1;
    }

    int image_count = 0;
    size_t offset = 0;
    
    //allocate buffer to store image
    uint8_t* buffer = (uint8_t*)malloc(MAX_IMAGE_SIZE + 4);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    // read each image
    while (1) {
        // read 4 bytes length
        uint8_t length_prefix[4];
        size_t read_size = fread(length_prefix, 1, 4, file);
        if (read_size != 4) {
            if (feof(file)) {
                printf("All images have been successfully extracted.\n");
            } else {
                fprintf(stderr, "Error: Failed to read length prefix\n");
            }
            break;
        }

        // parse length
        size_t image_size = 
            (length_prefix[0] << 24) |
            (length_prefix[1] << 16) |
            (length_prefix[2] << 8)  |
            length_prefix[3];

        if (image_size > MAX_IMAGE_SIZE) {
            fprintf(stderr, "Error: Image size %zu exceeds maximum allowed size\n", image_size);
            free(buffer);
            fclose(file);
            return -1;
        }

        // read image to file
        read_size = fread(buffer, 1, image_size, file);
        if (read_size != image_size) {
            fprintf(stderr, "Error: Failed to read image data\n");
            free(buffer);
            fclose(file);
            return -1;
        }

        // 生成输出图片的文件名
        char output_filename[50];
        snprintf(output_filename, sizeof(output_filename), "output_image_%d.jpg", image_count + 1);

        // 将图片数据写入到输出文件
        FILE* output_file = fopen(output_filename, "wb");
        if (!output_file) {
            fprintf(stderr, "Error: Could not create output file %s\n", output_filename);
            free(buffer);
            fclose(file);
            return -1;
        }
        fwrite(buffer, 1, image_size, output_file);
        fclose(output_file);

        printf("Image %d extracted to %s\n", image_count + 1, output_filename);

        // 计数图片数量
        image_count++;
    }

    free(buffer);
    fclose(file);

    return 0;
}

// 主函数，接收输入文件路径
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];

    // 调用函数进行图片提取
    if (extract_images(input_file) != 0) {
        fprintf(stderr, "Failed to extract images from %s\n", input_file);
        return 1;
    }

    return 0;
}

