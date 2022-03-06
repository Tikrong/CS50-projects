import sys

seq_file = open(f"sequences/{sys.argv[1]}")
seq = seq_file.read()  # here the sequence is stored

print(f"We are looking for {sys.argv[2]}")

start_ind = 0
max_counter = 0
pr_index = -10
tmp_counter = 0


while seq.find(sys.argv[2], start_ind) != -1:
    if seq.find(sys.argv[2], start_ind) == pr_index + len(sys.argv[2]):
        tmp_counter += 1
        pr_index = seq.find(sys.argv[2], start_ind)
        start_ind = seq.find(sys.argv[2], start_ind) + len(sys.argv[2])
    else:
        max_counter = max(tmp_counter, max_counter)
        pr_index = seq.find(sys.argv[2], start_ind)
        tmp_counter = 1
        start_ind = seq.find(sys.argv[2], start_ind) + len(sys.argv[2])
        
max_counter = max(tmp_counter, max_counter)    
    
print(max_counter)
