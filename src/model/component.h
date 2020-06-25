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

    struct Pos { float x, y; };

protected:

    /**
     * Additional information that is not relevant for algorithms.
     */
    struct MetaInfo
    {
        std::string name;
        Pos *pos{};
    };

public:

    // Methods
    virtual Component *copy() const = 0;
    /**
     * Dynamically allocate a Component based on the
     * specified class name.
     * @return Pointer to the dynamically allocated object.
     */
    static Component* newByName(std::string name);

    // Setters
    void setName(const std::string &name);
    /**
     * Set the position of this component in the schematic. If position
     * is null, the position is considered arbitrary.
     */
    void setPosition(Pos *position);

    // Getters
    /**
     * @return Name given by the user.
     */
    std::string getName() const;
    /** See setPosition */
    Pos *getPosition() const;
    virtual int getTerminalCount() const = 0;

protected:

    // Attributes

    MetaInfo *metaInfo = nullptr;
};

/**
 * An electrical node, i.e. a point
 * at which two or more branches meet.
 */
class Node : public Component
{
public:
    Node();

    Component *copy() const override;

    virtual int getTerminalCount() const override;
};

/**
 * An electrical branch.
 */
class Branch : public Component
{
public:

    // Constructors
    /**
     * A default branch, equivalent to a short circuit.
     */
    Branch();
    /**
     * Create a branch by merging two branches that contain a common node.
     * @throw TODO..if they don't have a common node
     */
    Branch(const Branch &branch1, const Branch &branch2);
    Branch(RowVector3f A, float B);
    Branch(Node &node1, Node &node2, RowVector3f A, float B);
    Branch(Component *component, Node *node1 = nullptr, Node *node2 = nullptr);

    // Methods
    Component *copy() const override;
    void addComponent(Component *component);

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
    int getTerminalCount() const override;

protected:

    // Attributes

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
};

class Resistor : public Component
{

public:

    // Constructors
    Resistor();
    Resistor(float resistance);

    // Methods
    Component *copy() const override;

    // Setters
    void setResistance(float r);

    // Getters
    int getTerminalCount() const;
    float getResistance() const;

private:

    // Attributes

    float resistance;
};

class Emf : public Component
{
public:

    // Constructors
    Emf();
    Emf(float emf);

    // Methods
    Component *copy() const override;

    // Setters
    void setEmf(float emf);

    // Getters
    int getTerminalCount() const override;
    float getEmf() const;

private:

    // Attributes

    float emf;
};

class CurrentSource : public Component
{
public:

    // Constructors
    CurrentSource();
    CurrentSource(float current);

    // Methods
    Component *copy() const override;

    // Setters
    void setCurrent(float current);

    // Getters
    int getTerminalCount() const override;
    float getCurrent() const;

private:

    // Attributes

    float current;
};

}

#endif // COMPONENT_H
