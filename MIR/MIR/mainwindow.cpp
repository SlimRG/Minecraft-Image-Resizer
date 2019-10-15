#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDirIterator>
#include <thread>
#include <QThread>
#include <mutex>
#include <QtGui>
#include <QColorDialog>

#include "private/qzipreader_p.h"
#include "private/qzipwriter_p.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // При запуске
    ui->spinBox->setValue(QThread::idealThreadCount());
    ui->spinBox->setMaximum(QThread::idealThreadCount());

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(this,
                                   QString::fromUtf8("Open file"),
                                   QDir::currentPath(),
                                   "Source Images (*.zip)"));
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    switch (value)
    {
        case (0):
            ui->label_5->setText("Auto");
            ui->groupBox->setEnabled(false);
            if ((ui->pushButton_2->geometry().x() / 1000) == 0) ui->pushButton_2->setGeometry(
                    ui->pushButton_2->geometry().x() * 1000,
                    ui->pushButton_2->geometry().y(),
                    ui->pushButton_2->geometry().width(),
                    ui->pushButton_2->geometry().height()
                    );
            break;

        case (1):
            ui->label_5->setText("Color with Alpha");
            ui->groupBox->setEnabled(false);
            if ((ui->pushButton_2->geometry().x() / 1000) == 0) ui->pushButton_2->setGeometry(
                    ui->pushButton_2->geometry().x() * 1000,
                    ui->pushButton_2->geometry().y(),
                    ui->pushButton_2->geometry().width(),
                    ui->pushButton_2->geometry().height()
                    );
            break;

        case (2):
            ui->label_5->setText("Color without Alpha");
            ui->groupBox->setEnabled(true);
            if ((ui->pushButton_2->geometry().x() / 1000) != 0) ui->pushButton_2->setGeometry(
                    ui->pushButton_2->geometry().x() / 1000,
                    ui->pushButton_2->geometry().y(),
                    ui->pushButton_2->geometry().width(),
                    ui->pushButton_2->geometry().height()
                    );
            break;

        case (3):
            ui->label_5->setText("Grayscale");
            ui->groupBox->setEnabled(false);
            if ((ui->pushButton_2->geometry().x() / 1000) == 0) ui->pushButton_2->setGeometry(
                    ui->pushButton_2->geometry().x() * 1000,
                    ui->pushButton_2->geometry().y(),
                    ui->pushButton_2->geometry().width(),
                    ui->pushButton_2->geometry().height()
                    );
            break;

        case (4):
            ui->label_5->setText("Black & White");
            ui->groupBox->setEnabled(false);
            if ((ui->pushButton_2->geometry().x() / 1000) == 0) ui->pushButton_2->setGeometry(
                    ui->pushButton_2->geometry().x() * 1000,
                    ui->pushButton_2->geometry().y(),
                    ui->pushButton_2->geometry().width(),
                    ui->pushButton_2->geometry().height()
                    );
            break;
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    QColor color = QColorDialog::getColor(Qt::magenta);
    ui->pushButton_2->setStyleSheet("background-color:  rgb(" + QString::number(color.red()) + ", "+QString::number(color.green())+", "+QString::number(color.blue())+")");
}

void MainWindow::on_pushButton_3_clicked()
{
    // Переменная ошибки
    bool exit = false;

    // Визуальный переход
    ui->spinBox->setEnabled(false);
    ui->spinBox_2->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->pushButton_3->setVisible(false);

    if ((ui->horizontalSlider->value())&&((ui->pushButton_2->geometry().x() / 1000) == 0)) ui->pushButton_2->setGeometry(
            ui->pushButton_2->geometry().x() * 1000,
            ui->pushButton_2->geometry().y(),
            ui->pushButton_2->geometry().width(),
            ui->pushButton_2->geometry().height()
            );

    // Открываем файл
       QZipReader zip_reader(ui->lineEdit->text());
       if (!exit){
           if (!zip_reader.exists()){
               QMessageBox::warning(this, "Warning","Can't open ZIP file!");
               exit = true;
           }
       }
       ui->progressBar->setValue(10);

    // Создаем временную папку
       QString MTEPathTMP = QDir::tempPath()+"/MTE99";
       if (!exit){
           QDir dir;
           if((!dir.mkpath(MTEPathTMP)) || (!dir.mkpath(MTEPathTMP+"/ZIP")||(!dir.mkpath(MTEPathTMP+"/SRC")))){
               QMessageBox::warning(this, "Warning","Can't create temp folder!");
               exit = true;
           }
       }
       ui->progressBar->setValue(15);

    // Распаковываем в неё файлы
       if (!exit){
           zip_reader.extractAll(MTEPathTMP+"/SRC");
           if (zip_reader.status() != QZipReader::NoError){
               QMessageBox::warning(this, "Warning","Can't unpack ZIP file!");
               exit = true;
           }
       }
       ui->progressBar->setValue(30);


}
