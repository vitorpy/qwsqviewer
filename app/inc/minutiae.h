#ifndef MINUTIAE_H
#define MINUTIAE_H

class QPainter;

class Minutiae
{
public:
    enum MinutiaeType {
        Ending,
        Bifurcation,
        Unknown
    };

    Minutiae(int x, int y);

    void draw(QPainter* painter);

private:
    double _x;
    double _y;
    MinutiaeType _type;
    double _angle;
};

#endif // MINUTIAE_H
