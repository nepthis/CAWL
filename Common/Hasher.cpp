/*
 * Hasher.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: cawl-server
 */

#include "Hasher.h"

namespace Packets {

Hasher::Hasher() {
	// TODO Auto-generated constructor stub

}

bool Hasher::checkHash(std::hash<int> hash,  int data) {
	bool match = false;
	std::hash<int> int_hash(data);

	return match;
}

Hasher::~Hasher() {
	// TODO Auto-generated destructor stub
}

} /* namespace Packets */
