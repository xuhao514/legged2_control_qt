#include "bluetoothproxy.h"
#include "ui_bluetoothproxy.h"
#define ACCESS_COARSE_LOCATION "android.permission.ACCESS_COARSE_LOCATION"
//#define ACCESS_COARSE_LOCATION "android.Manifest.permission.ACCESS_COARSE_LOCATION"
static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");

BluetoothProxy::BluetoothProxy(QWidget *parent,bool _default_connect ) :
    QDialog(parent),
    ui(new Ui::BluetoothProxy)
{
    ui->setupUi(this);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    localDevice = new QBluetoothLocalDevice();
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    is_connected = false;

    connect(socket,
             SIGNAL(connected()),
             this,
             SLOT(bluetoothConnectedEvent())
             );
     connect(socket,
             SIGNAL(disconnected()),
             this,
             SLOT(bluetoothDisconnectedEvent())
             );
     connect(socket,
                 SIGNAL(readyRead()),
                 this,
                 SLOT(readBluetoothDataEvent())
                 );

    blueReadArray.resize(1024);

    default_connect = _default_connect;
    if(default_connect)
    {
        openBlueTeeth();
        findDevices();
    }
}

BluetoothProxy::~BluetoothProxy()
{
    delete ui;
}



void BluetoothProxy::openBlueTeeth()
{
     localDevice->powerOn();
     localDevice->setHostMode( QBluetoothLocalDevice::HostDiscoverable);
}

void BluetoothProxy::closeBlueTeeth()
{
     localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void BluetoothProxy::findDevices()
{
    checkPermission();
    discoveryAgent->start();
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(findFinish()));  //完成信号发出进行动作
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(addBlueToothDevicesToList(QBluetoothDeviceInfo)));   //当发现info描述的蓝牙设备时发出此信号来进行动作
}

void BluetoothProxy::findFinish()
{
    qDebug()<<"find_finished";
    is_find_finished = true ;
    QList<QBluetoothDeviceInfo> devices_info =  discoveryAgent->discoveredDevices();

}

void BluetoothProxy::addBlueToothDevicesToList( const QBluetoothDeviceInfo &info )
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    qDebug()<<label;
    if(default_connect)
    {
        if(info.name() == default_device)
        {
            QString _msg ="The device:"+ default_device+" is connecting...";
            QMessageBox::information(this,tr("Info"),_msg);
            socket->connectToService(info.address(), QBluetoothUuid(serviceUuid) ,QIODevice::ReadWrite);
        }
    }
    QList<QListWidgetItem *> items = ui->list->findItems(label, Qt::MatchExactly);

   if (items.empty()) {
       QListWidgetItem *item = new QListWidgetItem(label);
       QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
       if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
           item->setTextColor(QColor(Qt::green));
       else
           item->setTextColor(QColor(Qt::black));
       item->setSizeHint(QSize(60,60));
       ui->list->addItem(item);
   }

}



void BluetoothProxy::readBluetoothDataEvent()
{
//    QByteArray line = socket->readAll();
//    QString strData = line.toHex();
    blueReadArray.append(socket->readAll());

}

QByteArray BluetoothProxy::readData()
{
    QByteArray _arr = blueReadArray;
    blueReadArray.clear();
    return _arr;
}

void BluetoothProxy::sendData(char* _arr,int _len)
{
    QByteArray _arrayData(_arr,_len);
    socket->write(_arrayData);
}


void BluetoothProxy::bluetoothConnectedEvent()
{
    discoveryAgent->stop();   //连接成功后停止扫描
    qDebug() << "The android device has been connected successfully!";
    QMessageBox::information(this,tr("Info"),tr("成功连接"));
    is_connected = true;
}

void BluetoothProxy::bluetoothDisconnectedEvent()
{
    qDebug() << "The android device has been disconnected successfully!";
     QMessageBox::information(this,tr("Info"),tr("连接断开"));
     is_connected = false;
}


 bool BluetoothProxy::checkPermission()
 {
    qDebug() << "to check";
     QtAndroid::PermissionResult r = QtAndroid::checkPermission(ACCESS_COARSE_LOCATION);
      if(r == QtAndroid::PermissionResult::Denied) {
          qDebug() << "checking";
          QtAndroid::requestPermissionsSync( QStringList() <<ACCESS_COARSE_LOCATION ,10);
          r = QtAndroid::checkPermission(ACCESS_COARSE_LOCATION);
          if(r == QtAndroid::PermissionResult::Denied) {
              qDebug() << "check flase";
               return false;
          }
          qDebug() << "check true";
     }
     return true;
 }

void BluetoothProxy::on_pushButton_open_clicked()
{
    qDebug()<<"打开蓝牙";
    openBlueTeeth();
}

void BluetoothProxy::on_pushButton_close_clicked()
{
    qDebug()<<"关闭蓝牙";
    closeBlueTeeth();
}

void BluetoothProxy::on_pushButton_find_clicked()
{
    qDebug()<<"查找设备";
     findDevices();
}

void BluetoothProxy::on_list_itemActivated(QListWidgetItem *item)
{
    QString text = item->text();

    int index = text.indexOf(' '); //空格分割  地址/名字

    if (index == -1)
        return;

    QBluetoothAddress address(text.left(index));
    QString name(text.mid(index + 1));
    qDebug() << "You has choice the bluetooth address is " << address;
    qDebug() << "The device is connneting.... ";
    QMessageBox::information(this,tr("Info"),tr("The device is connecting..."));
    socket->connectToService(address, QBluetoothUuid(serviceUuid) ,QIODevice::ReadWrite);
}

void BluetoothProxy::on_pushButton_break_clicked()
{
    socket->disconnectFromService();
}

void BluetoothProxy::on_pushButton_return_clicked()
{
    this->hide();
}
