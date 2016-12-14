#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qcustomplot.h"
#include <QVector>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void draw12();
//    void draw23();
    void draw13();
    void drawCircle(double x, double y, double radius,int flag);
    void drawSingle(double x, double y, double radius, int flag);
    ~Widget();

protected slots:
    void draw23();

private:
    Ui::Widget *ui;
    QVector<double> p1x, p1y;
    QVector<double> p2x,p2y;
    QVector<double> p3x ,p3y;
    void drawHyperbola(double a1, double b1, double a2, double b2, int flag);
};

#endif // WIDGET_H
