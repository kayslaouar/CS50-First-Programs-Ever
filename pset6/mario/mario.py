import cs50

ask_again = True

while ask_again:
    num_levels = cs50.get_int("How many levels do you want in your pyramid. Enter an integer between 1 and 8 inclusive ")
    ask_again = (not(num_levels >= 1 and num_levels <= 8))

for i in range(1, num_levels + 1):
    print((" " * (num_levels - i)) + ("#" * i) + "  " + ("#" * i))
