#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int strvict(int n);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks)) //if vote returns false
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int k = 0; k < candidate_count; k++) //scroll through all candidates and write their number in the current rank position
    {
        if (strcmp(name, candidates[k]) == 0)
        {
            ranks[rank] = k;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int k = 0; k < candidate_count; k++) //rank[k] - is the candidate which is favored over candidate rank[n]
    {
        for (int n = k + 1; n < candidate_count; n++)
        {
            preferences[ranks[k]][ranks[n]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++) // scroll through preferences array.
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] != preferences[j][i]) // if there is a preferred candidate in in the chosen pair, write it in pairs array
            {
                pair_count += 1; //increase the amount of pairs for every pair found
                if (preferences[i][j] > preferences[j][i])
                {

                    pairs[pair_count - 1].winner = i;
                    pairs[pair_count - 1].loser = j;
                }
                else
                {
                    pairs[pair_count - 1].winner = j;
                    pairs[pair_count - 1].loser = i;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swap_count;
    pair n;
    do //bubble sort
    {
        swap_count = 0;
        for (int i = 0; i < pair_count - 1; i++) //scrolling through all the pairs
        {
            if (strvict(i) < strvict(i + 1))
            {
                n = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = n;
                swap_count += 1;
            }
        }
    }
    while (swap_count != 0);

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++) //i is the number of a pair
    {
        bool gp1 = true; //1st condition
        bool gp2 = true; //2nd condition
        for (int j = 0; j < MAX; j++) //here we are checking that our winner is the source of the graph
        {
            if (locked[j][pairs[i].winner] == true)
            {
                gp1 = false;
            }
        }
        for (int j = 0; j < MAX; j++) //here we are checking there are no arrows from the dot we are going to
        {
            if (locked[pairs[i].loser][j] == true)
            {
                gp2 = false;
            }

        }

        if (gp1 == true || gp2 == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = true; // creating an arrow from winner to loser if the conditions are fulfilled
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool winner = true;
    for (int i = 0; i < candidate_count; i++)
    {
        winner = true;
        for (int j = 0; j < candidate_count; j++) //if there is an arrow pointed to the candidate he cannot be the winner
        {
            if (locked[j][i] == true)
            {
                winner = false;
            }
        }
        if (winner == true) //print winnes name if there is one with no direction to him
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

int strvict(int n) //getting the strenght of the victory of in a pair
{
    int str_vict = preferences[pairs[n].winner][pairs[n].loser] - 
                   preferences[pairs[n].loser][pairs[n].winner]; //votes for winner - votes for loser
    return str_vict;
   
}