#include "field.h"
#include "field_layers.h"
#include "field_style.h"
#include "parammanager.h"
#include <QElapsedTimer>
#include <QtDebug>
#include <cmath>
namespace{
    auto zpm = ZSS::LParamManager::instance();
    auto p = FP::instance();
    auto t = FT::instance();
    auto s = FS::instance();
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
    zpm->loadParam(size_ballDiameter, "size/ballDiameter", 100);
    zpm->loadParam(size_shadowDiameter, "size/shadowDiameter", 30);
    zpm->loadParam(size_carDiameter, "size/carDiameter", 180);
    zpm->loadParam(size_carFaceWidth, "size/carFaceWidth", 120);
    zpm->loadParam(size_number, "size/numberSize", 200);
    zpm->loadParam(size_debugPoint, "size/debugPointSize", 5);
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
    , ZSPlugin("FieldBase")
    ,_pixmap(nullptr){
    declare_publish("draw_signal");
    addLayers();
    for(auto _l : _layers){
        this->link(_l,"draw_signal");
    }

    int width,height;
    zpm->loadParam(width,"auto/fieldWidth",500);
    zpm->loadParam(height,"auto/fieldHeight",350);
    resetSize(width,height,false);
    for(auto _l : _layers){
        _l->start_detach();
    }
}
Field::~Field(){
    for(auto _l : _layers)
        _l->prepareExit();
    for(auto _l : _layers)
        delete _l;
}
void Field::paint(QPainter* painter){
    if(_pixmap_mutex.try_lock()){
        painter->drawPixmap(_area, *_pixmap);
        _pixmap_mutex.unlock();
    }
}
void Field::init(){
    _painter.setPen(QPen(QBrush(QColor(255,255,255)),20));
    _painter.setRenderHint(QPainter::Antialiasing, true);
    _painter.setRenderHint(QPainter::TextAntialiasing, true);
}
void Field::repaint(){
    if(_pixmap_mutex.try_lock())
    {
        _pixmap->fill(s->COLOR_FIELD);//COLORTODO
        for(auto _l : _layers){
            _l->lock();
            _painter.drawImage(0,0,*(_l->get()));
            _l->unlock();
        }
        _pixmap_mutex.unlock();
        this->update(_area);
    }
}
// Q_INVOKABLE Function
void Field::resetSize(int width,int height,bool update){
    if (width <= 0 || height <= 0) return;
    if (width == _area.width() && height == _area.height()) return;
    t->resize(width,height);
    for(auto _l : _layers){
        _l->resize(width,height);
    }
    publish("draw_signal");
    if(update){
        zpm->changeParam("auto/fieldWidth",width);
        zpm->changeParam("auto/fieldHeight",height);
    }
    _pixmap_mutex.lock();
    {
        qDebug() << "\"fieldBase\" resize function : " << width << height;
        if(_painter.isActive())
            _painter.end();
        delete _pixmap;
        _pixmap = new QPixmap(QSize(width,height));
        _pixmap->fill(s->COLOR_FIELD);
        _painter.begin(_pixmap);
        _area = QRect(0, 0, width, height);
        init();
    }
    _pixmap_mutex.unlock();
}
