#pragma once

#include "node.h"
#include <Eigen/Dense>

namespace Ohmcha
{

using namespace Eigen;

class Node;

/**
 * An electrical branch.
 */
class Branch
{
protected:
	/**
	 * Current flows through this object from node 1 to node 2.
	 * Consequently, the reference voltage is positive if
	 * node 1 is at a higher potential than node 2.
	 */
	Node *node1, *node2;
	/**
	 * A branch has three quantities: V1 (potential at node1),
	 * V2 (potential at node2), I (current from node1 to node2).
	 * to represent a linear branch, we require an
	 * equation A * transpose([ V1 V2 I ]) = B.
	 */
	RowVector3f A;
	float B;

public:
	/**
	 * Create a branch by merging two branches that contain a common node.
	 * @throw TODO..if they don't have a common node
	 */
	Branch(const Branch &branch1, const Branch &branch2);

	/**
	 * TODO
	 */
	Branch(RowVector3f A, float B);

	/**
	 * TODO
	 */
	Branch(Node &node1, Node &node2, RowVector3f A, float B);

	// Setters

	void setA(const RowVector3f &matrix)
	{ A = matrix; }

	void setB(float x)
	{ B = x; }

	void setNode1(Node &n)
	{ node1 = &n; }

	void setNode2(Node &n)
	{ node2 = &n; }

	// Getters

	RowVector3f getA() const
	{ return A; }

	float getB() const
	{ return B; }

	Node *getNode1() const
	{ return node1; }

	Node *getNode2() const
	{ return node2; }
};

/**
 * A branch containing a resistor.
 */
class BResistor : public Branch
{
public:

	BResistor(float resistance);

	BResistor(float resistance, Node &node1, Node &node2);

	void setA(const RowVector3f &) = delete;

	void setB(float) = delete;

};

/**
 * A branch containing an ideal voltage source.
 */
class BEmf : public Branch
{
public:
	BEmf(float emf);

	BEmf(float emf, Node &node1, Node &node2);

	void setA(const RowVector3f &) = delete;

	void setB(float) = delete;

};

// TODO: add current source
}
