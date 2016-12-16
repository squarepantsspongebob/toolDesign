#include "widget.h"
#include <QDebug>
#include "ui_widget.h"

#define PI 3.1415926

Widget::Widget(QWidget *parent) :
    QWidget(parent),p1x(1),p1y(1),p2x(1),p2y(1),p3x(1),p3y(1),
    p4x(1),p4y(1),pointLine(6),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |QCP::iSelectLegend);
    ui->widget->xAxis->setRange(-150,150);
    ui->widget->yAxis->setRange(-150,150);
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    //Initialize the pointLine
    for(int i=0;i<6;i++)
    {
        pointLine[i] = new QCPItemLine(ui->widget);
    }

    p1x[0]=0.0; p1y[0]=35;p2x[0]=0.0;p2y[0]=-35;
    double r12=p1y[0]-p2y[0];
    ui->P1->setText("(0,35)");ui->P2->setText("(0,-35)");
    ui->P3->setText("(50,50)");

    //c121 represents the circle using p1 as centroid, r12-3.5 as radius.
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

    // create graph and assign data to it:
    QCPScatterStyle pointStyle = QCPScatterStyle(QCPScatterStyle::ssDisc,10);
    QPen pointPen= QPen(QColor(255,0,0,255),10);
    ui->widget->addGraph();//0
    ui->widget->graph(0)->setScatterStyle(pointStyle);
    ui->widget->graph(0)->setData(p1x, p1y);
    ui->widget->addGraph();//1
    ui->widget->graph(1)->setScatterStyle(pointStyle);
    ui->widget->graph(1)->setData(p2x, p2y);
    ui->widget->addGraph ();//2
    ui->widget->graph(2)->setScatterStyle(pointStyle);
    drawHyperbola(p1x[0], p1y[0],p2x[0],p2y[0],12);
    drawLine(p1x,p1y,p2x,p2y,0);

    //radioButton initialization
    ui->rb3->setChecked (true);
//    connect signal and slot
    QObject::connect(ui->p3xEdit, SIGNAL(textChanged()), this, SLOT(draw3()));
    QObject::connect(ui->p3yEdit, SIGNAL(textChanged()), this, SLOT(draw3()));
    QObject::connect(ui->p4xEdit, SIGNAL(textChanged()), this, SLOT(draw4()));
    QObject::connect(ui->p4yEdit, SIGNAL(textChanged()), this, SLOT(draw4()));
    QObject::connect(ui->widget, SIGNAL(mouseDoubleClick(QMouseEvent*)), this,SLOT(doubleClickPoint(QMouseEvent*)));
    QObject::connect(ui->widget,SIGNAL(mouseMove(QMouseEvent*)),this, SLOT(movePoint(QMouseEvent *)));
}

void Widget::drawLine (const QVector<double> &ax, const QVector<double> &ay, const QVector<double> &bx,const QVector<double> &by, int flag)
{
    pointLine[flag]->start->setCoords (ax[0],ay[0]);
    pointLine[flag]->end->setCoords (bx[0],by[0]);
}

void Widget::draw4()
{

}

void Widget::movePoint (QMouseEvent *event)
{
    QPoint cursor_pos = event->pos();
    double x = ui->widget->xAxis->pixelToCoord(event->pos().x());
    double y = ui->widget->yAxis->pixelToCoord(event->pos().y());
    ui->mouse_x->setText (QString::number(x,'f',4));
    ui->mouse_y->setText(QString::number(y,'f',4));
    static int count=0;
    count += 1;
    qDebug("%d",count);
    //    setToolTip(QString("%1 , %2").arg(x).arg(y));
}

void Widget::doubleClickPoint(QMouseEvent *event)
{
    QPoint cursor_pos = event->pos();
    double x = ui->widget->xAxis->pixelToCoord(event->pos().x());
    double y = ui->widget->yAxis->pixelToCoord(event->pos().y());

    QPalette disableColor;
    disableColor.setColor(QPalette::WindowText,Qt::red);
    QPalette enableColor;
    enableColor.setColor(QPalette::WindowText,Qt::green);

    if (ui->rb3->isChecked ()){
        //hide points and lines
        pointLine[2]->setVisible (false);pointLine[4]->setVisible (false);pointLine[5]->setVisible (false);

        ui->P3->setText ("("+QString::number(x,'f',1)+","+QString::number(y,'f',1)+")");
        ui->p3xEdit->setText(QString::number(x,'f',1));
        ui->p3yEdit->setText(QString::number(y,'f',1));
        p3x[0] = x; p3y[0]=y;
        length12=qSqrt(qPow(p1x[0]-p2x[0],2)+qPow(p1y[0]-p2y[0],2));
        length13=qSqrt(qPow(p1x[0]-p3x[0],2)+qPow(p1y[0]-p3y[0],2));
        length23=qSqrt(qPow(p3x[0]-p2x[0],2)+qPow(p3y[0]-p2y[0],2));
        ui->l12->setText (QString::number(length12));
        ui->l13->setText (QString::number(length13));
        ui->l23->setText (QString::number(length23));
        if (qAbs(length12-length13)<3.5 || qAbs(length12-length23)<3.5 ||qAbs(length13-length23)<3.5)
        {
            ui->P3->setPalette(disableColor);
        }
        else
        {
            ui->P3->setPalette(enableColor);
        }
    }
    else{
        ui->P4->setText ("("+QString::number(x,'f',1)+","+QString::number(y,'f',1)+")");
        ui->p4xEdit->setText(QString::number(x,'f',1));
        ui->p4yEdit->setText(QString::number(y,'f',1));
        p4x[0] = x; p4y[0]=y;
        length14=qSqrt(qPow(p1x[0]-p4x[0],2)+qPow(p1y[0]-p4y[0],2));
        length24=qSqrt(qPow(p2x[0]-p4x[0],2)+qPow(p2y[0]-p4y[0],2));
        length34=qSqrt(qPow(p3x[0]-p4x[0],2)+qPow(p3y[0]-p4y[0],2));
        ui->l14->setText (QString::number(length14));
        ui->l24->setText (QString::number(length24));
        ui->l34->setText (QString::number(length34));
        pointLine[2]->setVisible (true);pointLine[4]->setVisible (true);pointLine[5]->setVisible (true);
        drawLine(p1x,p1y,p4x,p4y,2);
        drawLine(p2x,p2y,p4x,p4y,4);
        drawLine(p3x,p3y,p4x,p4y,5);
        ui->widget->replot();
    }
    if (qAbs(length14-length24)<3.5 || qAbs(length14-length34)<3.5 ||qAbs(length24-length34)<3.5 || qAbs(length14-length12)<3.5 ||qAbs(length14-length13)<3.5 ||qAbs(length14-length23)<3.5 ||qAbs(length24-length12)<3.5 ||qAbs(length24-length13)<3.5 ||qAbs(length24-length23)<3.5 || qAbs(length34-length12)<3.5 ||qAbs(length34-length13)<3.5 ||qAbs(length34-length23)<3.5)
    {
        ui->P4->setPalette(disableColor);
    }
    else
    {
        ui->P4->setPalette(enableColor);
    }
}

void Widget::draw3()
{
    draw23();
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
//    threePoint->setData(tPx,tPy);
    drawLine (p2x,p2y,p3x,p3y,3);
    drawLine(p1x,p1y,p3x,p3y,1);
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
