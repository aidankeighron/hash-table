import hashTable


hashTable.print(True)
hashTable.set("test", 2)
print(hashTable.get("test"))
print(hashTable.get("not"))
hashTable.print(False)
hashTable.delete("test")
print(hashTable.get("test"))
hashTable.print(False)
