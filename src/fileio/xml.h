#include <QtXml>
#include <QFile>
#include <string>
#include "src/model/component.h"
#include "src/model/schematic.h"

namespace Ohmcha
{

float parseValue(const std::string &text);

Component *xmlParseComponent(const QDomElement &element);
Component *xmlParseComponentRecursively(const QDomElement &element, Schematic &schematic);
Component *xmlParseResistor(const QDomElement &element);
Component *xmlParseEmf(const QDomElement &element);
Component *xmlParseCurrentSource(const QDomElement &element);
Schematic *xmlParseSchematic(std::string filename);

QString xmlConvertSchematic(Schematic *schematic);
QString xmlConvertComponent(Component *component);
QString xmlConvertResistor(Resistor *resistor);

}
