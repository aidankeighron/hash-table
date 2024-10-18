import hashTable, time
from hash_table import HashTable
from itertools import permutations

start = time.time_ns()

for i, p in enumerate(permutations('abcdefghi')):
    hashTable.set(''.join(p), i+1)
    # hashTable.print(False)

# hashTable.print(False)
print((time.time_ns()-start)/1_000_000, i)

start = time.time_ns()
table = {}
for i, p in enumerate(permutations('abcdefghi')):
    table[''.join(p)] = i+1
    # print(table)

# print(table)
print((time.time_ns()-start)/1_000_000, i)