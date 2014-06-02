/*
 * Queue.h
 *
 *  Created on: May 26, 2014
 *      Author: cawl-mobile
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <boost/lockfree/queue.hpp>

namespace Db {

class Queue {
public:
	Queue();
	virtual ~Queue();
};

} /* namespace Db */

#endif /* QUEUE_H_ */
