#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define LENGHT 45

typedef struct node
{
    char word[LENGHT + 1];
    struct node *next;
}
node;

const unsigned int N = 26; //number of buckets in hash table
unsigned int word_count = 0;

node *table[N]; //hash table

int hash(const char *word); //prototype for a hash function
bool load(const char *dictionary); //prototype for a load function
bool check(const char *word); //prototype for a checking function

int main(void)
{
    char *dictionary = malloc(45);
    strcpy(dictionary, "dictionaries/small"); //the address of dictionary
    load(dictionary);

    char *word = malloc(LENGHT + 1);
    printf("enter your word: ");
    scanf("%s", word); //getting a word to test

    check(word);

    printf("%i\n", word_count);
    
    //unload
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
    
    free(dictionary);
    free(word);

}


//hash function
int hash(const char *word)
{
    char tmp = tolower(word[0]); //in order this to work we need to convert first letter to lowercase
    int hash_key = tmp - 97;
    return hash_key;
}

bool load(const char *dictionary)
{
    //open a file with the dictionary
    FILE *inpt_dict = fopen(dictionary, "r");
    if (inpt_dict == NULL) //checking that the file opens
    {
        printf("Couldn't open the dictionary\n");
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
            printf("Cannot allocete memory for a new node\n"); //check that memory is allocated
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

bool check(const char *word)
{
    int key = hash(word);
    node *cursor = table[key];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            printf("correct\n");
            return true;
        }
        cursor = cursor->next;
    }

    printf("incorrect\n");
    return false;

}

