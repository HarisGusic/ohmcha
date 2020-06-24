#include "xml.h"
#include <exception>
#include <map>

namespace Ohmcha
{

const std::map<std::string, Component *(*)(const QDomElement &)> functionMap =
{
    {"resistor", xmlParseResistor},
    {"emf", xmlParseEmf},
    {"currentsource", xmlParseCurrentSource}
};

Schematic *xmlParseSchematic(std::string filename)
{
    // File checks...

    QDomDocument doc;
    QFile file(QString::fromStdString(filename));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Unable to read file");
    QString errorMsg;
    int errorLine, errorColumn;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
        throw std::logic_error(errorMsg.toStdString() + " at line "
                               + std::to_string(errorLine) + " column " + std::to_string(errorColumn));

    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() != "schematic")
        throw std::logic_error("The file is not a schematic");

    Schematic *schematic = new Schematic;

    // Document attributes
    bool linear = false;

    if (root.attribute("linear", "false") == "true")
        linear = true;

    for (auto child = root.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
        xmlParseComponentRecursively(child, *schematic);

    return schematic;
}

Component *xmlParseComponent(const QDomElement &element)
{
    try
    {
        auto function = functionMap.at(element.tagName().toStdString());
        return function(element);
    }
    catch (std::out_of_range&)
    {
        throw std::invalid_argument("The specified tag name is invalid");
    }
}

Component *xmlParseComponentRecursively(const QDomElement &element, Schematic &schematic)
{
    Component *parsedComponent = xmlParseComponent(element);
    schematic.add(parsedComponent);
    // Recursively parse any children of this element
    for (auto child = element.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
        xmlParseComponentRecursively(child, schematic);
    return parsedComponent;
}

Component *xmlParseResistor(const QDomElement &element)
{
    Resistor &r = *new Resistor;
    // If no value is given, the value of the last parsed resistor will be used
    static float lastValue = 1000;

    auto strValue = element.attribute("R", "");
    if (strValue == "")
        r.setResistance(lastValue);
    else
        r.setResistance(parseValue(strValue.toStdString()));

    return &r;
}

float parseValue(const std::string &text)
{
    float value;
    std::istringstream stream(text);

    if (!(stream >> value) || value < 0)
        throw std::logic_error("Invalid value");

    char prefix;
    while (std::isspace(prefix = stream.get()));
    if (stream.eof())
        return value;
    while (std::isspace(stream.get()));
    if (!stream.eof())
        throw std::logic_error("Value contains extra characters");

    switch (prefix)
    {
        case 'm':
            value *= 0.001;
        break;
        case 'u':
            value *= 1e-6;
        break;
        case 'n':
            value *= 1e-9;
        break;
        case 'p':
            value *= 1e-12;
        break;

        case 'k':
            value *= 1000;
        break;
        case 'M':
            value *= 1e6;
        break;
        case 'G':
            value *= 1e9;
        break;
        case 'T':
            value *= 1e12;
        break;

        default:
            throw std::logic_error("Invalid value prefix");
        }
    return value;
}

Component *xmlParseEmf(const QDomElement &element)
{
    //TODO implement
}

Component *xmlParseCurrentSource(const QDomElement &element)
{
    //TODO implement
}

}
