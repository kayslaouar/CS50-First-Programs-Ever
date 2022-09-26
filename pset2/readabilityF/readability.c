#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

//for every occurence of puncutation, increment sentence count
//for every occurence of space, increment word count
//for every occurence of upper/lowercase letter, increment letter count.

int main(void)
{
    int letter_count = 0, word_count = 1, sentence_count = 0;
    string user_text = get_string("Enter some text to analyze its reading level: ");
    double L = 0.0, S = 0.0, index = 0.0;
    int grade = 0;
    for (int i = 0, string_len = strlen(user_text); i < string_len; i++)
    {
        char curr_char = user_text[i];
        if ((curr_char >= 65 && curr_char <= 90) || (curr_char >= 97 && curr_char <= 122))
        {
            letter_count++;
        }
        else if (curr_char == ' ')
        {
            word_count++;
        }
        else if (curr_char == '?' || curr_char == '!' || curr_char == '.')
        {
            sentence_count++;
        }
    }
    L = (letter_count * 100.0) / word_count;
    S = (sentence_count * 100.0) / word_count;
    index = 0.0588 * L - 0.296 * S - 15.8;
    grade = (int) round(index);
    if (grade > 1 && grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else if (grade >= 16) 
    {
        printf("Grade 16+\n");
    }
    else if (grade <= 1) 
    {
        printf("Before Grade 1\n");
    }
}