#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // TODO
    string message = get_string("Message: ");

    for (int i = 0; message[i] != '\0'; i++)
    {
        int ascii_value = (int) message[i];

        for (int j = 7; j >= 0; j--)
        {
            int bit = (ascii_value >> j) & 1;
            print_bulb(bit);
        }

        printf("\n");
    }

    return 0;
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
