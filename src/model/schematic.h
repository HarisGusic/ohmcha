#ifndef OHMCHA_SCHEMATIC_H
#define OHMCHA_SCHEMATIC_H

#include "component.h"

namespace Ohmcha {

class Schematic
{
    std::vector<Component*> components;
    std::vector<Branch*> branches;
    std::vector<Node*> nodes;
public:
    // Constructors
    Schematic();

    // Methods
    void add(Component *component);

    std::vector<Component*> getComponents();
};

}

#endif // OHMCHA_SCHEMATIC_H
