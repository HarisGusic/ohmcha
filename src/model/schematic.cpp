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

std::vector<Component*> Schematic::getComponents()
{
    return components;
}


} // namespace Ohmcha
