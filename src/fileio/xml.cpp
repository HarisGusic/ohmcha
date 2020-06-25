#include "xml.h"
#include <exception>
#include <map>

namespace Ohmcha
{

const std::map<std::string, Component *(*)(const QDomElement &)> parseMap =
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
        auto function = parseMap.at(element.tagName().toStdString());
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

void parseCommonComponent(const QDomElement &element, Component &c)
{
    QString str, str2;

    // Name
    str = element.attribute("name", "");
    if (str != "")
        c.setName(str.toStdString());

    // X and Y coordinates
    str = element.attribute("x", "");
    str2 = element.attribute("y", "");
    if (str != "" && str2 != "")
        c.setPosition(new Component::Pos{str.toFloat(), str2.toFloat()});

    // Angle
    str = element.attribute("angle", "0");
    c.setAngle(str.toFloat());

    // Text position
    str = element.attribute("tx", "0");
    str2 = element.attribute("ty", "0");
    c.setTextPos({str.toFloat(), str2.toFloat()});

    // Text angle
    str = element.attribute("textAngle", "0");
    c.setTextAngle(str.toFloat());

    // Text independent
    str = element.attribute("textIndependent", "false");
    c.setTextOrientationIndependent(str == "true");

    // Text anchor
    str = element.attribute("textAnchor", "3");
    c.setTextAnchor(str.toInt());
}

Component *xmlParseResistor(const QDomElement &element)
{
    Resistor &r = *new Resistor;
    // If no value is given, the value of the last parsed resistor will be used
    static float lastValue = 1000;
    QString str;

    parseCommonComponent(element, r);

    // Resistance value
    str = element.attribute("R", "");
    if (str == "")
        r.setResistance(lastValue);
    else
        r.setResistance(parseValue(str.toStdString()));

    lastValue = r.getResistance();

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
    Emf &e = *new Emf;
    // If no value is given, the value of the last parsed emf will be used
    static float lastValue = 1;
    QString str;

    parseCommonComponent(element, e);

    // Emf value
    str = element.attribute("E", "");
    if (str == "")
        e.setEmf(lastValue);
    else
        e.setEmf(parseValue(str.toStdString()));

    lastValue = e.getEmf();

    return &e;
}

Component *xmlParseCurrentSource(const QDomElement &element)
{
    CurrentSource &i = *new CurrentSource;
    // If no value is given, the value of the last parsed emf will be used
    static float lastValue = 0.001;
    QString str;

    parseCommonComponent(element, i);

    // Emf value
    str = element.attribute("I", "");
    if (str == "")
        i.setCurrent(lastValue);
    else
        i.setCurrent(parseValue(str.toStdString()));

    lastValue = i.getCurrent();

    return &i;
}

QString convertCommonComponent(Component *component)
{
    auto *pos = component->getPosition();
    QString x = pos != nullptr ? QString::number(pos->x) : "",
            y = pos != nullptr ? QString::number(pos->y) : "";
    return QString("x=\"%1\" y=\"%2\" name=\"%3\" angle=\"%4\" textIndependent=\"%5\" "
                   "tx=\"%6\" ty=\"%7\" textAngle=\"%8\" textAnchor=\"%9\"")
            .arg(x).arg(y)
            .arg(QString::fromStdString(component->getName()))
            .arg(component->getAngle())
            .arg(component->isTextOrientationIndependent() ? "true" : "false")
            .arg(component->getTextPos().x).arg(component->getTextPos().y)
            .arg(component->getTextAngle())
            .arg(component->getTextAnchor());
}

QString xmlConvertComponent(Component *component)
{
    if (dynamic_cast<Resistor*>(component) != nullptr)
        return xmlConvertResistor(dynamic_cast<Resistor*>(component));
    else if (dynamic_cast<Emf*>(component) != nullptr)
        return xmlConvertEmf(dynamic_cast<Emf*>(component));
    else if (dynamic_cast<CurrentSource*>(component) != nullptr)
        return xmlConvertCurrentSource(dynamic_cast<CurrentSource*>(component));
    //TODO else
}

QString xmlConvertResistor(Resistor *resistor)
{
    QString result("<resistor ");
    result += convertCommonComponent(resistor);
    result += QString(" R=\"%1\"").arg(resistor->getResistance());
    result += "/>";
    return result;
}

QString xmlConvertSchematic(Schematic *schematic)
{
    QString result("<schematic>");
    for (auto *component : schematic->getComponents())
        result += "\n" + xmlConvertComponent(component);
    //TODO add branches
    result += "\n</schematic>";
    return result;
}

QString xmlConvertEmf(Emf *emf)
{
    QString result("<emf ");
    result += convertCommonComponent(emf);
    result += QString(" R=\"%1\"").arg(emf->getEmf());
    result += "/>";
    return result;
}

QString xmlConvertCurrentSource(CurrentSource *source)
{
    QString result("<currentsource ");
    result += convertCommonComponent(source);
    result += QString(" R=\"%1\"").arg(source->getCurrent());
    result += "/>";
    return result;
}

}
