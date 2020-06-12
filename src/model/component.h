#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <Eigen/Dense>

namespace Ohmcha
{

using namespace Eigen;

class Component
{
protected:
    /**
     * Additional information that is not relevant for algorithms.
     */
    struct MetaInfo
    {
        std::string name;
        std::vector<float> positions;
    };
    MetaInfo *metaInfo = nullptr;

public:

    // Methods
    virtual Component *copy() const = 0;

    // Setters
    void setName(const std::string &name);

    // Getters
    /**
     * @return Name given by the user.
     */
    std::string getName() const;
    virtual int getTerminalCount() const = 0;

    /**
     * Dynamically allocate a Component based on the
     * specified class name.
     * @return Pointer to the dynamically allocated object.
     */
    static Component* newByName(std::string name);
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
     * A default branch, equivalent to a short circuit.
     */
    Branch();
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

    Branch(Component *component, Node *node1 = nullptr, Node *node2 = nullptr);

    // Setters

    void setA(const RowVector3f &matrix);
    void setB(float x);
    void setNode1(Node &n);
    void setNode2(Node &n);
    void addComponent(Component *component);

    // Getters

    RowVector3f getA() const;
    float getB() const;
    Node *getNode1() const;
    Node *getNode2() const;
    int getTerminalCount() const;

    Component *copy() const override;
};

class Resistor : public Component
{
    float resistance;

public:

    Resistor();
    Resistor(float resistance);

    Component *copy() const override;

    void setResistance(float r);

    int getTerminalCount() const;
    float getResistance() const;
};

class Emf : public Component
{
    float emf;
public:

    Emf();
    Emf(float emf);

    Component *copy() const override;

    void setEmf(float emf);

    float getEmf() const;
    int getTerminalCount() const override;

};

class CurrentSource : public Component
{
    float current;
public:

    CurrentSource();
    CurrentSource(float current);

    Component *copy() const override;

    void setCurrent(float current);

    float getCurrent() const;
    int getTerminalCount() const override;
};

}

#endif // COMPONENT_H
