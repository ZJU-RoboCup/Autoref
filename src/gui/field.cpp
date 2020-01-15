#include "field.h"
#include "parammanager.h"
#include <QElapsedTimer>
#include <QtDebug>
#include <cmath>
namespace{
    auto zpm = ZSS::LParamManager::instance();
    auto p = FP::instance();
    auto t = FT::instance();
}
FieldParam::FieldParam(){
    reload();
}
FieldParam::~FieldParam(){}
void FieldParam::reload(){
    zpm->loadParam(param_width, "field/width", 6000);
    zpm->loadParam(param_height, "field/height", 4500);
    zpm->loadParam(param_canvas_width, "field/canvasWidth", 6600);
    zpm->loadParam(param_canvas_height, "field/canvasHeight", 4950);
    zpm->loadParam(param_goalWidth, "field/goalWidth", 800);
    zpm->loadParam(param_goalDepth, "field/goalDepth",  200);
    zpm->loadParam(param_penaltyWidth, "field/penaltyWidth", 800);
    zpm->loadParam(param_penaltyLength, "field/penaltyLength", 1600);
    zpm->loadParam(param_centerCircleRadius, "field/centerCircleRadius",  500);
    zpm->loadParam(param_lineWidth,"field/lineWidth",15);
}
FieldCoordsTransform::FieldCoordsTransform():zoomRatio(1){}
FieldCoordsTransform::~FieldCoordsTransform(){}
void FieldCoordsTransform::resize(int w,int h){
    double ratio = double(::p->param_canvas_width)/::p->param_canvas_height;
    canvasWidth = std::min(w,int(h*ratio));
    canvasHeight = std::min(int(w/ratio),h);
    zoomStart.setX((canvasWidth-w)/2);
    zoomStart.setY((canvasHeight-h)/2);
}
double FieldCoordsTransform::x(double _x) {
    return (_x * canvasWidth / ::p->param_canvas_width + canvasWidth / 2.0 - zoomStart.x()) / zoomRatio;
}
double FieldCoordsTransform::y(double _y) {
    return (-_y * canvasHeight / ::p->param_canvas_height + canvasHeight / 2.0 - zoomStart.y()) / zoomRatio;
}
QPointF FieldCoordsTransform::p(QPointF& _p) {
    return QPointF(x(_p.x()), y(_p.y()));
}
double FieldCoordsTransform::w(double _w) {
    return _w * canvasWidth / ::p->param_canvas_width / zoomRatio;
}
double FieldCoordsTransform::h(double _h) {
    return -_h * canvasHeight / ::p->param_canvas_height / zoomRatio;
}
double FieldCoordsTransform::a(double _a) {
    return _a * 16;
}
double FieldCoordsTransform::r(double _r) {
    return _r * 16;
}
Field::Field(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    ,pixmap(nullptr){
    pixmap = new QPixmap();
    painter.begin(pixmap);
    linelayer.start();
    linelayer.detech();
    init();
}
Field::~Field(){}
void Field::paint(QPainter* painter){
    if(pixmap_mutex.try_lock()){
        painter->drawPixmap(area, *pixmap);
        pixmap_mutex.unlock();
    }
}
void Field::init(){
    painter.setPen(QPen(QBrush(QColor(255,255,255)),20));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
}
void Field::repaint(){
    if(pixmap_mutex.try_lock()){
        pixmap->fill(QColor(100,0,100));//COLORTODO
        linelayer.lock();
        painter.drawImage(0,0,*linelayer.get());
        linelayer.unlock();
        pixmap_mutex.unlock();
        this->update(area);
    }
}
// Q_INVOKABLE Function
void Field::resetSize(int width,int height){
    if (width == 0 || height == 0) return;
    t->resize(width,height);
    linelayer.resize(width,height);
    pixmap_mutex.lock();
    {
        qDebug() << "field resize function : " << width << height;
        if(painter.isActive())
            painter.end();
        delete pixmap;
        pixmap = new QPixmap(QSize(this->property("width").toReal(), this->property("height").toReal()));
        painter.begin(pixmap);
        area = QRect(0, 0, this->property("width").toReal(), this->property("height").toReal());
        init();
    }
    pixmap_mutex.unlock();
}
LineLayer::LineLayer():ZSS::Layer("FieldLine"){
}
void LineLayer::init(){
    painter.setPen(QPen(QBrush(QColor(150, 150, 150)),t->w(p->param_lineWidth)));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    initFieldLinePath();
}
void LineLayer::run(){
    init();
    while(true){
        if(image_mutex.try_lock()){
            image->fill(QColor(48,48,48));
//            painter.drawLine(t->x(-100),t->y(-100),t->x(100),t->y(100));
            painter.drawPath(fieldLinePath);
            image_mutex.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void LineLayer::initFieldLinePath(){
    fieldLinePath.clear();
    fieldLinePath.addRect(t->x(-p->param_width / 2.0), t->y(-p->param_height / 2.0), t->w(p->param_width), t->h(p->param_height));
    fieldLinePath.addRect(t->x(-p->param_width / 2.0), t->y(-p->param_goalWidth / 2.0), t->w(-p->param_goalDepth), t->h(p->param_goalWidth));
    fieldLinePath.addRect(t->x(p->param_width / 2.0), t->y(-p->param_goalWidth / 2.0), t->w(p->param_goalDepth), t->h(p->param_goalWidth));
    fieldLinePath.moveTo(t->x(0), t->y(p->param_height / 2.0));
    fieldLinePath.lineTo(t->x(0), t->y(-p->param_height / 2.0));
    fieldLinePath.addEllipse(t->x(-p->param_centerCircleRadius), t->y(-p->param_centerCircleRadius), t->w(2 * p->param_centerCircleRadius), t->h(2 * p->param_centerCircleRadius));
    fieldLinePath.addRect(t->x(-p->param_width / 2.0), t->y(-p->param_penaltyLength / 2.0), t->w(p->param_penaltyWidth), t->h(p->param_penaltyLength));
    fieldLinePath.addRect(t->x(p->param_width / 2.0), t->y(-p->param_penaltyLength / 2.0), t->w(-p->param_penaltyWidth), t->h(p->param_penaltyLength));
}
