#include "services.h"
#include "ui_services.h"
#include "home.h"
#include "command.h"
#include "mobsatservices.h"
#include "mobinservices.h"
#include "levelingservices.h"
#include "status.h"
#include "power.h"
#include "cooler.h"
#include "temp.h"
#include "mobinstatus.h"
#include "mobsatstatus.h"
#include "levelingstatus.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"
#include <QTimer>

extern levelingstatus *lvlStatusObj;
extern mobsatstatus *msatStatusObj ;
extern mobinstatus *mobStatusObj;
extern temp *tmpHmdObj ;
extern cooler *coolObj ;
extern status *statusObj ;
extern command *cmdObj;
extern mobsatservices *msatSrvObj;
extern mobinservices *mobSrvObj ;
extern levelingServices *lvlSrvObj ;
extern power *pwrObj ;
extern Leveling *lvlObj ;
extern Mobin *mbnObj ;
extern Mobsat *mbsObj ;

extern QTimer *gpioTim;
extern gpio gp1;
extern gpio gp2;
extern gpio gp3;
extern gpio gp4;
extern gpio gp5;
extern gpio gp6;
extern gpio gp7;
extern gpio gp8;
extern gpio gp9;
extern gpio gp10;


services::services(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::services)
{
    ui->setupUi(this);
    gpioTim = new QTimer();
    gpioTim->start(50);
    connect(gpioTim, &QTimer::timeout, this, &services::checkGPIO);
}

services::~services()
{
    delete ui;
}

void services::on_bckBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_mbstBigBtn_clicked()
{
    msatSrvObj = new mobsatservices();
    msatSrvObj->show();
    this->close();
    gpioTim->stop();
}


void services::on_mbnBigBtn_clicked()
{
    mobSrvObj = new mobinservices();
    mobSrvObj->show();
    this->close();
    gpioTim->stop();
}


void services::on_lvlingBigBtn_clicked()
{
    lvlSrvObj = new levelingServices();
    lvlSrvObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_sttsBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_pwrBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_clrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_tempClrBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void services::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void services::checkGPIO()
{
    if(!gp1.getState()) cmdObj->on_mbstStupBtn_clicked();
    if(!gp2.getState()) cmdObj->on_mbnStupBtn_clicked();
    if(!gp3.getState()) cmdObj->on_lvlingStupBtn_clicked();
    if(!gp4.getState()) cmdObj->on_nrmlStupModBtn_clicked() ;
    if(!gp5.getState()) cmdObj->on_stopBtn_clicked();
//    if(!gp6.getState()) cmdObj->on_dfctionModBtn_clicked();
    if(!gp7.getState()) cmdObj->on_nrmlRlifModBtn_clicked();
    if(!gp8.getState()) cmdObj->on_lvlingRlifBtn_clicked();
    if(!gp9.getState()) cmdObj->on_mbnRlifBtn_clicked();
    if(!gp10.getState()) cmdObj->on_mbstRlifBtn_clicked() ;
}
