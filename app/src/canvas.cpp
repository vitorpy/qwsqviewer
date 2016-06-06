#include "inc/canvas.h"
#include <QPaintEvent>
#include <QPainter>
#include <QScrollArea>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{

}

void Canvas::setImage(const QImage &image) {
    _image = image;
    _scale = 1.0;

    resize(_image.size());
}

void Canvas::zoom(double factor)
{
    _scale *= factor;

    if (_scale < 0.5) _scale = 0.5;
    if (_scale > 2.0) _scale = 2.0;

    QSize size = _image.size();
    size *= _scale;

    resize(size);
}

void Canvas::fit()
{
    QScrollArea* sa = qobject_cast<QScrollArea*>(parent());
    if (!sa)
        return;

    QSize size = sa->size();

    double ar = (double)size.width()/(double)size.height();

    if (ar > 1.0) {
        _scale = (double)_image.width()/(double)size.width();
    } else {
        _scale = (double)_image.height()/(double)size.height();
    }

    QSize imsize = _image.size();
    imsize *= _scale;

    resize(imsize);
}

void Canvas::original()
{
    _scale = 1.0;
    resize(_image.size());
}

/*virtual*/ void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QRect source = _image.rect();
    QRect target(0, 0, _image.width() * _scale, _image.height() * _scale);

    painter.drawImage(target, _image, source);
}
