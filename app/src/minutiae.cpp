#include "inc/minutiae.h"

#include <QtGlobal>
#include <QPainter>

Minutiae::Minutiae(int x, int y) : _x(x), _y(y)
{

}

void Minutiae::draw(QPainter *painter)
{
    const auto side = 8;

    painter->drawEllipse(_x - side/2, _y - side/2, side, side);
}
