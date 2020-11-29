#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

#include "arm.h"
#include "data_processing.h"
#include "bluetoothproxy.h"

//运动线程
class RunThread : public QThread
{
    Q_OBJECT
signals:
   void imgDequeue(QImage _img);  //图像出队

public:
    explicit RunThread(QObject *parent = nullptr);
    void run();
    BluetoothProxy *blue_window;
    Arm arm1,armr;
private:

     DataProcess data_process;

     struct ConMsg
     {
       ArmAng arm_l;
       ArmAng arm_r;

       LegAng leg_l;
       LegAng leg_r;
     };
     ConMsg msg;

    float value;
    float id;

     void sendConMsg();
     void getMsg();
};


#endif // RUNTHREAD_H
