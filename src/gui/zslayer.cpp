#include "zslayer.h"
#include <QtDebug>
namespace ZSS{
Layer::Layer(const std::string& name):ZSPlugin(name){
    image = new QImage(QSize(200, 300),QImage::Format_ARGB32_Premultiplied);
    painter.begin(image);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
}
Layer::~Layer(){
}
void Layer::resize(int w,int h){
    if(w == 0 || h == 0) return;
    image_mutex.lock();
    qDebug() << "layer resize function : " << w << h;
    if(painter.isActive())
        painter.end();
    delete image;
    image = new QImage(QSize(w,h),QImage::Format_ARGB32_Premultiplied);
    painter.begin(image);
    area = QRect(0,0,w,h);
    init();
    image_mutex.unlock();
}
} // namespace ZSS
