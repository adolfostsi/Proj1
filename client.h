#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QTcpSocket>
#include <QUdpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    bool cnctTCP();
    void sendUDP(QString,bool);
    void sendTCP(QString);
    void setAddress(QString);
    void setSocket(quint16);
    void setCRC(bool);

signals:

public slots:

private:
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    QHostAddress host = QHostAddress::LocalHost;
    quint16 socket = 1024;
    bool crc;
};

#endif // CLIENT_H
