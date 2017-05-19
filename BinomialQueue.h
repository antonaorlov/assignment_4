/******************************************************************************
 * Title          : BinomialQueue.h
 * Author         : Renat Khalikov
 * Created on     : April 27, 2017
 * Description    : implementation of priority queue for Assignment 4.
 * Purpose        : 
 * Usage          : 
 * Build with     : 
 */
#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include "DoubleHashing.h"
#include <iostream>
#include <vector>
#include "dsexceptions.h"
using namespace std;

/**
 * Binomial queue class
 * 
 * CONSTRUCTION: with no parameters
 * ******************PUBLIC OPERATIONS*****************************************
 * bool insert( x )       --> Insert x
 * bool NewInsert( x )    --> Part 3: faster insert function
 * deleteMin( )           --> Return and remove smallest item
 * bool Remove( x )       --> Remove x from queue
 * Comparable findMin( )  --> Return smallest item
 * bool Find( x )         --> Return true if x is in the queue
 * bool isEmpty( )        --> Return true if empty; else false
 * void makeEmpty( )      --> Remove all items
 * void merge( rhs )      --> Absorb rhs into this heap
 * void OneElementMerge( rhs ) --> Efficient version of merge( )
 * ******************ERRORS****************************************************
 * Throws UnderflowException as warranted
 */
template <typename Comparable>
class BinomialQueue
{
 public:
  BinomialQueue( ) : theTrees( DEFAULT_TREES )
  {
    for( auto & root : theTrees )
      root = nullptr;
    currentSize = 0;
  }

  BinomialQueue( const Comparable & item ) : theTrees( 1 ), currentSize{ 1 }
    { theTrees[ 0 ] = new BinomialNode{ item, nullptr, nullptr, nullptr }; }

  BinomialQueue( const BinomialQueue & rhs )
    : theTrees( rhs.theTrees.size( ) ),currentSize{ rhs.currentSize }
  { 
    for( int i = 0; i < rhs.theTrees.size( ); ++i )
      theTrees[ i ] = clone( rhs.theTrees[ i ] );
  }

  BinomialQueue( BinomialQueue && rhs )
    : theTrees{ std::move( rhs.theTrees ) }, currentSize{ rhs.currentSize }
  { 
  }

  ~BinomialQueue( )
    { makeEmpty( ); }

  /**
   * Deep copy.
   */
  BinomialQueue & operator=( const BinomialQueue & rhs )
  {
    BinomialQueue copy = rhs;
    std::swap( *this, copy );
    return *this;
  }
      
  /**
   * Move.
   */
  BinomialQueue & operator=( BinomialQueue && rhs )
  {
    std::swap( currentSize, rhs.currentSize );
    std::swap( theTrees, rhs.theTrees );
      
    return *this;
  }
  
  /**
   * Return true if empty; false otherwise.
   */
  bool isEmpty( ) const
    { return currentSize == 0; }

  /**
   * Returns minimum item.
   * Throws UnderflowException if empty.
   */
  const Comparable & findMin( ) const
  {
    if( isEmpty( ) )
      throw UnderflowException{ };

    return theTrees[ findMinIndex( ) ]->element;
  }

  /**
   * @param {Comparable} x contains the item to be found
   * @return true if item exists in hash_table
   *
   * Find( ) calls hash_table, if hash_table contains the item, the item
   * exists in the priority queue
   */
  bool Find( const Comparable & x ) 
  {
    if ( isEmpty( ) )
      throw UnderflowException{ };
    else if (hash_table.Contains( x ))
      return true;
    return false;
  }

  /**
   * Insert item x into the priority queue; allows duplicates.
   */
  bool insert( const Comparable & x )
  { 
    BinomialQueue oneItem{ x };
    // theTrees[ 0 ] contains the node that contains x
    // store the pair <key,p> in your hash table
    if (hash_table.Insert( x, oneItem.theTrees[0] )) {
      merge( oneItem );
      return true;
    }
    return false;
  }

  /**
   * Insert item x into the priority queue; allows duplicates.
   */
  bool insert( Comparable && x )
  { 
    BinomialQueue oneItem{ std::move( x ) }; 
    if (hash_table.Insert( std::move(x), oneItem.theTrees[0] )) {
      merge( oneItem ); 
      return true;
    }
    return false;
  }

  /**
   * Used by TestNewInsert. Efficient version of merge( ).
   * Insert item x into the priority queue; allows duplicates.
   */
  bool NewInsert( const Comparable & x )
  { 
    BinomialQueue oneItem{ x };
    // theTrees[ 0 ] contains the node that contains x
    // store the pair <key,p> in your hash table
    if (hash_table.Insert( x, oneItem.theTrees[0] )) {
      OneElementMerge( oneItem );
      return true;
    }
    return false;
  }
  
  /**
   * Remove the smallest item from the priority queue.
   * Throws UnderflowException if empty.
   */
  void deleteMin( )
  {
    Comparable x;
    deleteMin( x );
  }

