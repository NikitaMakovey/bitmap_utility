#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct BMPHeader{       // Total: 54 bytes
    uint16_t  type;             // Magic identifier: 0x4d42
    uint32_t  size;             // File size in bytes
    uint16_t  reserved1;        // Not used
    uint16_t  reserved2;        // Not used
    uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
    uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
    int32_t   width_px;         // Width of the image
    int32_t   height_px;        // Height of image
    uint16_t  num_planes;       // Number of color planes
    uint16_t  bits_per_pixel;   // Bits per pixel
    uint32_t  compression;      // Compression type
    uint32_t  image_size_bytes; // Image size in bytes
    int32_t   x_resolution_ppm; // Pixels per meter
    int32_t   y_resolution_ppm; // Pixels per meter
    uint32_t  num_colors;       // Number of colors
    uint32_t  important_colors; // Important colors
} BMPHeader;
#pragma pack(pop)

#define HEADER_SIZE 54

int main(int argc, char** argv) {
    if (argc < 2)
    {
        printf("%s <bmp>", argv[0]);
        exit(1);
    }

    FILE *bmf = fopen(argv[1], "rb");
    FILE *bmount = fopen("output.bmp", "wb");

    BMPHeader bh;

    fread(&bh, sizeof bh, 1, bmf);
    fwrite(&bh, sizeof bh, 1, bmount);

    char *buf = malloc(bh.size);
    fread(buf, 1, bh.size, bmf);
    uint16_t bpp = bh.bits_per_pixel / 8; // bpp

    for (int i = 0; i < bh.height_px / 2; i++)
    {
        for (int j = 0; j < bh.width_px * bpp; j++)
        {
            char tmp = buf[i * (bh.width_px) * bpp + j];
            buf[i * (bh.width_px) * bpp + j] = buf[bpp * (bh.height_px - i - 1) * (bh.width_px) + j];
            buf[bpp * (bh.height_px - i - 1) * (bh.width_px) + j] = tmp;
        }
    }
    fwrite(buf, bh.size - HEADER_SIZE, 1, bmount);
    fclose(bmf);
    fclose(bmount);
    return 0;
}