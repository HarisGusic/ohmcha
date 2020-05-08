#pragma once

#include "branch.h"

/**
 * All entities of Ohmcha are part of this
 * namespace. This makes it possible to use
 * the project as a library for other projects.
 */
namespace Ohmcha
{

/**
 * Generate a minimum spanning tree of a connected sub-graph
 * starting at the specified node.
 * @param start - The start node
 * @return Vector of pointers of branches that constitute the generated MST
 * @details TODO
 */
std::vector<Branch *> generate_MST(Node *start);

}
