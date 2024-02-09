#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Pyramid Code
    for (int x = 0; x < height; x++)
    {
        for (int space = 0; space < height - x - 1; space++)
        {
            printf(" ");
        }

        for (int h = 0; h <= x; h++)
        {
            printf("#");
        }

        printf("  "); // spaces between

        for (int h = 0; h <= x; h++)
        {
            printf("#");
        }
        printf("\n");
    }

    return 0;
}