  /**
   * Remove the minimum item and place it in minItem.
   * Throws UnderflowException if empty.
   */
  void deleteMin( Comparable & minItem )
  {
    if( isEmpty( ) )
      throw UnderflowException{ };

    int minIndex = findMinIndex( );
    minItem = theTrees[ minIndex ]->element;

    BinomialNode *oldRoot = theTrees[ minIndex ];
    BinomialNode *deletedTree = oldRoot->leftChild;
    delete oldRoot;

    // Construct H''
    BinomialQueue deletedQueue;
    deletedQueue.theTrees.resize( minIndex );
    deletedQueue.currentSize = ( 1 << minIndex ) - 1;
    for( int j = minIndex - 1; j >= 0; --j )
    {
      deletedQueue.theTrees[ j ] = deletedTree;
      deletedTree = deletedTree->nextSibling;
      deletedQueue.theTrees[ j ]->nextSibling = nullptr;
    }

    // Construct H'
    theTrees[ minIndex ] = nullptr;
    currentSize -= deletedQueue.currentSize + 1;

    merge( deletedQueue );
  }

  /**
   * @param {Comparable} x contains the element to remove
   * @return {bool} true if deletion successful
   * 
   * Remove( ) makes it possible to remove any node in priority quene. Makes
   * x the smallest item in priority queue, calls deleteMin( ) to delete the
   * node. 
   */
  bool Remove( const Comparable & x )
  {
    if( isEmpty( ) )
      throw UnderflowException{ };

    if ( hash_table.Contains( x ) ) {
      // delete x from hash table and return pointer to the node
      BinomialNode* binomial_node_to_delete = hash_table.RemoveReturn( x );
      // element to delete is now the smallest element in the tree
      int minIndex = findMinIndex( );
      binomial_node_to_delete->element = theTrees[ minIndex ]->element - 1;
      // move element node to root
      while ( binomial_node_to_delete->parent_node != nullptr && 
              binomial_node_to_delete->element < binomial_node_to_delete->parent_node->element) {
        Comparable element_to_delete = binomial_node_to_delete->element;
        // update element
        binomial_node_to_delete->element = binomial_node_to_delete->parent_node->element;
        binomial_node_to_delete->parent_node->element = element_to_delete;
        // update hash_table 
        hash_table.ReInsertPointer( binomial_node_to_delete->element,
                                    binomial_node_to_delete );
        hash_table.ReInsertPointer( binomial_node_to_delete->parent_node->element,
                                    binomial_node_to_delete->parent_node );
        // update node
        binomial_node_to_delete = binomial_node_to_delete->parent_node;
        binomial_node_to_delete->parent_node = binomial_node_to_delete->parent_node;
      }
      // binomial_node_to_delete is now the smallest element
      deleteMin( );
      return true;
    }
    return false;
  }

  /**
   * Make the priority queue logically empty.
   */
  void makeEmpty( )
  {
    currentSize = 0;
    for( auto & root : theTrees )
      makeEmpty( root );
  }

  /**
   * Merge rhs into the priority queue.
   * rhs becomes empty. rhs must be different from this.
   * Exercise 6.35 needed to make this operation more efficient.
   */
  void merge( BinomialQueue & rhs )
  {
    if( this == &rhs )    // Avoid aliasing problems
      return;

    currentSize += rhs.currentSize;
    int capacity_size = capacity( );

    if( currentSize > capacity_size )
    {
      int oldNumTrees = theTrees.size( );
      int newNumTrees = max( theTrees.size( ), rhs.theTrees.size( ) ) + 1;
      theTrees.resize( newNumTrees );
      for( int i = oldNumTrees; i < newNumTrees; ++i )
          theTrees[ i ] = nullptr;
    }

    BinomialNode *carry = nullptr;
    for( int i = 0, j = 1; j <= currentSize; ++i, j *= 2 )
    {
      BinomialNode *t1 = theTrees[ i ];
      BinomialNode *t2 = i < rhs.theTrees.size( ) ? rhs.theTrees[ i ] : nullptr;

      int whichCase = t1 == nullptr ? 0 : 1;
      whichCase += t2 == nullptr ? 0 : 2;
      whichCase += carry == nullptr ? 0 : 4;

      switch( whichCase )
      {
        case 0: /* No trees */
        case 1: /* Only this */
          break;
        case 2: /* Only rhs */
          theTrees[ i ] = t2;
          rhs.theTrees[ i ] = nullptr;
          break;
        case 4: /* Only carry */
          theTrees[ i ] = carry;
          carry = nullptr;
          break;
        case 3: /* this and rhs */
          carry = combineTrees( t1, t2 );
          theTrees[ i ] = rhs.theTrees[ i ] = nullptr;
          break;
        case 5: /* this and carry */
          carry = combineTrees( t1, carry );
          theTrees[ i ] = nullptr;
          break;
        case 6: /* rhs and carry */
          carry = combineTrees( t2, carry );
          rhs.theTrees[ i ] = nullptr;
          break;
        case 7: /* All three */
          theTrees[ i ] = carry;
          carry = combineTrees( t1, t2 );
          rhs.theTrees[ i ] = nullptr;
          break;
      }
    }

    for( auto & root : rhs.theTrees ) {
      root = nullptr;
    }
    rhs.currentSize = 0;
  }

