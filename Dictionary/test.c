#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *str1 = "QWERTY";
    char *str2 = "qwerty";
    int i = strcasecmp(str1, str2);

    printf("%i\n", i);

    char *dictionary = argv[1];

    // Open input file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        printf("Could not open\n");
        return 1;
    }

    FILE *outptr = fopen("hash.txt", "w");
    if (outptr == NULL)
    {
        printf("Could not open\n");
        return 1;
    }

    int counter = 1;
    bool beginning = true;

    char *a = malloc(sizeof(45));
    fgets(a, 45, inptr); //read a line and put it to a buffer "a"
    printf("%s\n", a);

    fseek(inptr, 0, SEEK_SET); //set the pointer to the beginning of the file

    
    for (int c = fgetc(inptr); c != EOF; c = fgetc(inptr))
    {
        if (beginning == true)
        {
             fputc(c, outptr); //need to find out how to write integers into file
             fwrite("\n", sizeof(char), 1, outptr); 
             beginning = false;
        }
        if (c == '\n')
        {
           counter += 1;
           beginning = true;
        }
    }

    printf("number of words in dictionary is %i\n", counter);



}