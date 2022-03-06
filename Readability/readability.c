#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");
    int letters = 0;
    int words = 0;
    int sentences = 0;
    int n = (int) strlen(text);

    for (int i = 0; i < n; i++) // counting letters
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) // this condition helps to count letters only
        {
            letters = letters + 1;
        }

        else if (text[i] == ' ') // this condition helps to calculate spaces. Number of words is equal to number of spaces plus 1
        {
            words += 1;
        }

        else if (text[i] == '?' || text[i] == '!' || text[i] == '.') // this helps to count number of sentences
        {
            sentences += 1;
        }


    }
    words = words + 1; // words is equal to number of spaces plus 1
    float l = (float) letters * 100 / (float) words; // number of letters per 100 words
    float s = (float) sentences * 100 / (float) words; // number of sentences per 100 words
    float index = round(0.0588 * l - 0.296 * s - 15.8); // rounded index

    if (index < 1) // this conditions print the result
    {
        printf("Before Grade 1\n");
    }

    else if (index >= 1 && index < 16)
    {
        printf("Grade %i\n", (int) index);
    }

    else
    {
        printf("Grade 16+\n");
    }

}