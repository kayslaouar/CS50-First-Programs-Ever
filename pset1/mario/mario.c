#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Enter the number of levels you want to have: ");
    }
    while (!(height >= 1 && height <= 8));
    for (int i = 0, numSpaces = height - 1, numHashtags = 1; i < height; i++)
    {
        for (int j = 0; j < numSpaces; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < numHashtags; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j < numHashtags; j++)
        {
            printf("#");
        }
        printf("\n");
        numSpaces--;
        numHashtags++;
    }
}

