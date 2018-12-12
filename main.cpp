/*********************************************************************************
* Title: CIS-269 Assignment 5 - Blockchain Tree
* File: main.cpp
* Author: James Eli
* Date: 11/08/2018
* Due Date: Nov 29, 2018 11:59 PM
*
* Blockchain Class Driver Program. Tests our blockchain.
*
* Part 1: Make a simple blockchain (30 pts)
* Using your block.h code, generate a blockchain of 100 elements stored in an
* array. Set the genesis block in the 0 element of your array with the
* previous hash, nonce, and id to 0, and the current hash value of whatever
* the previous hash value concatenated with the nonce, and the timestamp
* should be the current time. After you set the genesis block, use a for-loop
* to build the next 99 blocks, where the nonce is a randomly generated number
* and the previous hash is the current hash from the previous block in the
* array. Your block id should be the same as the array position.
*
* Part 2: Add a mining method to your block class (30 pts)
* For this section, add a mining method that takes a difficulty integer parameter
* and returns the nonce that solves the difficulty of the problem. Now use your
* mining method to calculate the nonce instead of a random number. However, when
* you turn in the code, don’t have the difficult set higher than 3.
*
* Part 3: Put the array in a binary tree (20 pts)
* Using either an array or linked-list structure, skipping the genesis block put
* your blockchain into a binary tree where the branching function is based on the
* nonce.
*
* Part 4: Print out the elements using traversal algorithms (20 pts)
* Print out the order of the elements in the format of id:nonce:hash using a
* depth-first traversal algorithm (doesn’t matter which one), then a breadth-first
* traversal algorithm.
*
* Submit your .cpp and .h source files.
*
* Notes:
*  (1) Could not achieve consistent results when using the STL x64 hash
*      function. The STL hash returns std:size_t (32-bits on x86, and
*      64-bits on x64). The STL hash exihibtted sluggish performance and
*      suspect nonce values). So, the STL library hash function and 2
*      alternative functions were researched and provided. See comments
*      inside the hash_funcs.h file for further information.
*  (2) Using nonce as key value for tree is problematic because it is possible
*      to have duplicate nonce values (especially at lower levels of difficulty).
*      Program checks for duplicate nonce values and does not insert these blocks
*      into the tree.
*  (3) Uses my version of queue and vector.
*  (4) Bonus section gives basic tree statistics and attempts to balance tree.
*      Include by defining BALANCE_TREE macro.
*  (5) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0 (32 & 64-bit).
*  (6) Compiled/tested with Eclipse Oxygen.3a Release (4.7.3a), using
*      CDT 9.4.3 and MinGw32 gcc-g++ (6.3.0-1).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-269.
*********************************************************************************
* Change Log:
*   11/09/2018: Initial release. JME
*********************************************************************************/

#include <iostream>  // cout
#include <memory>    // smart pointer
#include <string>    // strings
#include <ctime>     // time
#include <random>    // random

// Uncomment to include tree balancing code.
//#define BALANCE_TREE

#include "block.h"   // myBlock
#include "tree.h"    // myTree

using namespace myBlock;
using namespace myTree;

// Demo tree size.
constexpr std::size_t TREE_SIZE{ 100 };
// Maximum random number (0 - MAX_RANDOM).
constexpr unsigned long MAX_RANDOM = 1000;
// Difficulty level for mining blocks.
constexpr unsigned int DIFFICULTY = 2;

int main()
{
	// Random number distribution [0, MAX_RANDOM] [inclusive, inclusive].
	std::uniform_int_distribution<unsigned long> dist(0, MAX_RANDOM);
	std::random_device rd;
	// Non-deterministic 32-bit seed.
	std::mt19937 mt(rd());

	// Catch exceptions.
	try
	{
		//
		// Part 1: Make a simple blockchain.
		//
		// Array of blocks: Block bArray[TREE_SIZE];
		std::unique_ptr<Block[]> bArray = std::make_unique<Block[]>(TREE_SIZE);

		// Fill array with 100 blocks. Genesis block has previous hash and nonce = 0. 
		// Subsequent blocks generate a hash using the previous hash + a random nonce.
		for (unsigned long i = 0; i < TREE_SIZE; i++)
			bArray[i] = ( i ? Block(i, bArray[i - 1].getHash(), dist(mt)) : Block(i, "0", 0) );

		// Print select array blocks.
		std::cout << "3 example blocks from the array:\n";
		std::cout << *bArray.get();
		std::cout << *(bArray.get() + TREE_SIZE / 2);
		std::cout << *(bArray.get() + TREE_SIZE - 1);

		//
		// Part 2: Add a mining method to your block class.
		// Part 3: Put the array in a binary tree.
		//
		std::cout << "Mining " << TREE_SIZE << " blocks at difficulty level: " 
				  << DIFFICULTY << std::endl;

		// Instantiate a binary tree of blocks.
		Tree<Block> bTree;

		// String saves previous hash. Init with "0" as genesis previous hash.
		std::string hash("0");

		// Mine blocks and fill a tree with (upto) 99 blocks.
		for (unsigned long i = 0; i < TREE_SIZE; i++)
		{
			// Mine a new block.
			Block newBlock(i, hash, 0);
			newBlock.MineBlock(DIFFICULTY);

			// Save hash to use as previousHash value in next block in chain.
			hash = newBlock.getHash();
			
			// Ignoring genesis block, check nonce doesn't already exist and add to tree.
			if (i && !bTree.find(newBlock))
				bTree.add(newBlock);
		}

		//
		// Part 4: Print out the elements using traversal algorithms.
		//
		std::cout << "\nDFS in-order traversal:\n";
		bTree.inOrder();
		std::cout << "BFS traversal:\n";
		bTree.bfs();

#ifdef BALANCE_TREE
		// Bonus section gives tree stats and balance.
		std::cout << "Tree stats:\n Tree size = " << bTree.size() << std::endl;
		std::cout << " Tree Height = " << bTree.getHeight() << std::endl;
		if (!bTree.isBalanced())
		{
			std::cout << " Tree is not balanced.\n";
			bTree.balance();
			std::cout << "After balance attempt:\n Tree Height = " << bTree.getHeight() << std::endl;
			std::cout << " Tree " << (bTree.isBalanced() ? "is " : "could not be") << " balanced!\n";
		} 
		else
			std::cout << " Tree is balanced.\n";
#endif // End BALANCE_TREE

		// Report duplicate nonce(s).
		if (bTree.size() != TREE_SIZE - 1)
			std::cout << (TREE_SIZE - 1) - bTree.size() << " duplicate nonce value(s) not inserted into tree.\n";

	}
	catch (std::exception& e)
	{
		// Report exception and exit with failure code.
		std::cout << "Encountered exception: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}
