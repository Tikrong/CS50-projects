#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float change;
    do
    {
        change = get_float("Сhange owed: ");
    }
    while (change <= 0);

    int cents = round(change * 100);
    int coins = cents / 25; // getting the amount of 25c coins
    int remain = cents % 25; // getting the remaining amount of change owed
    if (remain > 0)
    {
        coins = coins + remain / 10; // getting the amount of 10c coins
        remain = remain % 10; // getting the remaining amount of change owed
    }
    if (remain > 0)
    {
        coins = coins + remain / 5; // getting the amount of 5c coins
        remain = remain % 5; // getting the remaining amount of change owed
    }
    coins = coins + remain; // getting the final amount of coins
    printf("%i\n", coins);
}
