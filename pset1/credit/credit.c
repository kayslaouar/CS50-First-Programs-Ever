#include <stdio.h>
#include <cs50.h>
#include <math.h>
//1 for potentially AMEX, 2 for potentially MASTERCARD, 3 for POTENTIALLY VISA
int digits_check(long); //method for checking number of digits of entry
int starting_digits_check(long, int);//method for checking first digits of number
bool check_sum(long, int);//method for checksum
int check_sum_operations(long, int);
int check_sum_operations_2(int);
//method for confirming type: string

int main(void)
{
    long credit_card_number = get_long("Enter your credit card number: ");
    int num_digits = 0;
    string credit_card_type = "DID NOTHING";
    int prediction = 0;
    bool check_sum_test = false;

    num_digits = digits_check(credit_card_number);

    if (num_digits != 0)
    {
        //if passes digits_check, move to first_digits_check, else INVALID
        prediction = starting_digits_check(credit_card_number, num_digits);
        if (prediction != 0)
        {
            check_sum_test = check_sum(credit_card_number, num_digits);
            if (check_sum_test)
            {
                if (prediction == 1)
                {
                    credit_card_type = "AMEX";
                }
                else if (prediction == 2)
                {
                    credit_card_type = "MASTERCARD";
                }
                else if (prediction == 3)
                {
                    credit_card_type = "VISA";
                }
                else
                {
                    credit_card_type = "INVALID";
                }
            }
            else
            {
                credit_card_type = "INVALID";
            }
        }
        else
        {
            credit_card_type = "INVALID";
        }
    }
    else
    {
        credit_card_type = "INVALID";
    }
    printf("%s\n", credit_card_type);
}

int digits_check(long param)
{
    int result = 0;
    if (param < 1000000000000 || param >= 10000000000000000)
    {
        result = 0;
    }
    else if (param < 10000000000000)
    {
        result = 13;
    }
    else if (param < 100000000000000)
    {
        //credit card numbers never have 14 digits so we return 0 to indiciate a fake
        result = 0;
    }
    else if (param < 1000000000000000)
    {
        result = 15;
    }
    else if (param < 10000000000000000)
    {
        result = 16;
    }
    return result;
}

int starting_digits_check(long credit_num, int num_digits)
{
    long divisor = pow(10, num_digits - 2);
    int first_two_digits = credit_num / divisor; //finds first two digits in credit card num
    int first_digit = first_two_digits / 10; //finds first digit in credit card num
    int prediction = 0;
    bool MC_in_range = (first_two_digits > 50 && first_two_digits < 56);
    if (num_digits == 15 && (first_two_digits == 34 || first_two_digits == 37))
    {
        prediction = 1;
    }
    else if (num_digits == 16 && (MC_in_range))
    {
        prediction = 2;
    }
    else if ((num_digits == 13 || num_digits == 16) && first_digit == 4)
    {
        prediction = 3;
    }
    return prediction;
}

bool check_sum(long credit_num, int num_digits)
{
    bool result = false;
    int curr_digit = 0;
    int sum_digits = 0;
    for (int i = 2; i <= num_digits; i += 2)
    {
        curr_digit = check_sum_operations(credit_num, i);
        sum_digits += check_sum_operations_2(curr_digit);
        //calls function which performs checksum operations.
        //Method used for better code readability.
    }
    for (int i = 1; i <= num_digits; i += 2)
    {
        curr_digit = check_sum_operations(credit_num, i);
        sum_digits += curr_digit;
    }
    if (sum_digits % 10 == 0)
    {
        result = true;
    }
    return result;
}

int check_sum_operations(long credit_num, int i)
{
    long divisor = 0, divisor_2 = 0;
    divisor = pow(10, i);
    divisor_2 = divisor / 10;
    //first we use modulo on credit card num to get last numbers,
    //then we use division at pow(10, i - 1) to get rid of all digits that follow the one we want.
    //pow(10, i - 1) = divisor / 10
    return (credit_num % divisor) / divisor_2;
}

int check_sum_operations_2(int input)
{
    int new_val = input * 2;
    int first_digit = 0, second_digit = 0;
    if (new_val >= 10)
    {
        first_digit = new_val % 10;
        second_digit = new_val / 10;
        new_val = first_digit + second_digit;
    }
    return new_val;
}
/*use a math power function and a for loop up to but less than that power to determine
the number of times you must modulo to find each other digit in the number*/