  /**
   * Efficient version of merge( )
   * Merge rhs into the priority queue.
   * rhs becomes empty. rhs must be different from this.
   */
  void OneElementMerge( BinomialQueue & rhs )
  {
    if( this == &rhs )    // Avoid aliasing problems
      return;

    // if the tree is empty
    currentSize += rhs.currentSize;
    int capacity_size = capacity( );

    if( currentSize > capacity_size )
    {
      int oldNumTrees = theTrees.size( );
      int newNumTrees = max( theTrees.size( ), rhs.theTrees.size( ) ) + 1;
      theTrees.resize( newNumTrees );
      for( int i = oldNumTrees; i < newNumTrees; ++i )
          theTrees[ i ] = nullptr;
    }
    
    BinomialNode *carry = nullptr;
    BinomialNode *t2 = rhs.theTrees[ 0 ];
    // rhs is now empty
    rhs.theTrees[ 0 ]  = nullptr;
    rhs.currentSize = 0;
    for( int i = 0, j = 1; j <= currentSize; ++i, j *= 2 )
    {
      BinomialNode *t1 = theTrees[ i ];
      int whichCase = t1 == nullptr ? 0 : 1;
      whichCase += t2 == nullptr ? 0 : 2;
      whichCase += carry == nullptr ? 0 : 4;

      switch( whichCase )
      {
        // tree is empty 
        // inserting rhs into empty tree
        case 2: /* Only rhs */
          theTrees[ i ] = t2;
          rhs.theTrees[ i ] = nullptr;
          t2 = nullptr;
          break;
        case 4: /* Only carry */
          theTrees[ i ] = carry;
          carry = nullptr;
          break;
        case 3: /* this and rhs */
          carry = combineTrees( t1, t2 );
          theTrees[ i ] = rhs.theTrees[ i ] = nullptr;
          t2 = nullptr;
          break;
        case 5: /* this and carry */
          carry = combineTrees( t1, carry );
          theTrees[ i ] = nullptr;
          t2 = nullptr;
          break;
      }
      if ( rhs.theTrees[ 0 ] == nullptr && carry == nullptr )
        break; // out of for loop
    }
  }

 private:
  struct BinomialNode
  {
    Comparable    element;
    BinomialNode *parent_node;
    BinomialNode *leftChild;
    BinomialNode *nextSibling;

    BinomialNode( const Comparable & e, BinomialNode * p, BinomialNode *lt, BinomialNode *rt )
      : element{ e }, parent_node{ p }, leftChild{ lt }, nextSibling{ rt } { }
    
    BinomialNode( Comparable && e, BinomialNode * p, BinomialNode *lt, BinomialNode *rt )
      : element{ std::move( e ) }, parent_node{ std::move( p ) }, leftChild{ lt }, nextSibling{ rt } { }
  };

  const static int DEFAULT_TREES = 1;

  // double probing hash_table
  HashTableDouble<Comparable, BinomialNode*> hash_table;

  vector<BinomialNode *> theTrees;  // An array of tree roots
  int currentSize;                  // Number of items in the priority queue
  
  /**
   * Find index of tree containing the smallest item in the priority queue.
   * The priority queue must not be empty.
   * Return the index of tree containing the smallest item.
   */
  int findMinIndex( ) const
  {
    int i;
    int minIndex;

    for( i = 0; theTrees[ i ] == nullptr; ++i )
      ;

    for( minIndex = i; i < theTrees.size( ); ++i )
      if( theTrees[ i ] != nullptr &&
          theTrees[ i ]->element < theTrees[ minIndex ]->element )
        minIndex = i;

    return minIndex;
  }

  int FindIndex( BinomialNode* node ) const
  {
    int i;
    int index;

    for( i = 0; theTrees[ i ] == nullptr; ++i )
      ;

    for( index = i; i < theTrees.size( ); ++i )
      if( theTrees[ i ] != nullptr  )
        index = i;

    return index;
  }

  /**
   * Return the capacity.
   */
  int capacity( ) const
  { 
    int size = ( 1 << theTrees.size( ) ) - 1;
    return size;
  }

  /**
   * Return the result of merging equal-sized t1 and t2.
   */
  BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 )
  {
    if( t2->element < t1->element )
      return combineTrees( t2, t1 );
    t2->nextSibling = t1->leftChild;
    t1->leftChild = t2;

    // t1 is the root of the tree, no parent_node
    // t1 becomes the parent of t2
    t2->parent_node = t1;
    t1->parent_node = nullptr;
    return t1;
  }

  /**
   * Make a binomial tree logically empty, and free memory.
   */
  void makeEmpty( BinomialNode * & t )
  {
    if( t != nullptr )
    {
      makeEmpty( t->leftChild );
      makeEmpty( t->nextSibling );
      delete t;
      t = nullptr;
    }
  }

  /**
   * Internal method to clone subtree.
   */
  BinomialNode * clone( BinomialNode * t ) const
  {
    if( t == nullptr )
      return nullptr;
    else
      return new BinomialNode{ t->element, 
                               clone( t-> parent_node), 
                               clone( t->leftChild ), 
                               clone( t->nextSibling ) };
  }
};

#endif
