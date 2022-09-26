from sys import argv, exit
import math
import csv



def main():
    check_args()
    table = []
    sequence = ""
    STR_list = []
    with open(argv[1], "r") as file:
        database = csv.reader(file)
        table = list(database)
        header = table.pop(0)
        header.pop(0)
        STR_list = header
    with open(argv[2], "r") as file:
        sequence = file.read()
    STR_counts = count_STR_frequencies(sequence, STR_list)
    most_similar = find_most_similar(STR_list, STR_counts, table)
    print(most_similar)

# ensures the input has the correct number of arguments

def check_args():
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

# determines the number of times an STR repeats in the longest STR sequence
# iterates through all STRs in STR_list

def count_STR_frequencies(sequence, STR_list):
    STR_counts = []
    for i in range(len(STR_list)):
        STR_counts.append(0)
    i = 0
    for STR in STR_list:
        STR_counts[i] = find_longest_STR_seq(sequence, STR)
        i += 1
    return STR_counts

# finds the longest repeating STR sequence in a given DNA sequence

def find_longest_STR_seq(sequence, STR):
    max = 0
    cur = 0
    n = len(STR)
    i = 0
    while i < (len(sequence) - n):
        snippet = sequence[i:i + n]
        if (snippet == STR):
            cur = count_consecutive_snippets(sequence, STR, i, n)
            i = i + (cur * n)
        if (cur > max):
            max = cur
        i += 1
    return max

# when find_longest_STR_seq detects the a matching STR, this function counts the number of times that STR repeats consecutively

def count_consecutive_snippets(sequence, STR, start, n):
    repeats = 0
    for i in range(start, len(sequence), n):
        snippet = extract_snippet(sequence, i, n)
        if (snippet == STR):
            repeats += 1
        else:
            break
    return repeats

# extracts an n-sized substring dna "snippet" from dna sequence starting at index i

def extract_snippet(sequence, i, n):
    if (i + n) > len(sequence):
        return sequence[i:len(sequence)]
    else:
        return sequence[i:i + n]

# goes through STR_counts to find individual from database with the most similar STR
# returns "No Match" if similarity != 0 which means that nobody in the database matched the counts

def find_most_similar(STR_list, STR_counts, table):
    most_similar = ""
    similarity = float('inf')
    difference = 0
    for sample in table:
        for i in range(1, len(sample)):
            difference += abs(STR_counts[i - 1] - int(sample[i]))
        if difference < similarity:
            most_similar = sample[0]
            similarity = difference
        difference = 0
    if (similarity != 0):
        most_similar = "No Match"
    return most_similar


# execute main program

main()