import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import mummesoft 0.1

Label {
    id: root
    property var pid
    default property var format: function(pid) {
        return pid ? pid.valueStr : "";
    }
    Connections {
        target: pid
        onValueChanged: {
            root.text = root.format(root.pid)
        }
    }
    Component.onCompleted: {
        root.text = root.format(root.pid);
    }
}
