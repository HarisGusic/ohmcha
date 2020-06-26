#include <QtXml>
#include <QFile>
#include <string>
#include "src/model/component.h"
#include "src/model/schematic.h"

namespace Ohmcha
{

float parseValue(const std::string &text);

Component *xmlParseComponent(Schematic *schematic, const QDomElement &element);
Component *xmlParseComponentRecursively(const QDomElement &element, Schematic *schematic);
Component *xmlParseResistor(const QDomElement &element);
Component *xmlParseEmf(const QDomElement &element);
Component *xmlParseCurrentSource(const QDomElement &element);
Component *xmlParseNode(const QDomElement &element);
Component *xmlParseBranch(const QDomElement &element, Schematic *schematic);
Schematic *xmlParseSchematic(std::string filename);

QString xmlConvertSchematic(Schematic *schematic);
QString xmlConvertComponent(Component *component);
QString xmlConvertResistor(Resistor *resistor);
QString xmlConvertEmf(Emf *emf);
QString xmlConvertCurrentSource(CurrentSource *source);
QString xmlConvertBranch(Schematic *schematic, Branch *branch);
QString xmlConvertNode(Node *node);

}
