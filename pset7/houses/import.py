'''In import.py, write a program that imports data from a CSV spreadsheet.
Your program should accept the name of a CSV file as a command-line argument.
If the incorrect number of command-line arguments are provided, your program should print an error and exit.
You may assume that the CSV file will exist, and will have columns name, house, and birth.'''



import csv
from sys import argv, exit
from cs50 import SQL

# main program definition. opens csv file for read and inserts student information into database

def main():
    check_args()
    db = open_database()
    with open(argv[1], "r") as students:
        reader = csv.DictReader(students, delimiter = ',')
        for row in reader:
            insert_student(db, row)

# checks for correct number of command line arguments

def check_args():
    if len(argv) != 2:
        print("Usage: import.py characters.csv")
        exit(1)

# establishes connection with database, opens database for write, creates a new database object, and creates a new table in database

def open_database():
    open(f"students.db", "w").close()
    db = SQL("sqlite:///students.db")
    db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")
    return db

# inserts student information from csv file to database

def insert_student(db, row):
    names = row["name"].split(' ')
    if (has_middle_name(names)):
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?);", names[0], names[1], names[2], row["house"], row["birth"])
    else:
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?);", names[0], None, names[1], row["house"], row["birth"])

# checks whether a student has a middle name

def has_middle_name(names):
    if (len(names) == 3):
        return True
    else:
        return False

# executes main program

main()