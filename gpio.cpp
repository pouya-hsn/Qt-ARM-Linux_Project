#include "gpio.h"

gpio::gpio(int pin, QObject *parent) : QObject(parent)
{
    _pinNumber = pin ;
    _valueString = QString("/sys/class/gpio/gpio%1/value").arg(pin);
    _directionString = QString("sys/class/gpio/gpio%1/direction").arg(pin);
    _strPin = QString("%1").arg(pin);
}

gpio::~gpio()
{
    FILE* fp ;
    if((fp = fopen("/sys/class/gpio/unexport","w"))==NULL)
        return ;
    QString str = _strPin + QString("%1").arg("\n");
    const char *st = str.toStdString().c_str();
    fputs(st,fp) ;
    fclose(fp);
}

int gpio::openPin()
{
    FILE* fp;
    if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL)
        return -1;
    //rewind(fp);//Set pointer to begining of the file
    QString str = _strPin + QString("%1").arg("\n");
    const char *st = str.toStdString().c_str();
    fputs(st,fp) ;
    fclose(fp);
    return 0;
}

int gpio::closePin()
{
    FILE* fp;
    //This will create the folder /sys/class/gpio/gpio37
    if ((fp = fopen("/sys/class/gpio/unexport", "w")) == NULL)
        return -1;
    QString str = _strPin + QString("%1").arg("\n");
    const char *st = str.toStdString().c_str();
    fputs(st,fp) ;
    fclose(fp);
    return 0;
}

int gpio::setDirection(Direction direction)
{
    //set direction in/out. returns 0 if ok and -1 on error
    FILE* fp;
    if ((fp = fopen(_directionString.toLatin1(), "w")) == NULL)
        return -1;
    if(direction == In) fputs("1\n", fp);
    if(direction == Out) fputs("0\n", fp);
    fclose(fp);
    return 0;
}

int gpio::getDirection()
{//returns direction
    char dir[2] ;
    FILE* fp ;
    if ((fp = fopen(_directionString.toLatin1(), "r")) == NULL)
        return -1;
    //rewind(fp);
    fread(&dir,1, 2, fp) ;
    if(dir[0] == '1') return 1;
    else return 0 ;
}

int gpio::setState(bool state)
{//state is 0 or 1. No effect if other value. returns the new state or -1 on error
    FILE* fp;
    if ((fp = fopen(_valueString .toLatin1(), "w")) == NULL) return -1;
    if(state) fputs("1\n", fp);
    else fputs("0\n",fp);
    fclose(fp);
    return 0;
}

bool gpio::getState()
{ //returns 1 or 0 - the pin state. Returns -1 on error
    FILE* fp;
    char value[2];
    if ((fp = fopen(_valueString.toLatin1(), "r")) == NULL) return false;
    fread(&value, 1,2, fp);
    fclose(fp);
    if(value[0]=='1') return true;
    if(value[0]=='0') return false;
    return false;
}
