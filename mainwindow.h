#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QMovie>
#include <QTimer>
#include <random>
#include "client.h"
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sig(int);
    void on_fileButton_clicked();
    void on_ipButton_clicked();
    void on_udpButton_clicked();
    void on_tcpButton_clicked();
    void on_ipLine_textEdited(const QString &arg1);
    void on_socketBox_valueChanged(int arg1);
    void on_socketButton_clicked();
    void on_serverButton_clicked();
    void on_clientButton_clicked();
    void clicked(QAction*);
    void check();

private:
    Ui::MainWindow *ui;
    QDialog *d;
    QMovie *movie;
    QLabel *gif;
    Client *ct;
    Server *sv;
    quint16 s = 1024;
    QString h = "127.0.0.1";
};

#endif // MAINWINDOW_H
