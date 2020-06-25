#ifndef OHMCHA_SCHEMATIC_H
#define OHMCHA_SCHEMATIC_H

#include "component.h"

namespace Ohmcha {

class Schematic
{
public:

    // Constructors
    Schematic();

    // Methods
    void add(Component *component);
    void clearBranches();
    void clearNodes();

    // Setters

    // Getters
    std::vector<Component*> &getComponents();
    std::vector<Branch*> &getBranches();
    std::vector<Node*> &getNodes();
    std::vector<Component*> getComponents() const;
    std::vector<Branch*> getBranches() const;
    std::vector<Node*> getNodes() const;
    int getComponentId(Component *component) const;
    int getNodeId(Node *node) const;

private:

    // Attributes

    std::vector<Component*> components;
    std::vector<Branch*> branches;
    std::vector<Node*> nodes;
};

}

#endif // OHMCHA_SCHEMATIC_H
