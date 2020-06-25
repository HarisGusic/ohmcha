#include "schematic.h"

namespace Ohmcha {

Schematic::Schematic()
{

}

void Schematic::add(Component *component)
{
    if (dynamic_cast<Branch*>(component) != nullptr)
        branches.push_back(dynamic_cast<Branch*>(component));
    else if (dynamic_cast<Node*>(component))
        nodes.push_back(dynamic_cast<Node*>(component));
    else
        components.push_back(component);
}

void Schematic::clearBranches()
{
    branches.clear();
}

void Schematic::clearNodes()
{
    nodes.clear();
}

std::vector<Component*> Schematic::getComponents() const
{
    return components;
}

std::vector<Branch *> Schematic::getBranches() const
{
    return branches;
}

std::vector<Node *> Schematic::getNodes() const
{
    return nodes;
}

int Schematic::getComponentId(Component *component) const
{
    for (int i = 0; i < components.size(); ++i)
        if (components[i] == component)
            return i;
    return -1;
}

int Schematic::getNodeId(Node *node) const
{
    for (int i = 0; i < nodes.size(); ++i)
        if (nodes[i] == node)
            return i;
    return -1;
}

std::vector<Component *> &Schematic::getComponents()
{
    return components;
}

std::vector<Branch *> &Schematic::getBranches()
{
    return branches;
}

std::vector<Node *> &Schematic::getNodes()
{
    return nodes;
}

} // namespace Ohmcha
