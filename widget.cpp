#include "widget.h"
#include "ui_widget.h"

#define PI 3.1415926

Widget::Widget(QWidget *parent) :
    QWidget(parent),p1x(1),p1y(1),p2x(1),p2y(1),p3x(1),p3y(1),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->P1->setText("(0,35)");ui->P2->setText("(0,-35)");
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->widget->xAxis->setRange(-150,150);
    ui->widget->yAxis->setRange(-150,150);

    p1x[0]=0.0; p1y[0]=35;p2x[0]=0.0;p2y[0]=-35;
    double r12=p1y[0]-p2y[0];
    // create graph and assign data to it:
    ui->widget->addGraph();//0
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,10));
    ui->widget->graph(0)->setPen(QColor(80,100,100,100));
    ui->widget->graph(0)->setData(p1x, p1y);
    ui->widget->addGraph();//1
    ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar,10));
    ui->widget->graph(1)->setData(p2x, p2y);

    // give the axes some labels:
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    //c121 represents the circle using p1 as centroid, r12-3.5 as radius,
    //c122 represents the circle using p1 as centroid, r12+3.5 as radius.
    QCPItemEllipse *c121 = new QCPItemEllipse(ui->widget);
    c121->setAntialiased(true);
    c121->topLeft->setCoords(-r12+3.5, r12-3.5+35);
    c121->bottomRight->setCoords(r12-3.5, -r12+3.5+35);
    QCPItemEllipse *c122 = new QCPItemEllipse(ui->widget);
    c122->setAntialiased(true);
    c122->topLeft->setCoords(-r12-3.5, r12+3.5+35);
    c122->bottomRight->setCoords(r12+3.5, -r12-3.5+35);
    QCPItemEllipse *c211 = new QCPItemEllipse(ui->widget);
    c211->setAntialiased(true);
    c211->topLeft->setCoords(-r12+3.5, r12-3.5-35);
    c211->bottomRight->setCoords(r12-3.5, -r12+3.5-35);
    QCPItemEllipse *c212 = new QCPItemEllipse(ui->widget);
    c212->setAntialiased(true);
    c212->topLeft->setCoords(-r12-3.5, r12+3.5-35);
    c212->bottomRight->setCoords(r12+3.5, -r12-3.5-35);

    drawCircle(p1x[0],p1y[0],r12,12);
    drawCircle(p2x[0],p2y[0],r12,21);
    //hyperbolic curve
    QVector<double>ex(1000),ey(1000);//Array of the Circle
    double L=qSqrt(qPow(p1x[0]-p2x[0],2) + qPow(p1y[0]-p2y[0],2));
    double st = PI/(2.4)*2/500;
    double st1 = st - PI/2.4;
    for(int i=0; i<500;i++)
    {
        st1 = st*i - PI/2.4;
        ex[i]=qSqrt(qPow(L/2,2)-qPow(3.5/2,2))*qTan(st1);
        ey[i]=qSqrt(qPow(3.5/2,2))*(1/qCos(st1));
        ex[i+500] = ex[i];
        ey[i+500] = -ey[i];
    }
    ui->widget->addGraph ();//2
    drawHyperbola(p1x[0], p1y[0],p2x[0],p2y[0],12);
    ui->widget->addGraph ();//3
    ui->widget->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar,6));
    QObject::connect(ui->p3xEdit, SIGNAL(textChanged()), this, SLOT(draw23()));
    QObject::connect(ui->p3yEdit, SIGNAL(textChanged()), this, SLOT(draw23()));
    ui->widget->addGraph();//4
    p3x[0]=50; p3y[0]=50;
    drawCircle(50,50,qSqrt(qPow(p3x[0]-p1x[0],2)+qPow(p3y[0]-p1y[0],2)),13);
}

void Widget::mouseDoubleClickEvent (QMouseEvent *event)
{

}

