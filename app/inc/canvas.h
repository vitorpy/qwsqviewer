#ifndef CANVAS_H
#define CANVAS_H

#include <QImage>
#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);

    const QImage& image() const { return _image; }
    void setImage(const QImage& image) { _image = image; }

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    QImage _image;
};

#endif // CANVAS_H
