/*
 * Hasher.h
 *
 *  Created on: Nov 11, 2014
 *      Author: cawl-server
 */

#ifndef HASHER_H_
#define HASHER_H_

namespace Packets {

class Hasher {
public:
	Hasher();
	bool checkHash(int hash);
	int getHash(int key);
	virtual ~Hasher();
};

} /* namespace Packets */

#endif /* HASHER_H_ */
