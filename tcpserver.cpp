#include "tcpserver.h"
#include<QNetworkInterface>
TcpServer::TcpServer(QObject *parent)
    : QObject{parent}
{
    connect(&server, &QTcpServer::newConnection, this, &TcpServer::onNewConnection);
    connect(this, &TcpServer::newMessage, this, &TcpServer::onNewMessage);
    if(server.listen(QHostAddress::Any, 40000))
    {
        qInfo()<<"listening";
    }
}

QString TcpServer::getPcIpAdresses() const
{
    QString allIps;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             allIps.append(address.toString() + '\n');
    }
    if(allIps.isEmpty())
    {
        return "Ip not found";
    }
    else
    {
        return allIps;
    }
}

void TcpServer::onNewConnection()
{
    const auto newClientSock = server.nextPendingConnection();
    if(!newClientSock)
    {
        return;
    }
    qInfo()<<"Client connected";
    connectedClients.insert(formClientKey(newClientSock), newClientSock);
    connect(newClientSock, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
    connect(newClientSock, &QTcpSocket::disconnected, this, &TcpServer::onDisconnected);
}

void TcpServer::onReadyRead()
{
    const auto clientSock = qobject_cast<QTcpSocket*>(sender());
    if(!clientSock)
    {
        return;
    }
    const auto message = formClientKey(clientSock).toUtf8() + ": " + clientSock->readAll();
    emit newMessage(message);
}

void TcpServer::onNewMessage(const QByteArray &message)
{
    for(const auto& client : connectedClients)
    {
        client->write(message);
        client->flush();
    }
}

void TcpServer::onDisconnected()
{
    const auto clientSock = qobject_cast<QTcpSocket*>(sender());
    if(!clientSock)
    {
        return;
    }
    connectedClients.remove(formClientKey(clientSock));
}

void TcpServer::onSendMessage(const QString &message)
{
    if(message.isEmpty())return;
    emit newMessage("Server: "+ message.toUtf8());
}

QString TcpServer::formClientKey(QTcpSocket *sock) const
{
    auto ip = sock->peerAddress().toString();
    ip.remove("::ffff:");
    return QString(ip + ':' + QString::number(sock->peerPort()));
}
