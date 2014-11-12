/*
 * Hasher.h
 *
 *  Created on: Nov 11, 2014
 *      Author: Robin Bond
 *      If needed this will be implemented more.
 *      main usage was thought to hash contents before sending and also send the resulting hash
 *      together with the data. On the receiving end the data could be hashed again and be compared
 *      with the received hash.
 */

#ifndef HASHER_H_
#define HASHER_H_

#define KEY ultrasuperpotatismos

#include <functional>

namespace Packets {

class Hasher {
public:
	Hasher();
	bool checkHash(std::hash<int> hash, int data);
	int getHash();
	virtual ~Hasher();
};

} /* namespace Packets */

#endif /* HASHER_H_ */
