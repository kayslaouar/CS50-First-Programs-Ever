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
int get_margin(int i, int j);


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

            if (!vote(j, name, ranks))
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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //for second for loop start at 1
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
        /*we want the second for loop to ignore rows that have already been updated.
        ranks[i] is a candidate's index in the candidates array
        ranks[j] represents all the candidates that rank below ranks[i].
        This is how we select our row and column in the preferences table
        in preferences[k][l], if k = ranks[i] and l = ranks[j], incrementing preferences[ranks[i]][ranks[j]]
        We increment preferences[ranks[i]][ranks[j]] because it updates how many people prefer candidate k to candidate l.
        */
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int index_largest = -1;
    int curr_margin = -1;
    int largest_margin = -1;
    pair temp;
    for (int i = 0; i < pair_count - 1; i++)
    {
        index_largest = i;
        largest_margin = get_margin(pairs[i].winner, pairs[i].loser);
        for (int j = i + 1; j < pair_count; j++)
        {
            curr_margin = get_margin(pairs[j].winner, pairs[j].loser);
            if (curr_margin > largest_margin)
            {
                largest_margin = curr_margin;
                index_largest = j;
            }
        }
        temp = pairs[i];
        pairs[i] = pairs[index_largest];
        pairs[index_largest] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // checks if there are no "true" bools stored in the column of the person that loses to last place.
    // no trues indicates that the column candidate (last_place.loser) is the "source"
    // nobody can point to the source to avoid a loop
    // if there is a true bool in the column, it means that someone else points to the loser and the last_place.loser isn't the source
    bool true_columns[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        true_columns[i] = false;
    }
    // before locking, one column must always have 0 trues, else skip the pair that would add a true to that column,
    // given all other columns have a true
    int true_columns_counter = 0;
    for (int i = 0; i < pair_count; i++)
    {
        if (true_columns[pairs[i].loser])
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        else if (true_columns_counter < candidate_count - 1)
        {
            true_columns_counter++;
            true_columns[pairs[i].loser] = true;
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winner = -1;
    bool found = false;
    for (int i = 0; i < candidate_count && !found; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                break;
            }
            else if (j == candidate_count - 1)
            {
                winner = i;
                found = true;
            }
        }
        if (found)
        {
            break;
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}

int get_margin(int i, int j)
{
    return preferences[i][j] - preferences[j][i];
}