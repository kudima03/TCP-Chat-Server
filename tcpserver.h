#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

class TcpServer : public QObject
{
    Q_OBJECT
protected:

    QTcpServer server;
    QHash<QString, QTcpSocket*> connectedClients;
    QString formClientKey(QTcpSocket* sock) const;

public:
    explicit TcpServer(QObject *parent = nullptr);


signals:

    void newMessage(const QByteArray& message);

public slots:
    void onSendMessage(const QString& message);
    QString getPcIpAdresses()const;

protected slots:
    void onNewConnection();
    void onReadyRead();
    void onNewMessage(const QByteArray& message);
    void onDisconnected();


};

#endif // TCPSERVER_H
