import csv
from sys import argv, exit
from cs50 import SQL

# main program definition

def main():
    check_args()
    open(f"students.db", "r").close()
    db = SQL("sqlite:///students.db")
    query_house(db, argv[1])

# checks for correct number of command line arguments

def check_args():
    if len(argv) != 2:
        print("Usage: import.py students.db")
        exit(1)

# queries for all students that live in the house specified by the command line argument. prints results

def query_house(db, house):
    housemates = db.execute('SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first;', house)
    for resident in housemates:
        if resident['middle'] == None:
            print(f"{resident['first']} {resident['last']}, born {resident['birth']}")
        else:
            print(f"{resident['first']} {resident['middle']} {resident['last']}, born {resident['birth']}")
            
#executes main program
            
main()