#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long number = get_long("Number: "); //prompts to enter the number
    long number2 = number; 
    long result = 0; // variable for first sum (every second number miltiplied by 2)
    long result2 = 0; // viriable for second sum of the digits which were not multiplied
    for (int i = 0; i < 8; i++) // this cicle takes every second number starting from the end, multiplies by too and sum them up
    {
        long k = number2 / 10 % 10 * 2;
        if (k > 9) //this checks if the product is bigger then 9, e.g. consists of 2 digits and deals with it
        {
            k = k / 10 + k % 10;
            number2 = number2 / 100;
            result = result + k;
        } 
        else
        {
            result = result + k;
            number2 = number2 / 100;
        }
    }
    number2 = number;
    for (int i = 0; i < 8; i++) //this cicle calculates the second checksum adding up every second digit which we not multiplied
    {
        long j = number2 % 10;
        number2 = number2 / 100;
        result2 = result2 + j;
    }

    long total = result + result2;
    if (total % 10 == 0) // this decides whether card  is valid
    {
        if (number / 10000000000000 == 34 || number / 10000000000000 == 37)
        {
            printf("AMEX\n");
        }
        else if (number / 100000000000000 > 50 && number / 100000000000000 < 56)
        {
            printf("MASTERCARD\n");
        }
        else if (number / 1000000000000 == 4 || number / 10000000000000 == 4 || number / 100000000000000 == 4 || number  / 1000000000000000 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
         printf("INVALID\n");
    }

}
