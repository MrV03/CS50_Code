#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
#define FILENAME_SIZE 8

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <forensic_image>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        return 1;
    }

    uint8_t buffer[BLOCK_SIZE];
    char filename[FILENAME_SIZE];
    FILE *jpeg = NULL;
    int jpeg_count = 0;

    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpeg != NULL)
            {
                fclose(jpeg);
            }
            sprintf(filename, "%03d.jpg", jpeg_count);
            jpeg = fopen(filename, "w");
            if (jpeg == NULL)
            {
                fprintf(stderr, "Could not create JPEG file %s\n", filename);
                fclose(file);
                return 1;
            }

            jpeg_count++;
        }
        if (jpeg != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, jpeg);
        }
    }

    if (jpeg != NULL)
    {
        fclose(jpeg);
    }

    fclose(file);

    return 0;
}
