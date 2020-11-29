#include "runthread.h"
void Delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

RunThread::RunThread(QObject *parent):QThread(parent)
{
    arm1.init();armr.init();
}

void RunThread::run()
{
    qDebug()<<"RunThread_start\n";
    while (true) {
        if(blue_window->connected())
        {
            arm1.update(15); armr.update(15);
            sendConMsg();
            getMsg();
        }
        Delay(15);
    }
}

void RunThread::sendConMsg()
{

    char* _arr; int _len;
    char _id = 1 ;
    msg.arm_l = arm1.getAng();
    msg.arm_r = armr.getAng();
    _arr=data_process.dataEncode<ConMsg>(&msg,1,&_len);
    blue_window->sendData(_arr,_len);

//    value = msg.arm_l.c1;
//    _arr=data_process.dataEncode<float>(&value,2,&_len);
//    blue_window->sendData(_arr,_len);
//    qDebug()<<"value: "<< value<<"   _id:"<<_id <<"\n";
}

void RunThread::getMsg()
{
    float value;
 QByteArray _rec_arr = blue_window->readData();
 for(int i=0; i<_rec_arr.size() ; i++ )
 {
      char _data = _rec_arr.data()[i];
       //qDebug()<<"recdata: "<<_data<<"\n";
      if(data_process.getHeadMsg(_data))
      {
        if (data_process.headId()==11)
        {
//            if(data_process.dataDecode<float>(_data,&id) )
//            {
//               qDebug()<<"11 id:"<< (int)id <<"\n";
//            }
        }
       else if (data_process.headId()==12)
        {
//            if(data_process.dataDecode<float>(_data,&value) )
//            {
//               qDebug()<<"12 rec_value:"<< value <<"\n";
//            }
        }
       else if (data_process.headId()==13)
        {
//            if(data_process.dataDecode<float>(_data,&value) )
//            {
//               qDebug()<<"13 :"<< value <<"\n";
//            }
        }
        else
            data_process.clearFlag();

        }

      }

}
