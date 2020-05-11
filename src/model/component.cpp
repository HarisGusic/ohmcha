#include "component.h"

namespace Ohmcha
{

Component::Component()
{

}

Branch::Branch(const Branch &branch1, const Branch &branch2)
{
    //TODO implement
}

Branch::Branch(RowVector3f A, float B) : A(std::move(A)), B(B)
{}

Branch::Branch(Node &node1, Node &node2, RowVector3f A, float B)
        : node1(&node1), node2(&node2), A(std::move(A)), B(B)
{}

void Branch::setA(const RowVector3f &matrix)
{ A = matrix; }

void Branch::setB(float x)
{ B = x; }

void Branch::setNode1(Node &n)
{ node1 = &n; }

void Branch::setNode2(Node &n)
{ node2 = &n; }

RowVector3f Branch::getA() const
{ return A; }

float Branch::getB() const
{ return B; }

Node *Branch::getNode1() const
{ return node1; }

Node *Branch::getNode2() const
{ return node2; }

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
