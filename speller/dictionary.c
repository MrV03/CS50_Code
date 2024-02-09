// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
// Hashes word to a number
unsigned int hash(const char *word)

// TODO: Improve this hash function
// return toupper(word[0]) - 'A';
{
    unsigned int hash_value = 5381;

    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash_value = ((hash_value << 5) + hash_value) + tolower(*ptr);
    }

    return hash_value % N;
}

// Case insensitivity
int strcasecmp_custom(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        int diff = tolower(*s1) - tolower(*s2);
        if (diff != 0)
        {
            return diff;
        }
        s1++;
        s2++;
    }
    return tolower(*s1) - tolower(*s2);
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int index = hash(word);

    node *cursor = table[index];

    while (cursor != NULL)
    {

        if (strcasecmp_custom(cursor->word, word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {

        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            return false;
        }

        strncpy(new_node->word, word, LENGTH);

        int index = hash(word);

        new_node->next = table[index];
        table[index] = new_node;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int count = 0;

    for (int i = 0; i < N; i++)
    {

        node *cursor = table[i];

        while (cursor != NULL)
        {
            count++;
            cursor = cursor->next;
        }
    }

    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *temp = table[i];
            table[i] = table[i]->next;
            free(temp);
        }
    }
    return true;
}
