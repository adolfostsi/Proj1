#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QByteArray>
#include <QFileDialog>
#include <QThread>
#include <QVariant>
#include <QFile>
#include <QDebug>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startUDP();
    bool startTCP(bool);
    void setAddress(QString);
    void setSocket(quint16);
    bool confirm();
    void setCRC(bool);

signals:
    void ended();

public slots:
    void cnctdTCP();
    void cnctdUDP();
    void checking();

private:
    QUdpSocket *udpServer;
    QTcpServer *tcpServer;
    QHostAddress host = QHostAddress::LocalHost;
    quint16 socket = 1024;
    QByteArray check;
    bool _confirm=true;
    bool crc;
};

#endif // SERVER_H
