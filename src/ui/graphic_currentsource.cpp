#include "graphic_currentsource.h"

#include <QPainter>

namespace Ohmcha {

GraphicCurrentSource::GraphicCurrentSource()
    : GraphicComponent()
{
    component = new CurrentSource;
    size = {30, 70};
    terminals = { {0, -size.height() / 2}, {0, size.height() / 2} };
    textPos = {20, 0};
}

GraphicCurrentSource::GraphicCurrentSource(CurrentSource *source)
    : GraphicCurrentSource()
{
    component = source;
    synchronize();
}

GraphicCurrentSource::GraphicCurrentSource(const GraphicCurrentSource &original)
    : GraphicComponent(original)
{

}

QRectF GraphicCurrentSource::boundingRect() const
{
    return {
        -size.width()/2 - pen.width()/2,
        -size.height()/2 - pen.width()/2 - 4, // 4 is the radius of a terminal
        size.width() + pen.width(),
        size.height() + pen.width() + 8
    };
}

void GraphicCurrentSource::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    painter->drawLine(0, -size.height()/2, 0, -15);
    painter->drawLine(0, 15, 0, size.height()/2);
    painter->drawEllipse({0, 0}, 15, 15);
    // Draw inner line + arrow
    painter->drawLine(0, -10, 0, 10);
    painter->drawLine(0, 10, -4, 6);
    painter->drawLine(0, 10, 4, 6);

    drawText(painter);

}

GraphicComponent *GraphicCurrentSource::copy() const
{
    return new GraphicCurrentSource(*this);
}

}
