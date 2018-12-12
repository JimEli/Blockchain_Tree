/*************************************************************************
* Title: Block class
* File: block.h
* Author: James Eli
* Date: 9/21/2018
*
* Blockchain Block Class Declaration.
*
* Declares a block class containing an ID, Time Stamp, Hash, Previous Hash
* and Nonce members. All members are private. Includes accessors (get/set)
* for all members. Provides appropriate ctors for genesis block creation
* and follow-on chain blocks. Includes a formatted print function.
*
* Notes:
*  (1) Could not achieve consistent results when using the STL x64 hash 
*      function. The STL hash returns std:size_t (32-bits on x86, and 
*      64-bits on x64). The STL hash exihibtted sluggish performance and
*      suspect nonce values). So, the STL library hash function and 2 
*      alternative functions were researched and provided. See comments 
*      inside the hash_funcs.h file for further information.
*  (2) Blockchain information researched in "Mastering Blockchain",
*      2nd Edition, Imram Bashir.
*  (3) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0
*  (4) Compiled/tested with Eclipse Oxygen.3a Release (4.7.3a), using
*      CDT 9.4.3 and MinGw gcc-g++ (6.3.0-1).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-269.
*************************************************************************
* Change Log:
*   09/21/2018: Initial release. JME
*   11/09/2018: Added comparison operators. JME
*   11/09/2018: Moved DIFFICULTY declaration to main.cpp. JME
*   11/09/2018: Updated << operator per assignment directions. JME
*   11/09/2018: Changed debug print inside mineBlock(). JME
*   11/09/2018: Replaced STL hash with user selectable versions.  JME
*   11/09/2018: Cleaned up unused ctors/parameters.  JME
*************************************************************************/
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <cstdint>    // uint32_t
#include <iostream>   // cout
#include <iomanip>    // hex conversion manipulators
#include <sstream>    // string conversion
#include <string>     // c++ strings
#include <ctime>      // time()

namespace myBlock {

	class Block
	{
	public:
		// Default ctor.
		Block() = default;
		// Empty block ctor (id).
		explicit Block(const unsigned long);
		// All members except hash ctor.
		Block(const unsigned long, /* id */
			std::string,           /* previous hash */
			const unsigned long    /* nonce */
		);

		// Dtor.
		~Block() = default;

		// Accessor functions for id number.
		void setID(const unsigned long);
		unsigned long getID() const;

		// Accessor functions for hash.
		void setHash(std::string);
		std::string getHash() const;

		// Accessor functions for previous hash.
		void setPreviousHash(std::string);
		std::string getPreviousHash() const;

		// Accessor functions for nonce.
		void setNonce(unsigned long);
		unsigned long getNonce() const;

		// Accessor functions for time stamp.
		void setTimeID(time_t);
		time_t getTimeID() const;

		// Mine blocks.
		void MineBlock(unsigned int);

		// Validate stored hash against calculated hash to prevent forgery.
		bool isHashValid();

		// Print formatted block data.
		friend std::ostream& operator<< (std::ostream& os, const Block& b)
		{
			std::stringstream ss;

			ss << std::setfill('0') << std::setw(sizeof(unsigned long) * 2) << std::hex << b.getHash();
			return os << std::setfill(' ') << std::setw(2) << b.getID()
				<< ":0x" << ss.str() << ":" << b.getNonce() << std::endl;
		}

		// Less than operator, only based upon comparison of block nonce!
		bool operator< (Block rhs) { return (this->nonce < rhs.nonce); }
		// Equality operator, only based upon comparison of block nonce!
		bool operator== (Block rhs) { return (this->nonce == rhs.nonce); }

	private:
		// Private member data.
		unsigned long id;         // Block identification number (id).
		unsigned long nonce;      // Nonce, used in computing hash.
		time_t timeId;            // Timestamp.
		std::string hash;         // Hash of current block.
		std::string previousHash; // Hash of previous block.

		// Hash calculation.
		inline uint32_t calcHash();

		// Sets time stamp to now (seconds past Unix epoch).
		static time_t timeStamp();
	};
}
#endif
