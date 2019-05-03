#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
}

bool Server::startTCP(bool sendTCP){
    tcpServer = new QTcpServer(this);
    if(sendTCP)
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(cnctdTCP()));
    else
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(checking()));
    if(!tcpServer->listen(QHostAddress::Any,socket)){
        delete (tcpServer);
        return false;
    }else
        return true;
}

bool Server::startUDP(){
    udpServer = new QUdpSocket(this);
    connect(udpServer,SIGNAL(readyRead()),this,SLOT(cnctdUDP()));
    if(!udpServer->bind(QHostAddress::Any,socket)){
        delete (udpServer);
        return false;
    }else{
        startTCP(false);
        return true;
    }
}

void Server::setAddress(QString h)
{
    host = QHostAddress::LocalHost;
}

void Server::setSocket(quint16 s)
{
    socket = s;
}

bool Server::confirm()
{
    return _confirm;
}

void Server::setCRC(bool b)
{
    crc=b;
}

void Server::cnctdTCP(){
    QTcpSocket *s = tcpServer->nextPendingConnection();
    s->waitForReadyRead(1);
    QByteArray data,temp;
    while(true){
         temp = s->read(1024);
         s->waitForReadyRead(5);
         if(temp.isEmpty()) break;
         data.append(temp);
    }
    qDebug()<<data.size()<<" bytes read";
    QFile f(QFileDialog::getSaveFileName(nullptr, tr("Save")));
    f.open(QIODevice::WriteOnly);
    f.write(data);
    s->close();
    f.close();
}

void Server::cnctdUDP(){
    QByteArray temp, data;
    QFile f(QFileDialog::getSaveFileName(nullptr, tr("Save")));
    QHostAddress sender;
    quint16 port;
    f.open(QIODevice::WriteOnly);
    quint16 csum=0;
    while(udpServer->hasPendingDatagrams()){
        temp.resize(udpServer->pendingDatagramSize());
        udpServer->readDatagram(temp.data(),temp.size(),&sender,&port);
        udpServer->waitForReadyRead(5);
        data.append(temp);
        csum += qChecksum(temp.data(),temp.size());
    }
    qDebug() << data.size() << " bytes read";
    f.write(data);
    if(crc){
        _confirm=false;
    }else {
        QVariant qv(csum);
        if(qv.toString()==check.data())
            _confirm=true;
        else
            _confirm=false;
    }
    emit(ended());
}

void Server::checking()
{
    QTcpSocket *s = tcpServer->nextPendingConnection();
    s->waitForReadyRead(3000);
    check = s->readAll();
    s->close();
}
