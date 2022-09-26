#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

bool no_duplicates_checker(char input_char, char array[], int array_size);
char char_switch_statement(char input_char, string key);
//string to_string(char array[], int array_size);

int main(int argc, string argv[])
{
    string user_key = "";
    string error_message = "Usage: ./substitution key";
    char key_array[26];
    int key_array_size = 0;
    if (argc != 2)
    // checks if user does not type in 2 command arguments
    {
        printf("%s\n", error_message);
        return 1;
    }
    else
    {
        if (strlen(argv[1]) != 26)
        // checks if user does not input a proper key of 16 characters
        {
            printf("Key must contain 26 alphabetic characters.\n");
            return 1;
        }
        else
        {
            for (int i = 0; i < 26; i++)
            {
                char curr_char = argv[1][i];
                // for loop runs through all characters in string stored in argv[1]
                if ((curr_char >= 65 && curr_char <= 90) || (curr_char >= 97 && curr_char <= 122))
                // checks if characters are letters only
                {
                    if (no_duplicates_checker(curr_char, key_array, key_array_size))
                    // function uses and updates an array to make sure that there are no duplicate letters
                    {
                        key_array_size++;
                        //increments arraysize after item is added to array so item is added to next open index
                    }
                    else
                    {
                        printf("Key must contain each letter only once.\n");
                        return 1;
                        break;
                    }
                }
                else
                {
                    printf("Key must contain only alphabetic characters.\n");
                    return 1;
                    break;
                }
            }
            user_key = argv[1];
            // only runs if there were no issues, and the user inputted a valid key
        }
    }
    string user_text = get_string("plaintext: ");
    int user_text_length = strlen(user_text);
    char cipher_text[user_text_length];
    //string final_cipher = "";
    char user_char = ' ';
    char cipher_char = ' ';
    for (int i = 0; i < user_text_length; i++)
    {
        user_char = user_text[i];
        cipher_char = char_switch_statement(user_char, user_key);
        //passes in user_key and user_char to determine cipher version and uppercase/lowercase status
        cipher_text[i] = cipher_char;
        //special function of string.h that allows strings to append characters. Last param specifies number of chars to add
    }
    //final_cipher = to_string(cipher_text, user_text_length);
    printf("ciphertext: %s", cipher_text);
    printf("\n");
    return 0;
    // main function returns 0 to indicate successful completion of program
}

/*string to_string(char array[], int array_size)
{
    string result = "";
    for (int i = 0; i < array_size; i++)
    {
        result[i] = array[i];
    }
    result[array_size] = '\0';
    return result;
}*/

bool no_duplicates_checker(char input_char, char array[], int array_size)
{
    bool result = true;
    for (int i = 0; i < array_size; i++)
    {
        if (input_char == array[i])
        {
            result = false;
            break;
        }
    }
    if (result)
    //checks if input_char has not been seen before to add it to the array
    {
        array[array_size] = input_char;
        // input_char assigned to array_size because array_size starts at 0 and is incremented after item is added to array.
        // first item is added to index 0, and last item is added to index 25.
    }
    return result;
}

char char_switch_statement(char input_char, string key)
{
    bool upper_case = false;
    //keep track of case for end so we can run the toUpper functoin once (rather than in each switch statement) at end if necessary
  //  if (! ((input_char >= 'A' && input_char <= 'Z') || (input_char >= 'a' && input_char <= 'z')))
    if (!((input_char >= 65 && input_char <= 90) || (input_char >= 97 && input_char <= 122 )))
    {
        return input_char;
    }
    if (input_char >= 65 && input_char <= 90)
    {
        upper_case = true;
    }
    //each letter in user key directly correspnds to the order of the alphabet
    switch(input_char)
    {
        case ('A'):
        case ('a'):
            input_char = key[0];
            break;
        case ('B'):
        case ('b'):
            input_char = key[1];
            break;
        case ('C'):
        case ('c'):
            input_char = key[2];
            break;
        case ('D'):
        case ('d'):
            input_char = key[3];
            break;
        case ('E'):
        case ('e'):
            input_char = key[4];
            break;
        case ('F'):
        case ('f'):
            input_char = key[5];
            break;
        case ('G'):
        case ('g'):
            input_char = key[6];
            break;
        case ('H'):
        case ('h'):
            input_char = key[7];
            break;
        case ('I'):
        case ('i'):
            input_char = key[8];
            break;
        case ('J'):
        case ('j'):
            input_char = key[9];
            break;
        case ('K'):
        case ('k'):
            input_char = key[10];
            break;
        case ('L'):
        case ('l'):
            input_char = key[11];
            break;
        case ('M'):
        case ('m'):
            input_char = key[12];
            break;
        case ('N'):
        case ('n'):
            input_char = key[13];
            break;
        case ('O'):
        case ('o'):
            input_char = key[14];
            break;
        case ('P'):
        case ('p'):
            input_char = key[15];
            break;
        case ('Q'):
        case ('q'):
            input_char = key[16];
            break;
        case ('R'):
        case ('r'):
            input_char = key[17];
            break;
        case ('S'):
        case ('s'):
            input_char = key[18];
            break;
        case ('T'):
        case ('t'):
            input_char = key[19];
            break;
        case ('U'):
        case ('u'):
            input_char = key[20];
            break;
        case ('V'):
        case ('v'):
            input_char = key[21];
            break;
        case ('W'):
        case ('w'):
            input_char = key[22];
            break;
        case ('X'):
        case ('x'):
            input_char = key[23];
            break;
        case ('Y'):
        case ('y'):
            input_char = key[24];
            break;
        case ('Z'):
        case ('z'):
            input_char = key[25];
            break;
    }
    if (upper_case)
    {
        input_char = toupper(input_char);
    }
    else
    {
        input_char = tolower(input_char);
    }
    //last two branches ensure that the proper case is assigned to the result and returned
    return input_char;
}