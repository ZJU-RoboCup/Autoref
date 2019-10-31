import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import Li 1.0 as Li
ApplicationWindow {
    id:root
    visible: true
    width: 640
    height: 480
    title: qsTr("Autoref")
    Li.Interaction { id : interaction }
    menuBar:LMenuBar{interaction:interaction}
}
