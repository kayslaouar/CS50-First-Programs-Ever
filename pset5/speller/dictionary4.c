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
const unsigned int N = 5 * 7 * 11 * 13 * 17;

// prepends a node to the linked list of a bucket in our hash table
void prepend(node *prepend_node, int hash_val);

// converts all characters of a word to lower case
void to_lower(char *word);

// copies contents of immutable string to mutable string copy
void copy_constchar_to_char(const char *s1, char *s2);

// Hash table
node *table[N];

// initialize table as empty
void initialize_table();

// prints specs of hash table including size, longest chain, num words
void print_hash_table_specs();

// Array that stores the length of the linked list in every bucket of the hash table
int chain_lengths[N];

// keeps track of longest chain in hash table
int max = 0;

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
    char *word_buffer = malloc(strlen(word) + 1);
    copy_constchar_to_char(word, word_buffer);
    to_lower(word_buffer);
    int hash_val = hash(word_buffer);
    node *cur_bucket = table[hash_val];
    node *node_cursor = cur_bucket;
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

// Hashes word to a number
// hash function = multiplicative string hash function
// citation of hash function: https://learn.zybooks.com/zybook/DataStructuresEssentialsR53/chapter/5/section/7
unsigned int hash(const char *word)
{
    int hash_multiplier = 3;
    int hash_value = 2;
    for (int i = 0; i < strlen(word); i++)
    {
        hash_value = (hash_value * hash_multiplier) + word[i];
    }
    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char *word = malloc(LENGTH + 1);
    int hash_value = -1;
    initialize_table();
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
        chain_lengths[hash_value]++;
        num_words++;
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
    printf("Number of words in dictionary: %i\n", num_words);
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
            temp = cursor;
            cursor = cursor->next;
            free(temp);
            num_words--;
        }
        chain_lengths[i] = 0;
    }
    print_hash_table_specs();
    fclose(dict);
    return true;
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
    s2[strlen(s1)] = '\0';
}

void initialize_table()
{
    for(int i = 0; i < N; i++)
    {
        table[i] = NULL;
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
}