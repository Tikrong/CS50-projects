#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc != 2) //checking for the single argument
    {
        printf("error: you must choose the key\n");
        return 1;
    }
    
    for (int j = 0; j < strlen(argv[1]); j++) //checking that key is positive integer
    {
        if (argv[1][j] < '0' || argv[1][j] > '9')
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    
    
    int k = atoi(argv[1]);
    k = k % 26; //adapting the key for it to be from 0 to 25, as key 27 is equal to key 1
     
    string text = get_string("plaintext: "); //getting plaintext
   
    printf("ciphertext: ");  
   
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z') // encrypting capital letters
        {
            if ((text[i] + k) > 90)
            {
                printf("%c", text[i] + k - 26);
            }
            else
            {
                printf("%c", text[i] + k);
            }
        }
        
        else if (text[i] >= 'a' && text[i] <= 'z') // encrypting lowercase letters
        {
            if ((text[i] + k) > 122)
            {
                printf("%c", text[i] + k - 26);
            }
            else
            {
                printf("%c", text[i] + k);
            }
        }
        
        else
        {
            printf("%c", text[i]); // leaving other symbols and spaces as they are
        }
    }
    
    printf("\n");
    return 0;

}