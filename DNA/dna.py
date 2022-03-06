import sys
import csv
from csv import reader

# checking for command line arguments

if len(sys.argv) != 3:
    print("Error. Usage: dna.py databes.csv sequences.txt")
    sys.exit()

# loading a database into the memory, defineng STR's names

with open(sys.argv[1], "r") as csv_file:
    csv_reader = reader(csv_file)
    database = list(csv_reader)  # saving database into list of lists

# loading the sequence into the memory

seq_file = open(sys.argv[2], "r")
seq = seq_file.read()  # this is the sequence

# parsing through the sequence and counting maximum repeats for each str

result = []

for i in range(len(database[0]) - 1):  # repeat for each column. 
    start_ind = 0
    max_counter = 0
    pr_index = -10
    tmp_counter = 0
    
    while seq.find(database[0][i+1], start_ind) != -1:
        
        if seq.find(database[0][i+1], start_ind) == pr_index + len(database[0][i+1]):
            tmp_counter += 1
            pr_index = seq.find(database[0][i+1], start_ind)
            start_ind = seq.find(database[0][i+1], start_ind) + len(database[0][i+1])
        else:
            max_counter = max(tmp_counter, max_counter)
            pr_index = seq.find(database[0][i+1], start_ind)
            tmp_counter = 1
            start_ind = seq.find(database[0][i+1], start_ind) + len(database[0][i+1])
    
    max_counter = max(max_counter, tmp_counter)
    result.append(max_counter)
    

# comparing results with the database for a match

tmp = 0
found = False

for row in range(len(database) - 1):  # scrolling through database rows
    for col in range(len(database[0]) - 1):  # scrolling through database columns
        if int(database[row+1][col+1]) == result[col]:
            tmp += 1
        else:
            tmp = 0
            break
    if tmp == len(result):
        found = True
        name = row + 1
        break

if found == True:
    print(database[name][0])
else:
    print("No match")
