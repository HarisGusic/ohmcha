#include "component.h"

namespace Ohmcha
{

/**********
 * Branch *
 **********/

Branch::Branch()
    : A{1, -1, 0}, B(0)
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

Branch::Branch(Component *component, Node *node1, Node *node2)
    : Branch()
{
    addComponent(component);
    node1 = node1;
    node2 = node2;
}

void Branch::setA(const RowVector3f &matrix)
{ A = matrix; }

void Branch::setB(float x)
{ B = x; }

void Branch::setNode1(Node &n)
{ node1 = &n; }

void Branch::setNode2(Node &n)
{ node2 = &n; }

void Branch::addComponent(Component *component)
{
    if (component->getTerminalCount() != 2)
        return;//TODO this case must be treated separately
    RowVector3f A1 = A, A2;
    float B1 = B, B2 = 0;
    // TODO find a more general way to do this
    if (dynamic_cast<CurrentSource*>(component))
    {
        A = {0, 0, 1};
        B = dynamic_cast<CurrentSource*>(component)->getCurrent();
        return;
    }
    if (dynamic_cast<Resistor*>(component))
    {
        A2 = {1, -1, - dynamic_cast<Resistor*>(component)->getResistance()};
        B2 = 0;
    }
    if (dynamic_cast<Emf*>(component))
    {
        A2 = {1, -1, 0};
        B2 = dynamic_cast<Emf*>(component)->getEmf();
    }
    A(0) = -A1(0) * A2(0) / A1(1);
    A(1) = A2(1);
    A(2) = A2(2) - A2(0) / A1(1) * A1(2);
    B = B2 - A2(0) / A1(0) * B1;
}

RowVector3f Branch::getA() const
{ return A; }

float Branch::getB() const
{ return B; }

Node *Branch::getNode1() const
{ return node1; }

Node *Branch::getNode2() const
{ return node2; }

int Branch::getTerminalCount() const
{
    return 2;
}

Component *Branch::copy() const
{
    return new Branch(A, B);
}

/************
 * Resistor *
 ************/

float Resistor::getResistance() const
{
    return resistance;
}

Resistor::Resistor()
{
}

Resistor::Resistor(float resistance)
    : resistance(resistance)
{
}

Component *Resistor::copy() const
{
    return new Resistor(resistance);
}

void Resistor::setResistance(float r)
{
    resistance = r;
}

int Resistor::getTerminalCount() const
{
    return 2;
}

Emf::Emf()
{

}

Emf::Emf(float emf)
    : emf(emf)
{

}

Component *Emf::copy() const
{
    return new Emf(emf);
}

void Emf::setEmf(float emf)
{
    this->emf = emf;
}

float Emf::getEmf() const
{
    return emf;
}

int Emf::getTerminalCount() const
{
    return 2;
}

CurrentSource::CurrentSource()
{

}

CurrentSource::CurrentSource(float current)
    : current(current)
{

}

Component *CurrentSource::copy() const
{
    return new CurrentSource(current);
}

void CurrentSource::setCurrent(float current)
{
    this->current = current;
}

float CurrentSource::getCurrent() const
{
    return current;
}

int CurrentSource::getTerminalCount() const
{
    return 2;
}


void Component::setName(const std::string &name)
{
    if (metaInfo == nullptr)
        metaInfo = new MetaInfo;
    metaInfo->name = name;
}

std::string Component::getName() const
{
    return metaInfo ? metaInfo->name : "";
}

Component *Component::newByName(std::string name)
{
    if (name == "Resistor")
        return new Resistor;
    else;
        //TODO custom component type
}

}
