#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won == true)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie == true)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (candidates[i].eliminated != true)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int k = 0; k < candidate_count; k++) //scroll through all the candidates
    {
        if (strcmp(candidates[k].name, name) == 0) //do something when found the same name
        {
            preferences[voter][rank] = k; //add the number of candidate to preferences array
            return true;
        }
    }
    return false; // if there is no such candidate

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    bool voted;
    int j; //preference order
    for (int k = 0; k < voter_count; k++) //scroll through preference 1 of all the voters
    {
        voted = false;
        j = 0;
        do
        {
            if (candidates[preferences[k][j]].eliminated == false) //checking whether the  preferred candidate is not eliminated
            {
                candidates[preferences[k][j]].votes += 1; //add votes to preferred candidate if he is not eliminated
                voted = true;
            }
            j++; // switching to next preferred candidate
        }
        while (voted == false); //repeating untill get a valid vote from the voter
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int max_votes = 0;
    int winner;
    for (int k = 0; k < candidate_count; k++)
    {
        if (max_votes < candidates[k].votes)
        {
            max_votes = candidates[k].votes;
            if (max_votes > (voter_count / 2))
            {
                printf("%s\n", candidates[k].name);
                return true;
            }
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min_votes = candidates[0].votes;
    for (int k = 0; k < candidate_count; k++)
    {
        if (candidates[k].eliminated == false) //checking only remaining candidates
        {
            if (min_votes > candidates[k].votes)
            {
                min_votes = candidates[k].votes;
            }
        }
    }
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    for (int k = 0; k < candidate_count; k++)
        if (candidates[k].eliminated == false)
        {
            if (min != candidates[k].votes)
            {
                return false;
            }
        }
    return true;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int k = 0; k < candidate_count; k++)
    {
        if (min == candidates[k].votes)
        {
            candidates[k].eliminated = true;
        }
    }
    return;
}
