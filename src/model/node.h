#pragma once

#include <vector>
#include "branch.h"

namespace Ohmcha
{

class Branch;

/**
 * An electrical node, i.e. a point
 * at which two or more branches meet.
 */
class Node
{
	std::vector<Branch *> branches;
	float potential;
};

}
