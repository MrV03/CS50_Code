#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool is_valid_key(string key);

string encrypt(string plaintext, string key);

int main(int argc, string argv[])
{
    if (argc !=2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];

    if(!is_valid_key(key))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    string plaintext = get_string("Plaintext: ");

    string ciphertext = encrypt(plaintext, key);

    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}

bool is_valid_key(string key)
{
    int key_length = strlen(key);

    if(key_length != 26)
    {
        return false;
    }

    int freq[26] = {0};

    for (int i = 0; i < key_length; i++)
    {
        if(!isalpha(key[i]))
        {
            return false;
        }

        int index = tolower(key[i]) - 'a';

        if (freq[index] > 0)
        {
            return false;
        }

        freq[index]++;
    }

    return true;
}

string encrypt(string plaintext, string key)
{
    int text_length = strlen(plaintext);
    string ciphertext = plaintext;

    for (int i = 0; i < text_length; i++)
    {
        if (isalpha(plaintext[i]))
        {
            char original_case = isupper(plaintext[i]) ? 'A' : 'a';
            int index = tolower(plaintext[i]) - a;
            ciphertext[i] = isupper(plaintext[i]) ? toupper(key[index]);
        }
    }

    return ciphertext;
}
