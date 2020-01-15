#ifndef FIELD_H
#define FIELD_H

#include <QQuickPaintedItem>
#include <QPixmap>
#include <QPainter>
#include <mutex>
#include "zslayer.h"
#include "singleton.hpp"
class FieldParam{
public:
    FieldParam();
    ~FieldParam();
    void reload();
public:
    int param_width;
    int param_height;
    int param_canvas_width;
    int param_canvas_height;
    int param_goalWidth;
    int param_goalDepth;
    int param_penaltyWidth;
    int param_penaltyLength;
    int param_centerCircleRadius;
    int param_lineWidth;
};
class FieldCoordsTransform{
public:
    FieldCoordsTransform();
    ~FieldCoordsTransform();
    void resize(int,int);
    double x(double _x);
    double y(double _y);
    QPointF p(QPointF& _p);
    double w(double _w);
    double h(double _h);
    double a(double _a);
    double r(double _r);
public:
    int canvasWidth;
    int canvasHeight;
    double zoomRatio;
    QPoint zoomStart;
};
typedef Singleton<FieldParam> FP;
typedef Singleton<FieldCoordsTransform> FT;

class LineLayer : public ZSS::Layer{
public:
    LineLayer();
    void init() override;
    void run() override;
private:
    void initFieldLinePath();
    QPainterPath fieldLinePath;
};

class Field : public QQuickPaintedItem{
    Q_OBJECT
public:
    Q_INVOKABLE void resetSize(int,int);
public:
    Field(QQuickItem *parent = 0);
    virtual ~Field();
    void paint(QPainter* painter) override;
    Q_INVOKABLE void repaint();
private:
    void init();
    QPixmap* pixmap;
    QPainter painter;
    QRect area;
    std::mutex pixmap_mutex;
    LineLayer linelayer;
};
#endif // FIELD_H
