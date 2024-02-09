#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Input
    string text = get_string("Text: \n");

    int letter_count = 0, word_count = 0, sentence_count = 0;

    for (int i = 0, textLength = strlen(text); i <= textLength; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letter_count++;
        }

        if (text[i] == ' ' || text[i] == '\0')
        {
            word_count++;
        }

        if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            sentence_count++;
        }
    }

    // Coleman
    float avg_words_100 = (100.0 / word_count) * letter_count;
    float avg_sen_100 = (100.0 / word_count) * sentence_count;

    int index = round(0.0588 * avg_words_100 - 0.296 * avg_sen_100 - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    return 0;
}
