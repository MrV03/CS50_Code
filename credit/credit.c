#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //card number
    long long card_number;
    do
    {
        card_number = get_long("Number:");
    }
    while (card_number <= 0); //This should prevent negative numbers

    //Valid???
    int sum = 0, digit, count = 0;
    long long temp = card_number;

    while (temp > 0)
    {
        digit = temp % 10;

        if (count % 2 == 1)
        {
            digit *= 2;

            if (digit > 9)
            {
                digit = digit % 10 + digit / 10;
            }
        }
        sum += digit;
        temp /= 10;
        count++;

    }

    //card type
    if (sum % 10 == 0)
    {
        //American express
        if (count == 15 && (card_number / 1000000000000000 == 34 || card_number / 1000000000000000 == 37 ))
        {
            printf("AMEX\n");
        }
        //Mastercard
        else  if (count == 16 && (card_number / 1000000000000000 >= 51 && card_number / 1000000000000000 <= 55))
        {
            printf("MASTERCARD\n");
        }
        //Visa
        else if ((count == 13 || count == 16) && (card_number / 1000000000000000 == 4 || (card_number / 1000000000000000 >= 4 && card_number / 1000000000000000 <= 4)))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}
