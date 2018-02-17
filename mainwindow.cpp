#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(60000);
    this->tick();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_closeButton_clicked()
{
    this->hide();
}

void MainWindow::tick(){
    QProcess process;
    process.start("bash", QStringList() << "-c" << "upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep 'percentage' | tr -dc '0-9'");
    process.waitForFinished(-1);
    QProcess process2;
    process2.start("bash", QStringList() << "-c" << "upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep 'state'");
    process2.waitForFinished(-1);

    QString percentage = process.readAllStandardOutput();
    QString state = process2.readAllStandardOutput();

    if(percentage.toInt() < 10 && state.contains("discharging")){
        ui->message->setText("Le niveau de votre batterie est très faible ("+percentage+"%). Vous devriez brancher imédiatement l'alimentation de votre ordinateur");
        this->showFullScreen();
    }
}
