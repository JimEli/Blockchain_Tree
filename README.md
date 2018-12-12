# CIS269_Assignment-5
Constructs simple blockchain and fills a binary tree.

Part 1: Generate a blockchain of 100 elements stored in an array.
Part 2: Add a mining method that takes a difficulty integer parameter and returns the nonce that solves the difficulty of the problem. 
Part 3: Put the array in a binary tree.
Part 4: Print out the order of the elements in the format of id:nonce:hash using a depth-first traversal algorithm (doesn’t matter which one), then a breadth-first traversal algorithm.

Notes:
* Could not achieve consistent results when using the STL x64 hash function. The STL hash returns std:size_t (32-bits on x86, and 64-bits on x64). The STL hash exihibtted sluggish performance and suspect nonce values). So, the STL library hash function and 2  alternative functions were researched and provided. See comments inside the hash_funcs.h file for further information.
* Using nonce as key value for tree is problematic because it is possible to have duplicate nonce values (especially at lower levels of difficulty). Program checks for duplicate nonce values and does not insert these blocks into the tree.
* Uses my version of queue and vector.
* Bonus feature gives basic tree statistics and attempts to balance tree. Include by defining BALANCE_TREE macro.
* Compiled/tested with MS Visual Studio 2017 Community (v141), and Windows SDK version 10.0.17134.0 (32 & 64-bit).
* Compiled/tested with Eclipse Oxygen.3a Release (4.7.3a), using CDT 9.4.3 and MinGw32 gcc-g++ (6.3.0-1).
