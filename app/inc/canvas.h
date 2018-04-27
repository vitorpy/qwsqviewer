#ifndef CANVAS_H
#define CANVAS_H

#include <QImage>
#include <QWidget>
#include <QList>
#include <QMutex>

#include "minutiae.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);

    const QImage& image() const { return _image; }
    void setImage(const QImage& image);

    void zoom(double factor);
    void fit();
    void original();

    void addMinutiae(Minutiae m);

    double ppi() const { return _ppi; }
    void setPpi(double ppi) { _ppi = ppi; }

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    QImage _image;
    double _factor; //!< \brief Zoom factor
    double _ppi; //!< \brief Pixels per inch (assumes it's the same for x and y)
    QMutex _mutex;
    QList<Minutiae> _minutiae;
};

#endif // CANVAS_H
