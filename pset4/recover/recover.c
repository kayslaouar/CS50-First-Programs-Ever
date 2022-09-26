#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *memory_card = fopen(argv[1], "r");

    if (memory_card == NULL)
    {
        printf("Unable to open %s/n", argv[1]);
        return 1;
    }

    int num_jpegs = 0;
    unsigned char bytes[512];
    char image[8];
    FILE *jpeg;

    while (fread(bytes, 512, 1, memory_card) == 1)
    {
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            if (num_jpegs > 0) // detected first JPEG
            {
                fclose(jpeg);
            }
            sprintf(image, "%03i.jpg", num_jpegs);
            jpeg = fopen(image, "a");
            fwrite(bytes, 512, 1, jpeg);
            num_jpegs++;
        }
        else if (num_jpegs > 0)
        {
            fwrite(bytes, 512, 1, jpeg);
        }
    }

    // close files

    fclose(jpeg);
    fclose(memory_card);
    return 0;
}