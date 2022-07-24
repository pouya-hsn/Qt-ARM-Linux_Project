#include "temp.h"
#include "ui_temp.h"
#include "home.h"
#include "temp.h"
#include "command.h"
#include "status.h"
#include "cooler.h"
#include "power.h"
#include "mobinstatus.h"
#include "mobsatstatus.h"
#include "levelingstatus.h"
#include "pckthandler.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"
#include <QTimer>

extern pckthandler *handler ;
extern levelingstatus *lvlStatusObj ;
extern mobsatstatus *msatStatusObj ;
extern mobinstatus *mobStatusObj;
extern power *pwrObj ;
extern cooler *coolObj ;
extern status *statusObj ;
extern command *cmdObj ;
extern home *homeObj;
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


temp::temp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::temp)
{
    ui->setupUi(this);
    gpioTim = new QTimer();
    gpioTim->start(50);
    connect(gpioTim, &QTimer::timeout, this, &temp::checkGPIO);

    setTempValue() ;
    connect(handler, &pckthandler::tmpHmdChanged, this, &temp::setTempValue);
}

temp::~temp()
{
    delete ui;
}

void temp::on_bckBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    this->close();
    gpioTim->stop();
}

void temp::on_cmmndBtn_clicked()
{
    cmdObj = new command() ;
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void temp::on_sttsBtn_clicked()
{
    statusObj = new status() ;
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void temp::on_clrBtn_clicked()
{
    coolObj = new cooler() ;
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void temp::on_pwrBtn_clicked()
{
    pwrObj = new power() ;
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void temp::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void temp::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void temp::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void temp::setTempValue()
{
    ui->outTmpLbl->setText(QString::number(handler->tmpHmdBytes[0])+"°C");
    ui->outHmdLbl->setText(QString::number(handler->tmpHmdBytes[1])+"%");
    ui->kab1HmdLbl->setText(QString::number(handler->tmpHmdBytes[2])+"%");
    ui->kab2HmdLbl->setText(QString::number(handler->tmpHmdBytes[3])+"%");
    ui->ctrlBoxHmdLbl->setText(QString::number(handler->tmpHmdBytes[4])+"%");
    ui->kab1TmpLbl->setText(QString::number(handler->tmpHmdBytes[5])+"°C");
    ui->kab2TmpLbl->setText(QString::number(handler->tmpHmdBytes[6])+"°C");
    ui->ctrlBoxTmpLbl->setText(QString::number(handler->tmpHmdBytes[7])+"°C");
    ui->mbnDrvTmpLbl->setText(QString::number(handler->tmpHmdBytes[8])+"°C");
    ui->mbsDrvTmpLbl->setText(QString::number(handler->tmpHmdBytes[9])+"°C");
    ui->snrfDrvTmpLbl->setText(QString::number(handler->tmpHmdBytes[10])+"°C");
    ui->powPckTmpLbl->setText(QString::number(handler->tmpHmdBytes[11])+"°C");
}

void temp::checkGPIO()
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
