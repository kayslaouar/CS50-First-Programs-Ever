import cs50

def main():
    text = input("Text: ")
    counts = parse_text(text)
    letters = counts[0]
    words = counts[1]
    sentences = counts[2]
    grade = calculate_grade_level(letters, words, sentences)
    determine_grade_level(grade)

# counts the number of letters, words, and sentences in the input text
# returns output_list which is a list of 3 buckets that stores the number of
# letters, words, and sentences in the input text in indices 0, 1, and 2 respectively

def parse_text(text):
    punctuation = ['.', '!', '?']
    output = [0, 1, 0]
    for char in text:
        if (char >= 'A' and char <= 'Z') or (char >= 'a' and char <= 'z'):
            output[0] += 1
        elif char == ' ':
            output[1] += 1
        elif char in punctuation:
            output[2] += 1
    return output

# outputs the grade level of a text given grade

def determine_grade_level(grade):
    if grade <= 1:
        print("Before Grade 1")
    if (grade > 1 and grade < 16):
        print("Grade ", grade)
    elif (grade >= 16):
        print("Grade 16+")

# calculates the grade level of an input text given the number of letters and sentences present in every 100 words of the text.
# algorithm is given by the Coleman-Liau formula

def calculate_grade_level(letters, words, sentences):
    L = letters * 100 / words
    S = sentences * 100 / words
    index = (0.0588 * L) - (0.296 * S) - 15.8
    return int(round(index))

# execute main

main()