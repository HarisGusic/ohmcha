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

    // Setters

    // Getters
    std::vector<Component*> getComponents();

private:

    // Attributes

    std::vector<Component*> components;
    std::vector<Branch*> branches;
    std::vector<Node*> nodes;
};

}

#endif // OHMCHA_SCHEMATIC_H
