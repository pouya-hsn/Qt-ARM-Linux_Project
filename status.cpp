#include "status.h"
#include "ui_status.h"
#include "home.h"
#include "status.h"
#include "mobinstatus.h"
#include "mobsatstatus.h"
#include "levelingstatus.h"
#include "command.h"
#include "temp.h"
#include "cooler.h"
#include "power.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"
#include <QTimer>

extern power *pwrObj ;
extern cooler *coolObj ;
extern temp *tmpHmdObj ;
extern command *cmdObj;
extern levelingstatus *lvlStatusObj;
extern home *homeObj;
extern mobinstatus *mobStatusObj ;
extern mobsatstatus *msatStatusObj;
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


status::status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::status)
{
    ui->setupUi(this);
    gpioTim = new QTimer();
    gpioTim->start(50);
    connect(gpioTim, &QTimer::timeout, this, &status::checkGPIO);
}

status::~status()
{
    delete ui;
}

void status::on_bckBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    this->close();
    gpioTim->stop();
}


void status::on_mbnBigBtn_clicked()
{
    mobStatusObj = new mobinstatus();
    mobStatusObj->show();
    this->close();
    gpioTim->stop();
}


void status::on_mbstBigBtn_clicked()
{
    msatStatusObj = new mobsatstatus();
    msatStatusObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_lvlingBigBtn_clicked()
{
    lvlStatusObj = new levelingstatus();
    lvlStatusObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_cmmndBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_tempClrBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_clrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_pwrBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void status::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void status::checkGPIO()
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
