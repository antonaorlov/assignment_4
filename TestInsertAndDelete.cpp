/******************************************************************************
 * Title          : TestInsertAndDelete.cpp
 * Author         : Renat Khalikov
 * Created on     : April 27, 2017
 * Description    : main for part 2 of Assignment 4. Be able to remove a
 *                  specific key (not only the minimum key) from priority queue
 * Purpose        : to test remove functioality
 * Usage          : make delete100 or make delete1000
 * Build with     : make all
 */
#include "BinomialQueue.h"
#include "DoubleHashing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // exit(1) call
using namespace std;

/**
 * InsertElementsIntoQueue( ) opens the file and inserts elements into
 * a_priority_queue
 * 
 * @param {PriorityQueueType} a_priority_queue is priority queue object
 * @param {string} queue_filename input filename containing ints to insert.
 */
template <typename PriorityQueueType>
void InsertElementsIntoQueue(PriorityQueueType &a_priority_queue, const string &queue_filename) {
  ifstream input_queue_filename(queue_filename);
  if (input_queue_filename.fail()) {
    cerr << "Could not open <input_file_to_create_queue>\n";
    exit(1); // 1 indicates an error occurred
  }
  int number_in_file;
  int number_of_insertions = 0;
  while ( input_queue_filename >> number_in_file ) {
    if (a_priority_queue.insert(number_in_file))
      number_of_insertions++;
  }
  input_queue_filename.close();
  cout << "Successfully inserted " << number_of_insertions << " elements into the queue.";
  cout << " The minimum element is " << a_priority_queue.findMin() << endl;
}

// Sample main for program TestInsertAndSearch
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_search>" << endl;
    return 0;
  }
  
  const string queue_filename(argv[1]);
  const string delete_filename(argv[2]);

  BinomialQueue<int> a_priority_queue;
  // Insert a few elements into the queue...
  InsertElementsIntoQueue(a_priority_queue, queue_filename);
  ifstream delete_queue_filename(delete_filename);
  if (delete_queue_filename.fail()) {
    cerr << "Could not open <input_file_to_search_queue>\n";
    exit(1); // 1 indicates an error occurred
  }
  int number_in_file;
  while ( delete_queue_filename >> number_in_file ) {
    // Delete an element.
    const int b_value = number_in_file; 
    if (a_priority_queue.Remove(b_value)) {
      // This code deletes all b_value items.
      // Remove() returns true if item deleted and false otherwise 
      // (i.e. item not in the queue).
      cout << b_value << " deleted from queue." << endl;
    }
    // Check if deletion was successful. 
    if (a_priority_queue.Find(b_value)) {
      cout << "Serious problem with deletion routine... Need to debug...";
    } 
    else { 
      cout << "Deletion successful";
    }
    cout << " - New minimum is " << a_priority_queue.findMin() << endl;
  }
  delete_queue_filename.close();
  
  return 0;
}
