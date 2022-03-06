// Implements a dictionary's functionality

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

#define LENGHT 45

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
unsigned int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int key = hash(word);
    node *cursor = table[key];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    char tmp = tolower(word[0]); //in order this to work we need to convert first letter to lowercase
    int hash_key = tmp - 97;
    return hash_key;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open a file with the dictionary
    FILE *inpt_dict = fopen(dictionary, "r");
    if (inpt_dict == NULL) //checking that the file opens
    {
        return false;
    }

    char* word = malloc(LENGHT + 1); //here we will store our words
    int key;

    while (fscanf(inpt_dict, "%s", word) != EOF) //read dictionary word by word until EOF reached
    {
        key = hash(word); //calculate hash key

        node *n = malloc(sizeof(node)); //allocate memory for a new node;
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word); //copy a word from dictionary into new node
        n->next = table[key]; //copy the address from the appropriate table node into new node
        table[key] = n; //copy the address of the new node into appropriate table node

        word_count += 1; //increment word counter by 1
    }

    fclose(inpt_dict);
    free(word);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int x = 0; x < N; x++)
    {
        node *cursor2 = table[x];
        while (cursor2 != NULL)
        {
            node *tmp = cursor2;
            cursor2 = cursor2->next;
            free(tmp);
        }
    }
    return true;
}
