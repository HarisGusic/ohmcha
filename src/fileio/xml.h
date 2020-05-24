#include <QtXml>
#include <QFile>
#include <string>
#include "../model/component.h"

namespace Ohmcha
{

float parseValue(const std::string &text);

Component *xmlParseComponent(const QDomElement &element);
Component *xmlParseComponentRecursively(const QDomElement &element, Schematic &schematic);
Component *xmlParseResistor(const QDomElement &element);
Schematic *xmlParseSchematic(std::string filename);

}
