#pragma once

#include "node.h"

namespace Ohmcha
{

class Node;

/**
 * An electrical branch.
 */
class Branch
{
	/**
	 * Current flows through this object from node 1 to node 2.
	 * Consequently, the reference voltage is positive if
	 * node 1 is at a higher potential than node 2.
	 */
	Node *node1, *node2;
};

}
