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
const unsigned int N = 2*3*5*13*17;

// prepends a node to the linked list of a bucket in our hash table
void prepend(node *prepend_node, int hash_val);

// converts all characters of a word to lower case
void to_lower(char *word, int wordlen);

// copies contents of immutable string to mutable string copy
void copy_constchar_to_char(const char *s1, char *s2);

// Hash table
node *table[N];

// Array that stores the length of the linked list in every bucket of the hash table
int chain_lengths[N];

// keeps track of longest chain in hash table
int max = 0;

// gets lengths of all linked lists in every bucket of hash table
void longest_chain(int index);

int num_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int word_size = strlen(word);
    char* word_buffer = malloc(word_size + 1);
    copy_constchar_to_char(word, word_buffer);
    to_lower(word_buffer, word_size);
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

// Hashes word to a number
// hash function = multiplicative string hash function
// citation of hash function: https://learn.zybooks.com/zybook/DataStructuresEssentialsR53/chapter/5/section/7
unsigned int hash(const char *word)
{
    int hash_multiplier = 7;
    long hash_value = 11;
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
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        free(word);
        fclose(dict);
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
    fclose(dict);
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
    printf("Number of words: %i\n", num_words);
    node *cursor = table[0];
    node *temp = table[0];
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        longest_chain(i);
        while (cursor != NULL)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
            num_words--;
        }
        chain_lengths[i] = 0;
    }
    printf("longest chain in hash table, %i\n", max);
    printf("Number of words: %i\n", num_words);
    return true;
}

void prepend(node *prepend_node, int hash_val)
{
    node *temp = table[hash_val];
    prepend_node->next = temp;
    table[hash_val] = prepend_node;
}

void to_lower(char *word, int wordlen)
{
    for (int i = 0; i < wordlen; i++) {
        if (isupper(word[i]) != 0) {
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