#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
}

bool Client::cnctTCP()
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(host,socket);
    return tcpSocket->waitForConnected();
}

void Client::sendUDP(QString file,bool crc)
{
    udpSocket = new QUdpSocket(this);
    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QByteArray ba,temp;
    ba = f.readAll();
    f.close();
    int b=0;
    cnctTCP();
    if(crc){

    }else {
        quint16 csum = qChecksum(ba.data(),ba.size());
        QByteArray csba;
        QVariant qv(csum);
        csba = qv.toByteArray();
        tcpSocket->write(csba);
    }
    int sz = ba.size();
    int datagram_sz=8192;
    int pos=0;
    while(pos < sz){
        if(sz-pos >= datagram_sz)
            temp = ba.mid(pos,datagram_sz);
        else
            temp = ba.mid(pos,sz-pos);
        b+=udpSocket->writeDatagram(temp.data(),temp.size(),host,socket);
        pos = pos + datagram_sz;
    }
    qDebug() << b << "bytes written";
    udpSocket->close();
    tcpSocket->close();
}

void Client::sendTCP(QString file)
{
    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QByteArray ba, temp;
    ba = f.readAll();
    tcpSocket->flush();
    qint64 b;
    b=tcpSocket->write(ba);
    qDebug()<<b<<" bytes written";
    tcpSocket->waitForBytesWritten();
    tcpSocket->flush();
    f.close();
    tcpSocket->close();
}

void Client::setAddress(QString h)
{
    host = QHostAddress(h);
}

void Client::setSocket(quint16 s)
{
    socket = s;
}

void Client::setCRC(bool b)
{
    crc=b;
}
