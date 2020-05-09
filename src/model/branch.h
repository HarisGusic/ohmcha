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
	/**
	 * A branch has three quantities: V1 (potential at node1),
	 * V2 (potential at node2), I (current from node1 to node2).
	 * to represent a linear branch, we require an
	 * equation A * transpose([ V1 V2 I ]) = B.
	 */
	std::vector<float> A;
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
	Branch(const std::vector<float> A, float B) : A(A), B(B)
	{}

	// Setters

	void setA(const std::vector<float> &matrix)
	{ A = matrix; }

	void setB(float x)
	{ B = x; }

	void setNode1(Node &n)
	{ node1 = &n; }

	void setNode2(Node &n)
	{ node2 = &n; }

	// Getters

	std::vector<float> getA()
	{ return A; }

	float getB()
	{ return B; }

	Node *getNode1(Node *n)
	{ return node1; }

	Node *getNode2(Node *n)
	{ return node2; }
};

class BResistor : public Branch
{
public:
	BResistor(float resistance);

	void setA(const std::vector<float> &) = delete;

	void setB(float) = delete;

};

class BEmf : public Branch
{
public:
	BEmf(float emf);

	void setA(const std::vector<float> &) = delete;

	void setB(float) = delete;

};

}
