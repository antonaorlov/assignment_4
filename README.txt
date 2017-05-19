Assignment 4
----------------
All parts of the assignment are complete. Rehash factor R is 43. Part3 (faster insert function) made a slight modification to the merge( ) function (called OneElementMerge( )) by getting rid of case 0 because the tree we are inserting will always be non-empty, case 1 (the tree we are inserting will always be non-empty), case 6 (rhs only has one node), case 7 (rhs only has one node), and second for loop is replaced by simple assignment statement because rhs only has one node. Added if statement to break out of for loop if there are no trees left in rhs and the carry tree is nullptr.

----------------
Bugs
----------------
- The code compiles with warnings (comparisons between signed and unsigned), but it compiles and runs. 

- Nasty bug on linux machines, make delete100 causes corrupted linked list error or  segmentation fault. Somehow minimum node is lost after “240 deleted from queue.
Deletion successful - New minimum is 8”, the minimum supposed to stay 6. No errors running make delete1000, inserts and finds are ok. No errors on MacOs. 

- If the program doesn’t find item to delete (if it doesn’t exist), it will print “Deletion successful - New minimum is 1”. It shouldn’t do that. 

----------------
Compilation Instructions
----------------
Open a terminal.
Go to the directory containing the source code.
To compile type:
  make all

It is assumed that you are using a Linux machine and a g++ compiler.

----------------
To execute:
----------------
For Part 1:
In source code directory type:
./TestInsertAndSearch <input_file_to_create_queue> <input_file_to_check_search>

<input_file_to_create_queue> file containing ints. Ints will be inserted into mutable priority queue.

<input_file_to_check_search> file containing ints that you want to search in mutable priority queue.

Note:
In both files, each int must be on newline. Files must be in source directory. Read “The files” for more information.

Example: 

./TestInsertAndSearch 100_numbers.txt 100_test_search.txt

Alternatively you can run:
make search5 (for 100_test_search.txt)
make search20 (for 100_test_search.txt)
make search100 (for 100_test_search.txt)
make search1000 (for 1000_test_search.txt)
make search10000 (for 1000_test_search.txt)


For Part 2:
In source code directory type:

./TestInsertAndDelete <input_file_to_create_queue> <input_file_to_check_search>

<input_file_to_create_queue> file containing ints. Ints will be inserted into mutable priority queue.

<input_file_to_check_delete> file containing ints that you want to delete in mutable priority queue.

Note:
In both files, each int must be on newline. Files must be in source directory. Read “The files” for more information.

Example:

./TestInsertAndDelete 100_numbers.txt 1000_test_deletion.txt

Alternatively you can run:
make delete100 (for 1000_test_deletion.txt)
make delete1000 (for 1000_test_deletion.txt)
make delete10000 (for 10000_test_deletion.txt)


For Part 3:
In source code directory type:
./TestInsertAndSearch <input_file_to_create_queue> <input_file_to_check_search>

<input_file_to_create_queue> file containing ints. Ints will be inserted into mutable priority queue.

<input_file_to_check_search> file containing ints that you want to search in mutable priority queue.

Note:
In both files, each int must be on newline. Files must be in source directory. Read “The files” for more information.

Example: 

./TestNewInsert 100_numbers.txt 100_test_search.txt

Alternatively you can run:
make newsearch5 (for 100_test_search.txt)
make newsearch20 (for 100_test_search.txt)
make newsearch100 (for 100_test_search.txt)
make newsearch1000 (for 1000_test_search.txt)
make newsearch10000 (for 1000_test_search.txt)

---------------
The files
---------------
<x>_numbers.txt
contain sets of integers.
For example 20_numbers.txt contains 20 integers, one on each line.
You can use any of them as <input_file_to_create_queue>.
You can start with 5_numbers.txt, or 20_numbers.txt to debug your program.
The file 100_test_search.txt contains some numbers to use as <input_file_to_check_search>.
So you can run:
./TestInsertAndSearch 100_numbers.txt 100_test_search.txt

The file 100_test_delete.txt contains some number to use as <input_file_to_check_delete>.
So you can run
./TestInsertAndDelete 100_numbers.txt 100_test_delete.txt

You can also test your program with 1000_numbers.txt and 10000_numbers.txt:
./TestInsertAndSearch 1000_numbers.txt 1000_test_search.txt
./TestInsertAndDelete 1000_numbers.txt 1000_test_delete.txt
