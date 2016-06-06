#include "inc/canvas.h"
#include <QPaintEvent>
#include <QPainter>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{

}

/*virtual*/ void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.drawImage(0, 0, _image);
}
