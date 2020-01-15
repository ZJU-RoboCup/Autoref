import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import Li 1.0 as Li
ApplicationWindow {
    id:root
    visible: true
    width: 640
    height: 480
    minimumHeight: 100;
    minimumWidth: 100;
    title: qsTr("Autoref")
    Timer{
        id:fpsTimer;
        interval:16;
        running:true;
        repeat:true;
        onTriggered: {
            field.repaint();
        }
    }
    Li.Interaction { id : interaction }
    Li.Field{
        id:field;
        width:parent.width;
        height:parent.height;
        x:0;//parent.width;
        y:0;//parent.height;
        function resize(){ resetSize(width,height); }
        onWidthChanged: resize();
        onHeightChanged: resize();
    }
    menuBar:LMenuBar{interaction:interaction}
}
