#include "inc/canvas.h"
#include <QPaintEvent>
#include <QPainter>
#include <QScrollArea>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    _factor = 1.0;
}

void Canvas::setImage(const QImage &image) {
    _image = image;
    _factor = 1.0;

    resize(_image.size());
}

void Canvas::zoom(double factor)
{
    _factor *= factor;

    if (_factor < 0.5) _factor = 0.5;
    if (_factor > 2.0) _factor = 2.0;

    QSize size = _image.size();
    size *= _factor;

    resize(size);
}

void Canvas::fit()
{
    QObject* widget = parent();
    QScrollArea* sa = qobject_cast<QScrollArea*>(widget);
    if (!sa)
        return;

    QSize size = sa->size();

    double ar = (double)size.width()/(double)size.height();

    if (ar > 1.0) {
        _factor = (double)_image.width()/(double)size.width();
    } else {
        _factor = (double)_image.height()/(double)size.height();
    }

    QSize imsize = _image.size();
    imsize *= _factor;

    resize(imsize);
}

void Canvas::original()
{
    _factor = 1.0;
    resize(_image.size());
}

void Canvas::addMinutiae(Minutiae m)
{
    _mutex.lock();
    _minutiae.append(m);
    _mutex.unlock();
}

/*virtual*/ void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QRect source = _image.rect();
    QRect target(0, 0, _image.width() * _factor, _image.height() * _factor);

    painter.drawImage(target, _image, source);

    if (!_mutex.tryLock(10))
        return;

    foreach (Minutiae m, _minutiae) {
        m.draw(&painter);
    }

    _mutex.unlock();
}

