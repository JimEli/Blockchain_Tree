/*************************************************************************
* Title: Block Class
* File: block.cpp
* Author: James Eli
* Date: 9/21/2018
*
* Blockchain Block Class Definition.
*
* Notes:
*  (1) Blockchain information researched in "Mastering Blockchain",
*      2nd Edition, Imram Bashir.
*  (2) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0
*  (3) Compiled/tested with Eclipse Oxygen.3a Release (4.7.3a), using
*      CDT 9.4.3 and MinGw gcc-g++ (6.3.0-1).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-269.
*************************************************************************
* Change Log:
*   09/21/2018: Initial release. JME
*   10/07/2018: Added isBlockValid.  JME
*   11/01/2018: Changed isBlockValid stoi to stoul.  JME
*   11/09/2018: Added comparison operators. JME
*   11/09/2018: Moved DIFFICULTY declaration to main.cpp. JME
*   11/09/2018: Updated << operator per assignment directions. JME
*   11/09/2018: Changed mineBlock() loop. JME
*   11/09/2018: Replaced STL hash with user selectable versions.  JME
*   11/11/2018: Removed unused ctors.  JME
*************************************************************************/
#include "block.h"
#include "hash_funcs.h"

using namespace myBlock;

// All but hash ctor.
Block::Block(
	const unsigned long i, // id
	std::string ph,        // previous hash
	const unsigned long n  // nonce
	) : id(i), nonce(n), previousHash(ph)
{
	setTimeID(timeStamp());
	setHash(std::to_string(calcHash()));
}

// Additional block ctor.
Block::Block(unsigned long i)
{
	setTimeID(timeStamp());
	setID(i);
	setNonce(0);
	setPreviousHash("0");
	setHash(std::to_string(calcHash()));
}

// Get/set id.
unsigned long Block::getID() const { return id; }
void Block::setID(unsigned long i) { id = i; }

// Get/set nonce.
unsigned long Block::getNonce() const { return nonce; }
void Block::setNonce(unsigned long n) { this->nonce = n; }

// Get/set time stamp.
time_t Block::getTimeID() const { return timeId; }
void Block::setTimeID(time_t ts) { this->timeId = ts; }

// Get/set hash.
std::string Block::getHash() const { return hash; }
void Block::setHash(std::string h) { hash = h; }

// Get/set previous hash.
std::string Block::getPreviousHash() const { return previousHash; }
void Block::setPreviousHash(std::string ph) { previousHash = ph; }

// Calculate appropraite hash of "prevHash" + "nonce". See hash_funcs.h file for options.
inline uint32_t Block::calcHash() 
{
	// Use FNV1-a hash algorithm.
	Hash<> fnv1a; // Hash<stl_32> to use STL library hash.

	return fnv1a.hashString(previousHash + std::to_string(nonce)); 
}

// Block miner.
void Block::MineBlock(unsigned int difficulty)
{
	// Build string with difficulty number of leading zeros.
	std::string sDifficulty(difficulty, '0');

	uint32_t h;                // Temp holder of 32-bit hash value.
	std::stringstream hexHash; // Hash value formatted as hex string.

	// Loop (incrementing nonce) until hash meets difficulty level.
	for (; hexHash.str().substr(0, difficulty) != sDifficulty; nonce++)
	{
		// Hash "prevhash" + "nonce".
		h = calcHash();
		// Push hash through hex manipulator.
		hexHash.str("");
		hexHash << std::hex << std::setfill('0') << std::setw(8) << h;
	}

	// Save the hash as string.
	hash = std::to_string(h);

//#ifndef NDEBUG
	std::cout << ".";
//#endif
}

// Use current time as timestamp (milliseconds since Unix Epoch).
time_t Block::timeStamp() { return std::time(0); }

// Validate stored hash against calculated hash to prevent forgery.
bool Block::isHashValid() { return calcHash() == static_cast<unsigned long>(std::stoul(getHash())); }
