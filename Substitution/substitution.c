#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int  main(int argc, string argv[])
{

    if (argc != 2) //checking that there is only one argument
    {
        printf("Error: enter the correct key\n");
        return 1;
    }

    if (strlen(argv[1]) != 26) //checking that there is 26 letters in the key
    {
        printf("Error: your key must contain 26 letters\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++) //checking that the key consists only of letters
    {
        if (argv[1][i] < 'A' || (argv[1][i] > 'Z' && argv[1][i] < 'a') || argv[1][i] > 'z')
        {
            printf("Error: your key should contain only letters\n");
            return 1;
        }
    }

    for (int i = 0; i < strlen(argv[1]); i++) //checking that each letter is used only once
    {
        for (int j = i + 1; j < strlen(argv[1]); j++)
        {
            if (tolower(argv[1][i]) == tolower(argv[1][j]))
            {
                printf("Error: every letter in the key must be unique\n");
                return 1;
            }
        }
    }

    string text = get_string("plaintext: "); //getting the plaintext from  the user
    int l = strlen(text);
    printf("ciphertext: ");

    for (int i = 0; i < l; i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z') // encrypting uppercase letters
        {
            printf("%c", toupper(argv[1][text[i] - 65]));
        }

        else if (text[i] >= 'a' && text[i] <= 'z') // encrypting lowercase letters
        {
            printf("%c", tolower(argv[1][text[i] - 97]));
        }

        else
        {
            printf("%c", text[i]); //leaving non letter symbols as they are
        }
    }
    printf("\n");
    return 0;
}