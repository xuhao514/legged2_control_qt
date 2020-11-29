#include "mainwindow.h"

#include <QApplication>
#include <QtAndroid>
#include <QAndroidJniEnvironment>


enum Orientation
   {
       Unlocked = 4,           // SCREEN_ORIENTATION_SENSOR    (Android)
       LandscapeLocked = 0,    // SCREEN_ORIENTATION_LANDSCAPE (Android)  横屏
       PortraitLocked = 1,     // SCREEN_ORIENTATION_PORTRAIT  (Android)  竖屏
   };
//设置横屏竖屏
void setOrientation(Orientation orientation)   //
{
   const QAndroidJniObject Activity = QtAndroid::androidActivity();
   QAndroidJniEnvironment JniEnv;

   Activity.callMethod<void>("setRequestedOrientation", "(I)V", orientation);
   if(JniEnv->ExceptionCheck())
   {
       qDebug() << "setOrientation:: failed to set orientation!!!";
       JniEnv->ExceptionDescribe();
       JniEnv->ExceptionClear();
   }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setOrientation(LandscapeLocked);
    BluetoothProxy blue;
    MainWindow w(&blue);

    w.showFullScreen();

    return a.exec();
}
