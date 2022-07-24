#include "leveling.h"
#include "ui_leveling.h"
#include "home.h"
#include "pckthandler.h"
#include "command.h"
#include "status.h"
#include "mobin.h"
#include "mobsat.h"
#include "cooler.h"
#include "temp.h"
#include "power.h"
#include <QDebug>
#include "gpio.h"
#include <QTimer>

extern home *homeObj ;
extern pckthandler *handler ;
extern command *cmdObj;
extern QString lvlTxt;
extern status *statusObj;
extern Mobin *mbnObj ;
extern Mobsat *mbsObj;
extern cooler *coolObj;
extern temp *tmpHmdObj ;
extern power *pwrObj ;

extern QString lastStLvl;

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

Leveling::Leveling(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Leveling)
{
    ui->setupUi(this);
    ui->lvlTxtEdit->setTextColor(QColor(70,70,70));
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &Leveling::checkGPIO);
    gpioTim->start(50);

//    checkLvlSetup();
//    checkLvlRelief();
    angleValuesSetting() ;
    connect(handler, &pckthandler::lvlChanged, this, &Leveling::checkLvlSetup) ;
    connect(handler, &pckthandler::sttsChanged, this, &Leveling::angleValuesSetting) ;
}

Leveling::~Leveling()
{
    delete ui;
}

void Leveling::on_lvlBackBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    this->close();
    gpioTim->stop();
}

void Leveling::on_lvlCmdBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}



