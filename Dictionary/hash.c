#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

#define LENGHT 45

typedef struct node
{
    char word[LENGHT + 1];
    struct node *next;
}
node;

const unsigned int N = 26; //number of buckets in hash table

    node *table[N]; //hash table

int main(void)
{

    for (int x = 0; x < 26; x++)
    {
        table[x] = malloc(sizeof(node));
        table[x]->next = NULL; // WHY THERE IS A SEGMENTATION FAULT???
        printf("%p\n", table[x]->next);
    }

    FILE *inptr = fopen("dictionaries/small", "r");
    int key = 0; //setting key to zero
    char *word = malloc(LENGHT + 1);
    int index = 0;
    node *tmp; //temporary node for new info

    for (int a = fgetc(inptr); a != EOF; a = fgetc(inptr))
    {
        if (a - 97 == key)
        {
            while (a != '\n' && a != EOF)
            {
                word[index] = a; //add new letter to the word
                index++;
                a = fgetc(inptr); //go next letter
            }
            index = 0;
        }

        else if (a - 97 != key)
        {
            key = a - 97;
            while (a != '\n' && a != EOF)
            {
                word[index] = a; //add ne letterto the word while it's not the end of the word
                index++;
                a = fgetc(inptr); //go to the next letter
            }
            index = 0;
        }
        tmp = malloc(sizeof(node));
        int k = -1;
        do
        {
            k++;
            tmp->word[k] = word[k];
        }
        while (word[k] != '\n');
        tmp->next = table[key]->next; //put the address from the table to tmp node
        table[key]->next = tmp; //put the address of the new node at the table
        printf("%s - %i\n", word, key); //here we should add our word to the hash table
        free(word);
        word = malloc(LENGHT + 1);

    }

    printf("%s\n", table[2]->next->next->word);
    printf("%s\n", table[2]->next->word);

    char *test_word = malloc(45);
    scanf("%s", test_word); //getting the word for checking

    node *test_node = malloc(sizeof(node)); transforming the word being checked into node form with 46 symbols
    int i = 0;
    while (test_word[i] != '\n')
    {
        test_node->word[i] = test_word[i];
        i++;
    }



    printf("your word is - %s\n", test_word);

    //here will be a hashfunction
    int hash_key = test_node->word[0] - 97;
    printf("hash key is - %i\n", hash_key);

    //here will be function for checking whether our word is in the dictionary
    node *tmp_search = table[hash_key];
    while(tmp_search != NULL)
    {
        if (strcasecmp(tmp_search->word, test_node->word) == 0)
        {
            printf("the word is correct\n");
            break;
        }
        tmp_search = tmp_search->next;
        if (tmp_search == NULL)
        {
            printf("the word is not correct\n");
        }
    }

    printf("%s\n", test_node->word);

    free(word); //we must free allocated memory
    fclose(inptr); //we should close the sourse file

}