from sys import argv, exit
from cs50 import SQL
import csv

def main():
    db = open_database()
    check_args()
    query(db)



def check_args():
    if len(argv) != 4:
        print("Usage: fiftyville.py fiftyville.db log2.sql results.txt")
        exit(1)
        
        
        
def open_database():
    open(f"fiftyville.db", "r").close()
    db = SQL("sqlite:///fiftyville.db")
    return db



def query(db):
    querying = True
    with open(argv[2], "w") as log:
        while(querying):
            query = input("Enter a query. Enter 'quit' to quit, 'schema' to view schema: ")
            if query == "quit":
                thief_id = querying = False
                continue
            if query == "schema":
                print(db.execute(".schema"))
                continue
            display_results(db, query)
            save = input("Would you like to save this query? Enter 'y' or 'n': ")
            if save == 'y':
                log.write(query)
            comment = input("Would you like to write a comment about this query? Enter 'y' or 'no'")
            if comment == 'y':
                log.write('-- ' + comment)



def display_results(db, query):
    with open(argv[3], "a") as file:
        results = db.execute(query)
        writer = csv.DictWriter(file, fieldnames = parse_query(query))
        for row in results:
            writer.write(row)
            
            
            
def parse_query(query):
    words = query.split(' ', ', ')
    for i in range(len(words)):
        if words[i] == 'SELECT':
            j = i + 1
        elif words[i] == 'FROM':
            k = i
            break
    return words[j:k]
    


main()