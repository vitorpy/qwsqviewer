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

    Minutiae();

    void draw(QPainter* painter);

private:
    double _x;
    double _y;
    MinutiaeType _type;
};

#endif // MINUTIAE_H
