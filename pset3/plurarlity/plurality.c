#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int most_votes = 0;
    candidate winners[MAX];
    //in the case of a tie between all candidates, make sure there is enough space in the array for all candidates
    winners[0].name = "name";
    winners[0].votes = 0;
    int winners_size = 1;
    //initialize to zero with the assumption that there will be one winner
    int num_winners = 0;
    //will be used at the end. Initialized at zero because it needs to be used as an index. 
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > most_votes)
        //when there is a candidate with the most votes, update winners array with that candidate
        {
            most_votes = candidates[i].votes;
            winners[0] = candidates[i];
        }
        else if (candidates[i].votes == most_votes)
        //in the case that there are more candidates with the same number of votes, add them to the winners array
        {
            winners[winners_size] = candidates[i];
            winners_size++;
        }
    }
    for (int i = 0; i < winners_size; i++)
    {
        if (winners[i].votes == most_votes)
        //after going through the entire array in the previous for loop
        //we now need to make sure that all the candidates in the winners array have the most votes
        //since winners array is updated linearly, candidate with the most votes could be at the end of the array
        //making all the rest of the candidates toward the beginning of the array irrelevant. 
        //num_winners is our final variable for tracking the number of winners
        {
            winners[num_winners] = winners[i];
            num_winners++;
        }
    }
    for (int i = 0; i < num_winners; i++)
    {
        printf("%s\n", winners[i].name);
    }
    return;
}

