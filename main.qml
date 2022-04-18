import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.12
Window {
    width: 640
    height: 480
    minimumHeight: 280
    minimumWidth: 440
    visible: true
    title: qsTr("Chat Server")

    Connections
    {
        target: server

        function onNewMessage(message)
        {
            listModelMessages.append({message: message + ""});
        }
    }

    ColumnLayout
    {
        anchors.fill: parent
        Text
        {
            text: "IP адреса компьютера: \n" + server.getPcIpAdresses();
        }

        ListView
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            model: ListModel
            {
                id: listModelMessages
                ListElement
                {
                    message: "Добро пожаловать в сервер чата!";
                }
            }
            delegate: ItemDelegate
            {
                text: message
            }
            ScrollBar.vertical: ScrollBar{}
        }
        RowLayout
        {
            TextField
            {
                id: textFieldMessage
                placeholderText: qsTr("Ваше сообщение: ")
                Layout.fillWidth: true
                onAccepted: buttonSend

            }
            Button
            {
                id: buttonSend
                text: qsTr("Отправить")
                onClicked:
                {
                    server.onSendMessage(textFieldMessage.text)
                    textFieldMessage.clear()
                }
            }
        }
    }

}
