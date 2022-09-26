import cs50


def main():
    card_num = cs50.get_int("Number: ")
    num_len = len(str(card_num))
    if (not(is_valid_credit_card(card_num, num_len))):
        print("INVALID")
    elif (is_visa(card_num, num_len)):
        print("VISA")
    elif(is_mastercard(card_num, num_len)):
        print("MASTERCARD")
    elif(is_amex(card_num, num_len)):
        print("AMEX")
    else:
        print("INVALID")

# converts input card_num to a string and reverses the string


def reverse_string(card_num, num_len):
    string = str(card_num)
    reverse_string = ""
    for i in range(num_len - 1, -1, -1):
        reverse_string += string[i]
    return reverse_string

# performs Luhn's credit card validity algorithm


def check_sum(card_num, num_len):
    sums = []
    check_sum = 0
    reverse_str = reverse_string(card_num, num_len)
    for i in range(1, num_len, 2):
        sums.append(str(int(reverse_str[i]) * 2))
    for i in range(0, num_len, 2):
        sums.append(reverse_str[i])
    digits = parse_digits(sums)
    for i in range(len(digits)):
        check_sum += int(digits[i])
    return check_sum % 10 == 0

# ensures that final list of digits in check sum calculation are all single digit elements


def parse_digits(sums):
    digits = []
    for i in range(len(sums)):
        if(len(sums[i]) == 2):
            digits.append(sums[i][:1])
            digits.append(sums[i][1:])
        else:
            digits.append(sums[i])
    return digits

# checks first digits of card number


def check_first_n_digits(card_num, n):
    first_digits = str(card_num)
    first_digits = first_digits[:n]
    return int(first_digits)


def is_valid_credit_card(card_num, num_len):
    return check_sum(card_num, num_len)

# checks if card is visa based on the first digits of the card and the length of the card number


def is_visa(card_num, num_len):
    return check_first_n_digits(card_num, 1) == 4 and (num_len == 13 or num_len == 16)

# checks if card is mastercard based on the first digits of the card and the length of the card number


def is_mastercard(card_num, num_len):
    possible_first_digits = [51, 52, 53, 54, 55]
    return check_first_n_digits(card_num, 2) in possible_first_digits and num_len == 16

# checks if card is amex based on the first digits of the card and the length of the card number


def is_amex(card_num, num_len):
    possible_first_digits = [34, 37]
    return check_first_n_digits(card_num, 2) in possible_first_digits and num_len == 15


# executes main program
main()