#include "branch.h"

namespace Ohmcha
{

Branch::Branch(const Branch &branch1, const Branch &branch2)
{
	//TODO implement
}

Branch::Branch(RowVector3f A, float B) : A(std::move(A)), B(B)
{}

Branch::Branch(Node &node1, Node &node2, RowVector3f A, float B)
		: node1(&node1), node2(&node2), A(std::move(A)), B(B)
{}

BResistor::BResistor(float resistance)
		: Branch({1, -1, -resistance}, 0)
{

}

BResistor::BResistor(float resistance, Node &node1, Node &node2)
		: Branch(node1, node2, {1, -1, -resistance}, 0)
{}

BEmf::BEmf(float emf)
		: Branch({1, -1, 0}, emf)
{}

BEmf::BEmf(float emf, Node &node1, Node &node2) : Branch(node1, node2, {1, -1, 0}, emf)
{}

}
