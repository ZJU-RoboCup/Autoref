#include "field.h"
#include "field_layers.h"
#include "parammanager.h"
#include "setthreadname.h"
#include "visionmodule.h"
#include "vision_detection.pb.h"
namespace {
    auto zpm = ZSS::LParamManager::instance();
    auto p = FP::instance();
    auto t = FT::instance();
}
void Field::addLayers(){
    _layers.push_back(new FieldLineLayer());
    _layers.push_back(new VisionLayer());
}

FieldLineLayer::FieldLineLayer():ZSS::Layer("FieldLineLayer"){
    _controls.push_back(std::unique_ptr<ZSS::ControlVariable>(new ZSS::ControlVariable("field_lines")));
}
FieldLineLayer::~FieldLineLayer(){
    #ifdef ZSPLUGIN_DEBUG
    std::cout << this << " FieldLineLayer destructor" << std::endl;
    #endif
}
void FieldLineLayer::init(){
    _painter.setPen(QPen(QBrush(QColor(150, 150, 150)),t->w(p->param_lineWidth)));
    initFieldLinePath();
}
void FieldLineLayer::run(){
    SetThreadName("FieldLineLayer");
    init();
    while(true){
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        receive("draw_signal");
        if(needExit) break;
        if(_image_mutex.try_lock()){
            _image->fill(Qt::transparent);
            _painter.drawPath(fieldLinePath);
            _image_mutex.unlock();
        }
    }
    qDebug() << "FieldLineLayer:run() safe exit!";
}
VisionLayer::VisionLayer():ZSS::Layer("VisionLayer"){
    _controls.push_back(std::unique_ptr<ZSS::ControlVariable>(new ZSS::ControlVariable("origin_vision")));
    _controls.push_back(std::unique_ptr<ZSS::ControlVariable>(new ZSS::ControlVariable("fusion_vision")));
    declare_receive("zss_vision");
    VisionModule::instance()->link(this,"zss_vision");
}
VisionLayer::~VisionLayer(){
    #ifdef ZSPLUGIN_DEBUG
    std::cout << this << " VisionLayer destructor" << std::endl;
    #endif
}
void VisionLayer::init(){
    _painter.setPen(QPen(QBrush(QColor(150, 150, 150)),t->w(p->param_lineWidth)));
}
void VisionLayer::run(){
    SetThreadName("VisionLayer");
    init();
    ZSData data;
    Vision_DetectionFrame frame;
    while(true){
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        receive("zss_vision",data);
        if(needExit) break;
        frame.ParseFromArray(data.data(),data.size());
        if(_image_mutex.try_lock()){
            _image->fill(Qt::transparent);
            if(_controls[0]->sw){
                drawOriginVision(frame);
            }
            if(_controls[1]->sw){
                drawFusionVision(frame);
            }
            _image_mutex.unlock();
        }
    }
    qDebug() << "VisionLayer:run() safe exit!";
}
