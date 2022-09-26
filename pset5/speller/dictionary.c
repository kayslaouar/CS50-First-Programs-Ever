// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// sets N to a very large prime number that is just 2 larger than dictionaries/large in size!
const unsigned int N = 143063;

// prepends a node to the linked list of a bucket in our hash table
void prepend(node *prepend_node, int hash_val);

// converts all characters of a word to lower case
void to_lower(char *word);

// copies contents of immutable string to mutable string copy
void copy_constchar_to_char(const char *s1, char *s2);

// Hash table
node *table[N];

// prints specs of hash table including size, longest chain, num words
void print_hash_table_specs();

// gets rough distribution of hash clustering
void get_hashing_distribution();

// Array that stores the length of the linked list in every bucket of the hash table
int chain_lengths[N];

// keeps track of longest chain in hash table
int max = 0;
int num_linked_lists[25];
void check_num_linked_lists(int i, int j);

// gets lengths of all linked lists in every bucket of hash table
void longest_chain(int index);

// tracks the number of buckets used in the hash table
int num_buckets_used;

// tracks number of words there are in the dictionary
int num_words = 0;

// local copy of dictionary. Opened in load, closed in unload
FILE *dict = NULL;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char *word_buffer = calloc(strlen(word) + 1, 1);
    copy_constchar_to_char(word, word_buffer);
    to_lower(word_buffer);
    int hash_val = hash(word_buffer);
    node *node_cursor = table[hash_val];
    while (node_cursor != NULL)
    {
        if (strcmp(node_cursor->word, word_buffer) == 0)
        {
            free(word_buffer);
            return true;
        }
        node_cursor = node_cursor->next;
    }
    free(word_buffer);
    return false;
}

unsigned int hash(const char *word)
{
    unsigned long *result = malloc(sizeof(int) * 25);
    *result = 1;
    int hash_mod[5] = {7, 11, 13, 17, 19};
    int mult = 29;
    int hash = 23;
    int word_len = strlen(word);
    for (int i = 0; i < word_len; i++)
    {
        *result = *result * (hash * mult) + word[i];
        if (i % 2 == 0)
        {
            *result = *result % hash_mod[i % 5];
        }
    }
    int int_result = (int) (*result % N);
    free(result);
    return int_result;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char *word = malloc(LENGTH + 1);
    int hash_value = -1;
    dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        free(word);
        return false;
    }
    while (fscanf(dict, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        strcpy(new_node->word, word);
        hash_value = hash(word);
        prepend(new_node, hash_value);
        num_words++;
        chain_lengths[hash_value]++;
    }
    free(word);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return num_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    int j = 0;
    node *cursor = table[0];
    node *temp = table[0];
    for (int i = 0; i < N; i++)
    {
        longest_chain(i);
        cursor = table[i];
        if (table[i] != NULL) {
            num_buckets_used++;
        }
        while (cursor != NULL)
        {
            j++;
            temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        check_num_linked_lists(i, j);
        j = 0;
    }
    print_hash_table_specs();
    fclose(dict);
    return true;
}

void check_num_linked_lists(int i, int j)
{
    num_linked_lists[j]++;
}

void prepend(node *prepend_node, int hash_val)
{
    node *temp = table[hash_val];
    prepend_node->next = temp;
    table[hash_val] = prepend_node;
}

void to_lower(char *word)
{
    int str_len = strlen(word);
    for (int i = 0; i < str_len; i++)
    {
        if (isupper(word[i]) != 0)
        {
            word[i] = tolower(word[i]);
        }
    }
}

void copy_constchar_to_char(const char *s1, char *s2)
{
    for (int i = 0; i < strlen(s1); i++)
    {
        s2[i] = s1[i];
    }
}

void longest_chain(index) {
    if (chain_lengths[index] > max)
    {
        max = chain_lengths[index];
    }
}

void print_hash_table_specs() {
    printf("Hash table size: %i\n", N);
    printf("Number of buckets used in hash table: %i\n", num_buckets_used);
    printf("Max chain length in hash table %i\n", max);
    printf("Spread: %f\n\n", ((float) num_buckets_used) / (float) N);
    for (int i = 0; i < 25; i++)
    {
        printf("Number of linked lists with %i elements in table: %i\n", i, num_linked_lists[i]);
        printf("Total Number of elements in linked list: %i\n\n", i * num_linked_lists[i]);
        if (num_linked_lists[i] == 0)
        {
            break;
        }
    }
    get_hashing_distribution();
}

void get_hashing_distribution()
{
    int sum = 0;
    int number_of_segments = 50;
    int segment_size = (int) ((float) N / (float) number_of_segments);
    for (int i = 0; i < N; i += segment_size)
    {
        for (int j = i; j < (i + segment_size) && j < N; j++)
        {
            sum = sum + chain_lengths[j];
        }
        sum = (int) ((float)sum / (float) segment_size);
        printf("%i\n", sum);
        sum = 0;
    }
}