void Widget::draw23 ()
{
    QString s_x, s_y;
    s_x = ui->p3xEdit->toPlainText();s_y = ui->p3yEdit->toPlainText ();
    if (s_x=="" || s_y=="")
        return;
    p3x[0]=s_x.toDouble (); p3y[0]=s_y.toDouble ();
    ui->widget->graph(2)->setData(p3x,p3y);

    double r23 = qSqrt(qPow(p3x[0]-p2x[0],2)+qPow(p3y[0]-p2y[0],2));

    static QCPItemEllipse *c231 = new QCPItemEllipse(ui->widget);
    c231->setAntialiased(true);
    c231->topLeft->setCoords(p3x[0]-r23+3.5, p3y[0]+r23-3.5);
    c231->bottomRight->setCoords(p3x[0]+r23-3.5, p3y[0]-r23+3.5);

    static QCPItemEllipse *c232 = new QCPItemEllipse(ui->widget);
    c232->setAntialiased(true);
    c232->topLeft->setCoords(p3x[0]-r23-3.5, p3y[0]+r23+3.5);
    c232->bottomRight->setCoords(p3x[0]+r23+3.5, p3y[0]-r23-3.5);

    static QCPItemEllipse *c321 = new QCPItemEllipse(ui->widget);
    c321->setAntialiased(true);
    c321->topLeft->setCoords(p2x[0]-r23+3.5, p2y[0]+r23-3.5);
    c321->bottomRight->setCoords(p2x[0]+r23-3.5, p2y[0]-r23+3.5);
    c321->setPen (QPen(QColor(24,100,100,1),6));

    static QCPItemEllipse *c322 = new QCPItemEllipse(ui->widget);
    c322->setAntialiased(true);
    c322->topLeft->setCoords(p2x[0]-r23-3.5, p2y[0]+r23+3.5);
    c322->bottomRight->setCoords(p2x[0]+r23+3.5, p2y[0]-r23-3.5);
    drawCircle(p2x[0],p2y[0],r23,23);
    drawCircle(p3x[0],p3y[0],r23,32);
    draw13();
    ui->widget->graph(0)->setData(p1x, p1y);
    ui->widget->graph(1)->setData(p2x, p2y);
    ui->widget->graph(2)->setData(p3x, p3y);
    drawHyperbola(p2x[0],p2y[0],p3x[0],p3y[0],23);
    drawSingle(p1x[0],p1y[0],r23,1);
    static QCPCurve *threePoint=new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    QVector<double> tPx(4),tPy(4);
    tPx[0]=p1x[0];tPy[0]=p1y[0];tPx[1]=p2x[0];tPy[1]=p2y[0];tPx[2]=p3x[0];tPy[2]=p3y[0];tPx[3]=p1x[0];tPy[3]=p1y[0];
    threePoint->setData(tPx,tPy);
    ui->widget->replot();
}

void Widget::drawSingle(double x, double y, double radius, int flag)
{
    int length=1000;
    QVector<double> ct(length),cx(length),cy(length);//Array of the Circle
    double st = PI/length*2;
    double st1;
    double r1 = radius-3.5;
    double r2 = radius+3.5;
    for(int i=0; i<length/2;i+=2)
    {
        st1=st*i;
        ct[i]=i; ct[i+1]=i+1;
        cx[i]=r1*qCos(st1)+x;
        cy[i]=r1*qSin(st1)+y;
        cx[i+1] = r2*qCos(st1)+x;
        cy[i+1] = r2*qSin(st1)+y;
        ct[i+length/2]=length-i; ct[i+length/2+1]=length-i-1;
        cx[i+length/2]=r1*qCos(st1)+x;
        cy[i+length/2]=-r1*qSin(st1)+y;
        cx[i+length/2+1] = r2*qCos(st1)+x;
        cy[i+length/2+1] = -r2*qSin(st1)+y;
    }
    static QCPCurve *c1=new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    static QCPCurve *c2= new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    static QCPCurve *c3= new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    QPen p1=QPen(QColor(255,255,0,100),3);
    QPen p2=QPen(QColor(0,255,255,100),3);
    QPen p3=QPen(QColor(255,0,255,100),3);
    c1->setPen(p1); c2->setPen(p2); c3->setPen(p3);
    switch (flag)
    {
    case 1:
        c1->setData (ct,cx,cy);
        break;
    case 2:
        c2->setData(ct,cx,cy);
        break;
    case 3:
        c3->setData(ct,cx,cy);
        break;
    default:
        break;
    }
}

void Widget::draw13()
{
    double r13 = qSqrt(qPow(p3x[0]-p1x[0],2)+qPow(p3y[0]-p1y[0],2));
    static QCPItemEllipse *c131 = new QCPItemEllipse(ui->widget);
    c131->setAntialiased(true);
    c131->topLeft->setCoords(-r13+3.5+p1x[0], r13-3.5+p1y[0]);
    c131->bottomRight->setCoords(r13-3.5+p1x[0], -r13+3.5+p1y[0]);
    static QCPItemEllipse *c132 = new QCPItemEllipse(ui->widget);
    c132->setAntialiased(true);
    c132->topLeft->setCoords(-r13-3.5+p1x[0], r13+3.5+p1y[0]);
    c132->bottomRight->setCoords(r13+3.5+p1x[0], -r13-3.5+p1y[0]);
    static QCPItemEllipse *c311 = new QCPItemEllipse(ui->widget);
    c311->setAntialiased(true);
    c311->topLeft->setCoords(-r13+3.5+p3x[0], r13-3.5+p3y[0]);
    c311->bottomRight->setCoords(r13-3.5+p3x[0], -r13+3.5+p3y[0]);
    static QCPItemEllipse *c312 = new QCPItemEllipse(ui->widget);
    c312->setAntialiased(true);
    c312->topLeft->setCoords(-r13-3.5+p3x[0], r13+3.5+p3y[0]);
    c312->bottomRight->setCoords(r13+3.5+p3x[0], -r13-3.5+p3y[0]);
    drawCircle(p3x[0],p3y[0],r13,31);
    drawCircle(p1x[0],p1y[0],r13,13);
    drawHyperbola(p1x[0],p1y[0],p3x[0],p3y[0],13);
    drawSingle(p2x[0],p2y[0],r13,2);
    drawSingle(p3x[0],p3y[0],qSqrt(qPow(p1x[0]-p2x[0],2) + qPow(p1y[0]-p2y[0],2)),3);
}

