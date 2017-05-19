/******************************************************************************
 * Title          : TestNewInsert.cpp
 * Author         : Renat Khalikov
 * Created on     : April 27, 2017
 * Description    : main for part 3 of Assignment 4. Write a faster merge( )
 *                  function. This program will behave in the exact same way
 *                  as the one in Part1 ( TestInsertAndSearch )
 * Purpose        : to write a faster merge( ) function
 * Usage          : make newsearch5, make newsearch20, make newsearch100,
 *                  make newsearch1000, make newsearch10000
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
    if ( a_priority_queue.NewInsert(number_in_file ))
      number_of_insertions++;
  }
  input_queue_filename.close();
  cout << "Successfully inserted " << number_of_insertions << " elements into the queue.";
  cout << " The minimum element is " << a_priority_queue.findMin() << endl;
}

// Sample main for program TestNewInsert
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_search>" << endl;
    return 0;
  }
  
  const string queue_filename(argv[1]);
  const string search_filename(argv[2]);

  BinomialQueue<int> a_priority_queue;
  // Insert a few elements into the queue...
  InsertElementsIntoQueue(a_priority_queue, queue_filename);
  ifstream search_queue_filename(search_filename);
  if (search_queue_filename.fail()) {
    cerr << "Could not open <input_file_to_search_queue>\n";
    exit(1); // 1 indicates an error occurred
  }
  int number_in_file;
  while ( search_queue_filename >> number_in_file ) {
    // Search for an element.
    const int a_value = number_in_file; 
    if ( a_priority_queue.Find(a_value) )
      cout << a_value << " Found" << endl; 
    else 
      cout << a_value << " Not found" << endl; 
  }
  search_queue_filename.close();
  
  return 0;
}
