#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(BluetoothProxy *_blue_window,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    blue_window = _blue_window;
    run_thread = new RunThread();
    run_thread->blue_window = blue_window;
    run_thread->start();
    choose_l_r = true;
     ui->button_left->setStyleSheet("QPushButton{background-color: rgb(20, 62, 134);border:none;color:rgb(255, 255, 255);}");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_blue_clicked()
{
     blue_window->showFullScreen();
}

void MainWindow::setTextXYZ()
{
    if(choose_l_r)
    {
        ui->textEdit_x->setText(QString("%1").arg(run_thread->arm1.get_x()*1000));
        ui->textEdit_y->setText(QString("%1").arg(run_thread->arm1.get_y()*1000));
        ui->textEdit_z->setText(QString("%1").arg(run_thread->arm1.get_z()*1000));
    }
    else
    {
        ui->textEdit_x->setText(QString("%1").arg(run_thread->armr.get_x()*1000));
        ui->textEdit_y->setText(QString("%1").arg(run_thread->armr.get_y()*1000));
        ui->textEdit_z->setText(QString("%1").arg(run_thread->armr.get_z()*1000));
    }
}

void MainWindow::setTextC123()
{
    if(choose_l_r)
    {
        ui->textEdit_c1->setText(QString("%1").arg(run_thread->arm1.get_c1()));
        ui->textEdit_c2->setText(QString("%1").arg(run_thread->arm1.get_c2()));
        ui->textEdit_c3->setText(QString("%1").arg(run_thread->arm1.get_c3()));
    }
    else
    {
        ui->textEdit_c1->setText(QString("%1").arg(run_thread->armr.get_c1()));
        ui->textEdit_c2->setText(QString("%1").arg(run_thread->armr.get_c2()));
        ui->textEdit_c3->setText(QString("%1").arg(run_thread->armr.get_c3()));
    }
}

void MainWindow::on_horizontalSlider_c0_valueChanged(int value)
{
    if(choose_l_r)
        run_thread->arm1.setJointAng(value,run_thread->arm1.get_c2(),run_thread->arm1.get_c3());
    else
        run_thread->armr.setJointAng(value,run_thread->armr.get_c2(),run_thread->armr.get_c3());
    ui->textEdit_c1->setText(QString("%1").arg(value));
    setTextXYZ();
}

void MainWindow::on_horizontalSlider_c1_valueChanged(int value)
{
   if(choose_l_r)
    run_thread->arm1.setJointAng(run_thread->arm1.get_c1(),value,run_thread->arm1.get_c3());
   else
    run_thread->armr.setJointAng(run_thread->armr.get_c1(),value,run_thread->armr.get_c3());
   ui->textEdit_c2->setText(QString("%1").arg(value));
   setTextXYZ();
}

void MainWindow::on_horizontalSlider_c2_valueChanged(int value)
{
   if(choose_l_r)
    run_thread->arm1.setJointAng(run_thread->arm1.get_c1(),run_thread->arm1.get_c2(),value);
   else
     run_thread->armr.setJointAng(run_thread->armr.get_c1(),run_thread->armr.get_c2(),value);
   ui->textEdit_c3->setText(QString("%1").arg(value));
   setTextXYZ();
}

void MainWindow::on_button_left_clicked()
{
    choose_l_r = true;
    ui->horizontalSlider_c0->setValue(run_thread->arm1.get_c1());
    ui->horizontalSlider_c1->setValue(run_thread->arm1.get_c2());
    ui->horizontalSlider_c2->setValue(run_thread->arm1.get_c3());
    ui->button_left->setStyleSheet("QPushButton{background-color: rgb(20, 62, 134);border:none;color:rgb(0, 0, 0);}");
    ui->button_rigtht->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:none;color:rgb(0, 0, 0);}");
    ui->horizontalSlider_armx->setValue(run_thread->arm1.get_x()*1000);
    ui->horizontalSlider_army->setValue(run_thread->arm1.get_y()*1000);
    ui->horizontalSlider_armz->setValue(run_thread->arm1.get_z()*1000);

}

void MainWindow::on_button_rigtht_clicked()
{
    choose_l_r = false;
    ui->horizontalSlider_c0->setValue(run_thread->armr.get_c1());
    ui->horizontalSlider_c1->setValue(run_thread->armr.get_c2());
    ui->horizontalSlider_c2->setValue(run_thread->armr.get_c3());
    ui->button_rigtht->setStyleSheet("QPushButton{background-color: rgb(20, 62, 134);border:none;color:rgb(0, 0, 0);}");
    ui->button_left->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border:none;color:rgb(0, 0, 0);}");
    ui->horizontalSlider_armx->setValue(run_thread->armr.get_x()*1000);
    ui->horizontalSlider_army->setValue(run_thread->armr.get_y()*1000);
    ui->horizontalSlider_armz->setValue(run_thread->armr.get_z()*1000);

}

void MainWindow::on_horizontalSlider_armx_valueChanged(int value)
{
     ui->textEdit_x->setText(QString("%1").arg(value));
    float val = (float)value/1000;
    if(choose_l_r)
        run_thread->arm1.setPos(val,run_thread->arm1.get_y(),run_thread->arm1.get_z());
    else
        run_thread->armr.setPos(val,run_thread->armr.get_y(),run_thread->armr.get_z());
    setTextC123();
}

void MainWindow::on_horizontalSlider_army_valueChanged(int value)
{
    ui->textEdit_y->setText(QString("%1").arg(value));
    float val = (float)value/1000;
    if(choose_l_r)
        run_thread->arm1.setPos(run_thread->arm1.get_x(),val,run_thread->arm1.get_z());
    else
        run_thread->armr.setPos(run_thread->armr.get_x(),val,run_thread->armr.get_z());
    setTextC123();
}

void MainWindow::on_horizontalSlider_armz_valueChanged(int value)
{
    ui->textEdit_z->setText(QString("%1").arg(value));
    float val = (float)value/1000;
    if(choose_l_r)
        run_thread->arm1.setPos(run_thread->arm1.get_x(),run_thread->arm1.get_y(),val);
    else
        run_thread->armr.setPos(run_thread->armr.get_x(),run_thread->armr.get_y(),val);
    setTextC123();
}

