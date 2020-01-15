#ifndef __LAYER_H__
#define __LAYER_H__
#include <QImage>
#include <QRect>
#include <QPainter>
#include <mutex>
#include "zsplugin.hpp"
namespace ZSS{
class Layer : public ZSPlugin{
public:
    Layer(const std::string& name);
    ~Layer();
    void resize(int,int);
    virtual void init() = 0;
    const QImage* const get() const{ return image; }
    void lock(){image_mutex.lock();}
    void unlock(){image_mutex.unlock();}
protected:
    QImage* image;
    QPainter painter;
    QRect area;
    std::mutex image_mutex;
};
}
#endif // __LAYER_H__