void Widget::drawHyperbola(double a1, double b1, double a2, double b2, int flag)
{
    static QCPCurve *e12 = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
    static QCPCurve *e23 = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
    static QCPCurve *e13 = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
    QPen p12=QPen(QColor(100,100,100,100),3);
    QPen p13=QPen(QColor(100,100,100,100),3);
    QPen p23=QPen(QColor(100,100,100,100),3);
    e12->setPen(p12);
    e13->setPen(p13);
    e23->setPen(p23);

    int length=1000;
    QVector<double> e1(2),e2(2);
    e2[0]=(a1-a2)/qSqrt(qPow(a1-a2,2)+qPow(b1-b2,2));
    e2[1]=(b1-b2)/qSqrt(qPow(a1-a2,2)+qPow(b1-b2,2));
    e1[0]=e2[1];
    e1[1]=-e2[0];
    QVector<double> et(length), ex(length),ey(length);//Array of the Circle
    double L=qSqrt(qPow(a1-a2,2) + qPow(b1-b2,2));
    double st = PI/(2.4)*2/length*2;
    double st1 = st - PI/2.4;
    for(int i=0; i<length/2;i++)
    {
        st1 = st*i - PI/2.4;
        double p,q,r,s;
        et[2*i]=2*i;
        p=qSqrt(qPow(L/2,2)-qPow(3.5/2,2))*qTan(st1);
        q=qSqrt(qPow(3.5/2,2))*(1/qCos(st1));
        et[2*i+1]=2*i+1;
        r = p;
        s = -q;
        ex[2*i]=e1[0]*p+e2[0]*q+(a1+a2)/2;
        ey[2*i]=e1[1]*p+e2[1]*q+(b1+b2)/2;
        ex[2*i+1]=e1[0]*r+e2[0]*s+(a1+a2)/2;
        ey[2*i+1]=e1[1]*r+e2[1]*s+(b1+b2)/2;
    }

    switch (flag)
    {
    case 12:
        e12->setData (et,ex,ey);
        break;
    case 13:
        e13->setData(et,ex,ey);
        break;
    case 23:
        e23->setData(et,ex,ey);
        break;
    default:
        break;
    }
}

void Widget::drawCircle (double x, double y, double radius, int flag)
{
    int length=1000;
    QVector<double> ct(length),cx(length),cy(length);//Array of the Circle
    double st = PI/length*2;
    double st1;
    double r1 = radius-3.5;
    double r2 = radius+3.5;
    for(int i=0; i<length/2;i+=2)
    {
        st1=st*i;
        //First Method
//        cx[i]=r1*qCos(st1)+x;
//        cy[i]=r1*qSin(st1)+y;
//        cx[1000-i] = r2*qCos(st1)+x;
//        cy[1000-i] = r2*qSin(st1)+y;

        //Second Method
//        cx[2*i]=r1*qCos(st1)+x;
//        cy[2*i]=r1*qSin(st1)+y;
//        cx[2*i+1] = r2*qCos(st1)+x;
//        cy[2*i+1] = r2*qSin(st1)+y;

        //Third Method
        ct[i]=i; ct[i+1]=i+1;
        cx[i]=r1*qCos(st1)+x;
        cy[i]=r1*qSin(st1)+y;
        cx[i+1] = r2*qCos(st1)+x;
        cy[i+1] = r2*qSin(st1)+y;
        ct[i+length/2]=length-i; ct[i+length/2+1]=length-i-1;
        cx[i+length/2]=r1*qCos(st1)+x;
        cy[i+length/2]=-r1*qSin(st1)+y;
        cx[i+length/2+1] = r2*qCos(st1)+x;
        cy[i+length/2+1] = -r2*qSin(st1)+y;
    }
    static QCPCurve *c13=new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    static QCPCurve *c12= new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    static QCPCurve *c23= new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    static QCPCurve *c31=new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    static QCPCurve *c21= new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    static QCPCurve *c32= new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    QPen p12=QPen(QColor(255,0,0,100),3);
    QPen p13=QPen(QColor(0,255,0,100),3);
    QPen p23=QPen(QColor(0,0,255,100),3);
    c12->setPen(p12); c21->setPen(p12);
    c13->setPen(p13);c31->setPen (p13);
    c23->setPen(p23);c32->setPen (p23);
    switch (flag)
    {
    case 13:
        c13->setData (ct,cx,cy);
        break;
    case 12:
        c12->setData(ct,cx,cy);
        break;
    case 23:
        c23->setData(ct,cx,cy);
        break;
    case 31:
        c31->setData (ct,cx,cy);
        break;
    case 21:
        c21->setData(ct,cx,cy);
        break;
    case 32:
        c32->setData(ct,cx,cy);
        break;
    default:
        break;
    }
}

Widget::~Widget()
{
    delete ui;
}
