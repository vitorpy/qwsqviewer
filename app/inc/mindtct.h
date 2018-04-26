#ifndef MINDTCT_H
#define MINDTCT_H

#include <QRunnable>
#include <lfs.h>

class Canvas;

class MinutiaeDetect : public QRunnable
{
public:
    MinutiaeDetect(Canvas* canvas = 0);

    Canvas* canvas() const { return _canvas; }
    void setCanvas(Canvas* canvas) { _canvas = canvas; }

    virtual void run();

private:
    Canvas* _canvas;
};

#endif // MINDTCT_H
