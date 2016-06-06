#include "inc/canvas.h"
#include <QPaintEvent>
#include <QPainter>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{

}

void Canvas::setImage(const QImage &image) {
    _image = image;

    resize(_image.size());
}

/*virtual*/ void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.drawImage(0, 0, _image);
}
