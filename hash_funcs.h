/*************************************************************************
* Title: Hash Functions.
* File: hash_funcs.h
* Author: James Eli
* Date: 11/12/2018
*
* Collection of 3 public domain 32-bit hash functions:
*   C++ STL Hash function.
*   FNV-1a 32-bit hash function.
*   CRC32 32-bit cyclic redundancy check.
*   SDBM 32-bit function.
*
* Notes:
*  (1) Could not achieve consistent results when using the STL x64 hash
*      function. The STL hash returns std:size_t (32-bits on x86, and
*      64-bits on x64). The STL hash exihibtted sluggish performance and
*      suspect nonce values). So, the STL library hash function and 2
*      alternative functions are provided. Additional information can be 
*      found below.
*  (2) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0
*  (3) Compiled/tested with Eclipse Oxygen.3a Release (4.7.3a), using
*      CDT 9.4.3 and MinGw gcc-g++ (6.3.0-1).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-269.
*************************************************************************
* Change Log:
*   11/12/2018: Initial release. JME
*************************************************************************/
#ifndef _HASH_FUNCTIONS_H_
#define _HASH_FUNCTIONS_H_

#include <cstdint>    // uints
#include <functional> // STL hash
#include <string>     // strings

// Hash function pointer typedef.
typedef uint32_t(*HashFunc)(std::string);

// Function prototypes.
uint32_t stl_32(std::string);
uint32_t fnv1a_32(std::string);
uint32_t crc_32(std::string);
uint32_t sdbm_32(std::string);

// Hash class with FNV1a algorithm as default function.
template <HashFunc hf = fnv1a_32>
struct Hash { uint32_t hashString(std::string s) { return hf(s); } };

/*************************************************************************
 * C++ STL <functional> library hash function.
*************************************************************************/
uint32_t stl_32(std::string key)
{
	std::hash<std::string> sHash;

	// Returns size_t which is 64-bit under x64.
	return static_cast<uint32_t>(sHash(key));
}

/*************************************************************************
 * Fowler–Noll–Vo is a non-cryptographic hash function created by Glenn Fowler,
 * Landon Curt Noll, and Kiem-Phong Vo. The FNV hash was designed for fast hash
 * table and checksum use, not cryptography. The core of the FNV-1a hash
 * algorithm is as follows:
 *
 *  hash = offset_basis
 *    for each byte_of_data to be hashed
 *    hash = hash XOR octet_of_data
 *    hash = hash * FNV_prime
 *  return hash
 *
 * The FNV-1a hash parameters are as follows:
 *  - hash is an n bit unsigned integer, where n is the bit length of hash.
 *  - The multiplication is performed modulo 2n where n is the bit length of hash.
 *  - The XOR is performed on the low order octet (8 bits) of hash.
 *  - The FNV_prime is dependent on n, the size of the hash:
 *	 32-bit FNV_prime = 224 + 28 + 0x93 = 16777619 (0x1000193)
 *
 * Part of the magic of FNV is the selection of the FNV_prime for a given sized
 * unsigned integer. Some primes do hash better than other primes for a given integer
 * size. The offset_basis for FNV-1a is dependent on n, the size of the hash:
 *  - 32-bit offset_basis = 2166136261 (0x811C9DC5)
 * 
 * Information researched here: http://www.isthe.com/chongo/tech/comp/fnv/
 *************************************************************************/
uint32_t fnv1a_32(std::string key)
{
	uint32_t hash = 0x811c9dc5;
	uint32_t prime = 0x1000193;

	for (unsigned int i = 0; i < key.length(); ++i)
	{
		uint8_t value = key[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

/*************************************************************************
 * CRC-32 implementation researched in the paper, "Reversing CRC–Theory and 
 * Practice", Stigge, Plotz, etal. May 2006. Copy located here: 
 * http://stigge.org/martin/pub/SAR-PR-2006-05.pdf
 * Additional information, including table generation found in the "Painless 
 * Guide to CRC Error Detection Algorithms", R. Williams, August 1993. Copy 
 * located here: 
 * http://chrisballance.com/wp-content/uploads/2015/10/CRC-Primer.html
 *************************************************************************/
uint32_t crc_32(std::string key)
{
	uint32_t crc = 0xffffffff, i=0;
	std::size_t len = key.length();

	while (len--)
	{
		uint32_t val = (crc ^ key[i++]) & 0xff;

		for (int i = 0; i < 8; i++)
			val = val & 1 ? (val >> 1) ^ 0xedb88320 : val >> 1;
		
		crc = val ^ crc >> 8;
	}

	return crc ^ 0xffffffff;
}

/*************************************************************************
 * This is the algorithm used in the open source SDBM project. The hash 
 * function seems to have a good over-all distribution for many different 
 * data sets. Algorithm adapted from Hash Functions, York University, 
 * located here: http://www.cse.yorku.ca/~oz/hash.html
 *************************************************************************/
uint32_t sdbm_32(std::string key)
{
	uint32_t hash = 0;

	for (std::size_t i = 0; i < key.length(); i++)
		hash = (key[i]) + (hash << 6) + (hash << 16) - hash;

	return hash;
}

#endif