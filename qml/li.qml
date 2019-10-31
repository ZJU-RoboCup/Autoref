import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import Li 1.0 as Li
ApplicationWindow {
    id:root
    visible: true
    width: 640
    height: 480
    title: qsTr("Autoref")
    Li.Interaction { id : interaction }
    Row{
        anchors.fill: parent
        ComboBox{
            id:themelist;
            model: interaction.availableTheme();
            Component.onCompleted: {
                var styleIndex = find(interaction.getTheme(), Qt.MatchFixedString)
                if (styleIndex !== -1)
                    currentIndex = styleIndex
            }
        }
        Button{
            text: "Change\\Restart"
            onClicked: {
                interaction.setTheme(themelist.currentText);
                interaction.restartApp();
            }
        }
    }
    Component.onCompleted: {
//        console.log()
    }
}
