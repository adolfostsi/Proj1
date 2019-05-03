#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    d = new QDialog(this);
    connect(ui->help, SIGNAL(triggered(QAction*)), this, SLOT(clicked(QAction*)));

    //img configuration
    QPixmap pix1(":/img/imgs/client_server.jpeg");
    pix1=pix1.scaled(390,240);
    ui->imgLabel->setPixmap(pix1);
    QPixmap pix2(":/img/imgs/sign-check-icon.png");
    pix2=pix2.scaled(25,25);
    ui->checkLabel->setPixmap(pix2);
    ui->socketLabel->setPixmap(pix2);
    //setting
    ui->checksumButton->click();
    ui->tcpButton->click();
    ui->textClient->setReadOnly(true);
    ui->textServer->setReadOnly(true);
    ui->fileLine->setReadOnly(true);
    ui->ipLine->setText("127.0.0.1");
    ui->tcpButton->click();
    d->setWindowTitle("Transferindo");
    d->setFixedSize(300,300);
    gif = new QLabel(d);

    ct = new Client(this);
    sv = new Server(this);
    ui->checksumButton->setEnabled(false);
    ui->crcButton->setEnabled(false);
    connect(sv,SIGNAL(ended()),this,SLOT(check()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ct;
    delete sv;
}

void MainWindow::sig(int frameNumber)
{
    if(frameNumber == (movie->frameCount()-1)) {
        movie->stop();
        d->close();
        if(ui->udpButton->isChecked()){
            if(ui->crcButton->isChecked()){
                ct->setCRC(true);
                ct->sendUDP(ui->fileLine->text(),true);

            }else{
                ct->setCRC(false);
                ct->sendUDP(ui->fileLine->text(),false);
            }
            ui->textClient->append("Arquivo transferido");
        }else{
            if(ct->cnctTCP()){
                ct->sendTCP(ui->fileLine->text());
                ui->textClient->append("Arquivo transferido");
            }else
                ui->textClient->append("Conexão não pode ser estabelecida");
        }
    }
}


void MainWindow::on_fileButton_clicked()
{
    ui->fileLine->setReadOnly(false);
    QString filename=QFileDialog::getOpenFileName(this,tr("Open File"),".","All Files (*.*)");
    ui->fileLine->setText(filename);
    ui->fileLine->setReadOnly(true);
}

void MainWindow::on_ipButton_clicked()
{
    QPixmap pix(":/img/imgs/sign-check-icon.png");
    pix=pix.scaled(25,25);
    ui->checkLabel->setPixmap(pix);
    h = ui->ipLine->text();
}

void MainWindow::on_ipLine_textEdited(const QString &arg1)
{
    QPixmap pix(":/img/imgs/atention.png");
    pix=pix.scaled(25,25);
    ui->checkLabel->setPixmap(pix);
}

void MainWindow::on_socketBox_valueChanged(int arg1)
{
    QPixmap pix(":/img/imgs/atention.png");
    pix=pix.scaled(25,25);
    ui->socketLabel->setPixmap(pix);
}

void MainWindow::on_socketButton_clicked()
{
    QPixmap pix(":/img/imgs/sign-check-icon.png");
    pix=pix.scaled(25,25);
    ui->socketLabel->setPixmap(pix);
    s = (quint16)ui->socketBox->value();
}

void MainWindow::on_udpButton_clicked()
{
    ui->checksumButton->setEnabled(true);
    ui->crcButton->setEnabled(true);
}

void MainWindow::on_tcpButton_clicked()
{
    ui->checksumButton->setEnabled(false);
    ui->crcButton->setEnabled(false);
}

void MainWindow::on_serverButton_clicked()
{
    ui->serverButton->hide();
    ui->textServer->setReadOnly(false);
    sv->setSocket(s);
    sv->setAddress(h);
    if(ui->udpButton->isChecked()){
        if(ui->crcButton->isChecked()){
            sv->setCRC(true);
        }else {
            sv->setCRC(false);
        }
        if(!sv->startUDP()){
            ui->textServer->setText("Impossível iniciar o server com este IP");
        }else {
            ui->textServer->setText("Server UDP iniciado");
            ui->textServer->append("Porta escolhida: " + QString::number(s));
        }
    }else{
        if(!sv->startTCP(true)){
            ui->textServer->setText("Impossível iniciar o server");
        }else {
            ui->textServer->setText("Server TCP iniciado");
            ui->textServer->append("Porta escolhida: " + QString::number(s));
        }
    }
    ui->serverButton->show();
    ui->textServer->setReadOnly(true);
}

void MainWindow::on_clientButton_clicked()
{
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 3);
    int mean = uniform_dist(e1);
    switch(mean){
        case 3:
            movie = new QMovie(":/img/imgs/giphy.gif",QByteArray(),gif);
            break;
        case 1:
            movie = new QMovie(":/img/imgs/gif2.gif",QByteArray(),gif);
            break;
        case 2:
            movie = new QMovie(":/img/imgs/pizza.gif",QByteArray(),gif);
            break;
    }
    movie->setScaledSize(d->size());
    gif->resize(d->size());
    gif->setMovie(movie);
    connect(movie,SIGNAL(frameChanged(int)),this,SLOT(sig(int)));

    ct->setSocket(s);
    ct->setAddress(h);
    ui->textClient->setReadOnly(false);
    if(ui->fileLine->text() == ""){
        QMessageBox msgbox;
        msgbox.setWindowTitle("Atenção");
        msgbox.setText("Nenhum arquivo selecionado");
        msgbox.exec();
    }else if(ui->udpButton->isChecked()){
        ui->textClient->setText("Iniciando transmissão utilizando UDP");
        d->show();
        movie->start();

    }else{
        ui->textClient->setText("Iniciando conexão com servidor");
        ui->textClient->append("Iniciando transmissão utilizando TCP");
        d->show();
        movie->start();
    }
}

void MainWindow::clicked(QAction*)
{
    QMessageBox q;
    q.setWindowTitle("HELP");
    q.setText("Este programa serve para enviar arquivos utilizando protocolos de comunicação UDP e TCP\n"
              "Para carregar o arquivo utilize o botão Open File. Selecione um arquivo em seu computador e carregue. Escolha o protocolo de comunicação, o método de checagem caso seu protocolo seja UDP"
              " e então clique em Start Server e em seguida em Start Client. Ao final da transferência a janela aberta permitirá escolher o nome e onde você deseja salvar o arquivo transferido.");
    q.exec();
}

void MainWindow::check()
{
    if(ui->crcButton->isChecked()){
        if(sv->confirm())
            ui->textClient->append("CRC confirmado");
        else
            ui->textClient->append("CRC não confirmado");
    }else{
        if(sv->confirm())
            ui->textClient->append("Checksum confirmado");
        else
            ui->textClient->append("Checksum não confirmado");
    }
}
