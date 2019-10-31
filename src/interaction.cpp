#include <QQuickStyle>
#include <QGuiApplication>
#include <QProcess>
#include <QtDebug>
#include "interaction.h"
#include "parammanager.h"
Interaction::Interaction(QObject *parent) : QObject(parent){
}
void Interaction::setTheme(QString theme){
    qDebug() << "change theme : " << theme;
    ZSS::LParamManager::instance()->changeParam("theme/name",QVariant(theme));
}
QString Interaction::getTheme(){
    return QQuickStyle::name();
}
void Interaction::restartApp(){
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
QStringList Interaction::availableTheme(){
    return QQuickStyle::availableStyles();
}
