#include "graphic_emf.h"

#include <QPainter>

namespace Ohmcha {

GraphicEmf::GraphicEmf()
    : GraphicComponent()
{
    component = new Emf;
    terminals = { {0, -size.height() / 2}, {0, size.height() / 2} };
    textPos = {26, 0};
}

GraphicEmf::GraphicEmf(Emf *emf)
    : GraphicEmf()
{
    component = emf;
}

GraphicEmf::GraphicEmf(const GraphicEmf &original)
    : GraphicComponent(original)
{

}

QRectF GraphicEmf::boundingRect() const
{
    //TODO: remove excessive space
    return {
        -size.width()/2 - pen.width()/2,
        -size.height()/2 - pen.width()/2 - 4, // 4 is the radius of a terminal
        size.width() + pen.width(),
        size.height() + pen.width() + 8
    };
}

void GraphicEmf::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!component) return;

    applyColors(painter, option);

    // Draw circle when cursor is near terminal
    if (_selectedTerminal)
    {
        painter->save();
        painter->setBrush(QBrush(painter->pen().color()));
        painter->drawEllipse(*_selectedTerminal, 4, 4);
        painter->restore();
    }

    painter->drawLine(0, -size.height()/2, 0, -4);
    painter->drawLine(-size.width()/2, -4, size.width()/2, -4);
    painter->drawLine(-size.width()/2 + 9, 4, size.width()/2 - 9, 4);
    painter->drawLine(0, 4, 0, size.height()/2);

    drawText(painter);

}

GraphicComponent *GraphicEmf::copy() const
{
    return new GraphicEmf(*this);
}

}
