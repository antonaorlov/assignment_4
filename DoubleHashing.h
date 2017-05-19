/*****************************************************************************
  Title          : DoubleHashing.h
  Author         : Renat Khalikov
  Created on     : April 06, 2017
  Description    : implementation of double hashing table for Assignment 4.
                   Table size is 101. Rehash factor is 43. 
  Purpose        : 
  Usage          :
  Build with     :
*/
#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>

namespace {
/**
 * Internal method to test if a positive number is prime.
 * @param {size_t} n is the number to test
 * @return {bool} true if prime
 */
bool IsPrimeDouble(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( size_t i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}

/**
 * Internal method to find next largest prime number
 * @param {size_t} n is a prime number
 * @return {int} next largest prime number
 */
static inline int NextPrimeDouble(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!IsPrimeDouble(n)) n += 2;  
  return n;
}

}  // namespace

/**
 * DoubleHashing table class
 * 
 * CONSTRUCTION: an approximate initial size or default of 101. 
 *               Rehash factor: 43.
 * 
 * ******************PUBLIC OPERATIONS*****************************************
 * bool insert( x )                        --> Insert x
 * bool remove( x )                        --> Remove x
 * bool contains( x )                      --> Return true if x is present
 * void makeEmpty( )                       --> Remove all items
 * void ReInsertPointer( x, p )            --> updates pointer
 * HashedPosition RemoveReturn( x )        --> removes x and returns pointer 
 * double getTotalNumberOfElements( )      --> Return total number of elements
 *                                             in hash table
 * double getTotalSizeOfTable( )           --> Return total size of the 
 *                                             hash table
 * void getTotalNumberOfCollisions ( n )   --> Assign n the total number
 *                                             of collisions
 * void resetCollisions (  )               --> Set total collisions to 0
 */
template <typename HashedObj, typename HashedPosition>
class HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(NextPrimeDouble(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    number_of_collisions_ = 0;
    for (auto &entry : array_) {
      entry.priority_queue_pointer_ = nullptr;
      entry.info_ = EMPTY;
    }
  }

  bool Insert(const HashedObj & x, const HashedPosition & p) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos)) {
      return false;
    }
    
    array_[current_pos].element_ = x;
    array_[current_pos].priority_queue_pointer_ = p;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(const HashedObj && x, const HashedPosition && p) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].priority_queue_pointer_ = std::move(p);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  /**
   * ReInsertPointer( ) updates the hash_table pointer. This function is called
   * by binomial queue Remove( ) function
   * 
   * @param {HashedObj} int that needs its pointer updated
   * @param {HashedPosition} pointer to node in binomial queue
   */
  void ReInsertPointer(const HashedObj & x, const HashedPosition &p) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    array_[current_pos].priority_queue_pointer_ = p;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  /**
   * RemoveReturn( ) removes x and returns its pointer
   * @param {HashedObj} item to be removed
   * @return {HashedPosition} pointer to the node in binomial queue
   */
  HashedPosition RemoveReturn( const HashedObj & x ) {
    size_t current_pos = FindPos(x);

    array_[current_pos].info_ = DELETED;
    return array_[current_pos].priority_queue_pointer_;
  }

  // getTotalNumberOfElements() returns the size of the hash table by
  // converting private member current_size_ of type size_t to type double
  double getTotalNumberOfElements( ) { return static_cast<double>( current_size_ ); }

  // getTotalSizeOfTable() assigns n to the size of the array
  double getTotalSizeOfTable( ) { return static_cast<double>( array_.size() ); }

  // getTotalNumberOfCollisions() returns the number of collitions that occured
  // during insert operation
  void getTotalNumberOfCollisions ( size_t & n ) { n = number_of_collisions_; }

  // resetCollisions() resets number of collisions to 0
  void resetCollisions (  ) { number_of_collisions_ = 0; }

 private:        
  struct HashEntry {
    HashedObj element_;
    HashedPosition priority_queue_pointer_;
    EntryType info_;
    
    HashEntry(const HashedObj & e = HashedObj{}, const HashedPosition & p = HashedPosition{}, EntryType i = EMPTY)
    :element_{e}, priority_queue_pointer_{p}, info_{i} { }
    
    HashEntry(HashedObj && e, HashedPosition && p, EntryType i = EMPTY)
    :element_{std::move(e)}, priority_queue_pointer_{std::move(p)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;
  // mutable value of number_of_collisions_ can change even in const functions
  mutable size_t number_of_collisions_; 

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  // FindPos() finds the next available position to insert into hash table by
  // calling DoubleHash() function
  size_t FindPos(const HashedObj & x) const {
    size_t current_pos = InternalHash(x);
    while (  array_[current_pos].info_ != EMPTY &&
             array_[current_pos].element_ != x  ) {
      current_pos += DoubleHash(x);
      if (current_pos >= array_.size()) {
        current_pos -= array_.size();
      }
      number_of_collisions_++;
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;
    // Create new double-sized, empty table.
    array_.resize(NextPrimeDouble(2 * old_array.size()));
    for (auto & entry : array_) {
      entry.priority_queue_pointer_ = nullptr;
      entry.info_ = EMPTY;
    }
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
        Insert(std::move(entry.element_), std::move(entry.priority_queue_pointer_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }

  // DoubleHash() used by the FindPos() function to calculate the next
  // available positon by applying double hashing formula with a rehash factor
  // of 43.
  size_t DoubleHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return 43 + (hf(x) % 43);
  }
};

#endif