void Leveling::on_lvlSttsBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void Leveling::on_lvlMbnBtn_clicked()
{
    mbnObj = new Mobin();
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void Leveling::on_lvlMbsBtn_clicked()
{
    mbsObj = new Mobsat();
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void Leveling::on_lvlClrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void Leveling::on_lvlTmpBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void Leveling::on_lvlPwrBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void Leveling::checkLvlSetup()
{
    if(!ui->lvlTxtEdit->toPlainText().isEmpty())
    {
        QStringList st = ui->lvlTxtEdit->toPlainText().split('\n',QString::SkipEmptyParts) ;
        lastStLvl =  st.last() ;
    }
    //======================== Setup ===========================//
    QString str = "LEVELING IS SETUPING..." ;
    if(((handler->lvlBytes[0]) & 0x80) && (str != lastStLvl))
        ui->lvlTxtEdit->append(str);

    QString strtemp = "JACK1 COMMING DOWN";
    if(((handler->lvlBytes[0]) & 0x40) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK1 REACH ON THE GROUND" ;
    if(((handler->lvlBytes[0]) & 0x20)  && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);
    }

    strtemp = "JACK2 COMMING DOWN" ;
    if((handler->lvlBytes[0] & 0x10) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK2 REACH ON THE GROUND" ;
    if((handler->lvlBytes[0] & 0x08) && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);
    }

    strtemp = "JACK3 COMMING DOWN" ;
    if((handler->lvlBytes[0] & 0x04) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK3 REACH ON THE GROUND" ;
    if((handler->lvlBytes[0] & 0x02) && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);
    }


    strtemp = "JACK4 COMMING DOWN" ;
    if((handler->lvlBytes[1] & 0x80) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK4 REACH ON THE GROUND" ;
    if((handler->lvlBytes[1] & 0x40) && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);
    }

    str = "ALL JACKS ON THE GROUND" ;
    if((handler->lvlBytes[1] & 0x20) && (str != lastStLvl))
        ui->lvlTxtEdit->append(str) ;

    lvlTxt = ui->lvlTxtEdit->toPlainText();
    str = "SYSTEM LEVELED √" ;
    if((handler->lvlBytes[1] & 0x10) && (str != lastStLvl))
    {    ui->lvlTxtEdit->clear();
         ui->lvlTxtEdit->setTextColor(QColor(120,120,120));
         ui->lvlTxtEdit->setText(lvlTxt);
         ui->lvlTxtEdit->setTextColor(QColor(70,70,70));
         ui->lvlTxtEdit->append(str);
    }

    str = "LEVELING SETUPING STOPPED" ;
    if((handler->lvlBytes[1] & 0x08) && (str != lastStLvl))
        ui->lvlTxtEdit->append(str);

    str = "SETUP FAULT" ;
    if((handler->lvlBytes[1] & 0x04) && (str != lastStLvl))
        ui->lvlTxtEdit->append(str);

    //======================== Relief ===========================//
    str = "LEVELING IS RELIEFING" ;
    if((handler->lvlBytes[2] & 0x80) && (str != lastStLvl))
        ui->lvlTxtEdit->append(str);

    strtemp = "JACK1 COMMING UP" ;
    if((handler->lvlBytes[2] & 0x40) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK1 RELIEFED" ;
    if((handler->lvlBytes[2] & 0x20) && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);
    }

    strtemp = "JACK2 COMMING UP" ;
    if((handler->lvlBytes[2] & 0x10) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK2 RELIEFED" ;
    if((handler->lvlBytes[2] & 0x08) && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);
    }

    strtemp = "JACK3 COMMING UP" ;
    if((handler->lvlBytes[2] & 0x04) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK3 RELIEFED" ;
    if((handler->lvlBytes[2] & 0x02) && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);
    }

    strtemp = "JACK4 COMMING UP" ;
    if((handler->lvlBytes[2] & 0x01) && (strtemp != lastStLvl))
        ui->lvlTxtEdit->append(strtemp);

    str = "JACK4 RELIEFED" ;
    if((handler->lvlBytes[3] & 0x80) && (str != lastStLvl))
    {
        lvlTxt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.remove(strtemp);
        lvlTxt.remove(lvlTxt.lastIndexOf("\n"),1);
        ui->lvlTxtEdit->setText(lvlTxt);
        ui->lvlTxtEdit->append(str);

    }
    lvlTxt = ui->lvlTxtEdit->toPlainText();
    str = "LEVELING RELIEFED √" ;
    if((handler->lvlBytes[3] & 0x40) && (str != lastStLvl))
    {    ui->lvlTxtEdit->clear();
         ui->lvlTxtEdit->setTextColor(QColor(120,120,120));
         ui->lvlTxtEdit->setText(lvlTxt);
         ui->lvlTxtEdit->setTextColor(QColor(70,70,70));
         ui->lvlTxtEdit->append(str);
    }

    str = "LEVELING RELIEFING STOPPED" ;
    if((handler->lvlBytes[1] & 0x20) && (str != lastStLvl))
        ui->lvlTxtEdit->append(str);

    str = "RELIEF FAULT" ;
    if((handler->lvlBytes[1] & 0x10) && (str != lastStLvl))
        ui->lvlTxtEdit->append(str);

    lvlTxt = ui->lvlTxtEdit->toPlainText();
    if(ui->lvlTxtEdit->document()->lineCount()>7)
    {
        QString txt = ui->lvlTxtEdit->toPlainText();
        lvlTxt.clear();
        ui->lvlTxtEdit->clear();
        QStringList ls =  txt.split('\n',QString::SkipEmptyParts);
        ui->lvlTxtEdit->setText(ls.at(7));
        lvlTxt.append(ls.at(7));
    }
}



void Leveling::angleValuesSetting()
{
    long int xBytes = ((handler->stBytes[7])& 0x000000FF) | ((handler->stBytes[8] << 8)& 0x0000FF00) | ((handler->stBytes[9] << 16)& 0x00FF0000) | ((handler->stBytes[10] << 24)&0xFF000000);
    float x = float(xBytes)/1000 ;

    long int yBytes = ((handler->stBytes[11])& 0x000000FF) | ((handler->stBytes[12] << 8)& 0x0000FF00) | ((handler->stBytes[13] << 16)& 0x00FF0000) | ((handler->stBytes[14] <<24) & 0xFF000000);
    float y = float(yBytes)/1000 ;

    ui->lvlXvalLbl->setText(QString("%1").arg(x));
    ui->lvlYvalLbl->setText(QString("%1").arg(y));
}

void Leveling::checkGPIO()
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
