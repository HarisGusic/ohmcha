#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <Eigen/Dense>

namespace Ohmcha
{

using namespace Eigen;

class Component
{
public:
    Component();
};

/**
 * An electrical node, i.e. a point
 * at which two or more branches meet.
 */
class Node
{
    float potential;
};


/**
 * An electrical branch.
 */
class Branch : public Component
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

    void setA(const RowVector3f &matrix);

    void setB(float x);

    void setNode1(Node &n);

    void setNode2(Node &n);

    // Getters

    RowVector3f getA() const;

    float getB() const;

    Node *getNode1() const;

    Node *getNode2() const;
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

class Resistor : public Branch
{

};

}

#endif // COMPONENT_H
