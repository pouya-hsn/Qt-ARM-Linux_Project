#include "mobin.h"
#include "ui_mobin.h"
#include "home.h"
#include "pckthandler.h"
#include "command.h"
#include "status.h"
#include "mobsat.h"
#include "power.h"
#include "cooler.h"
#include "temp.h"
#include "leveling.h"
#include "gpio.h"
#include <QTimer>
#include <QDebug>

extern home *homeObj;
extern pckthandler *handler ;
extern command *cmdObj;
extern QString mbnTxt ;
extern status *statusObj ;
extern Mobsat *mbsObj ;
extern power *pwrObj ;
extern cooler *coolObj;
extern temp *tmpHmdObj;
extern Leveling *lvlObj ;

extern QString lastStMbn;

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


Mobin::Mobin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mobin)
{
    ui->setupUi(this);
    ui->mbnTxtEdit->setTextColor(QColor(70,70,70));
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &Mobin::checkGPIO);
    gpioTim->start(50);
    ui->mbnTxtEdit->setText(mbnTxt) ;
    connect(handler, &pckthandler::mbnChanged, this, &Mobin::checkMobinSetup);
}

Mobin::~Mobin()
{
    delete ui;
}

void Mobin::on_mbnBackBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::on_mbnCmdBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::on_mbnSttsBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::on_mbnMbsBtn_clicked()
{
    mbsObj = new Mobsat();
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::on_mbnPwrBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::on_mbnClrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::on_mbnTmpBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::on_mbnLvlBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void Mobin::checkMobinSetup()
{
    if(!ui->mbnTxtEdit->toPlainText().isEmpty())
    {
        QStringList st = ui->mbnTxtEdit->toPlainText().split('\n',QString::SkipEmptyParts) ;
        lastStMbn =  st.last() ;
    }
    //===================== Setup =======================//
    QString str = "MOBIN PEDSTAL IS SETUPING..." ;
    if(((handler->mbnBytes[0]) & 0x80) && (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    QString strtemp = "SUNROOF IS OPENING";
    if(((handler->mbnBytes[0]) & 0x40)&& (strtemp != lastStMbn))
        ui->mbnTxtEdit->append(strtemp);


    str = "SUNROOF IS OPEN" ;
    if(((handler->mbnBytes[0]) & 0x20) && (str != lastStMbn))
    {
        mbnTxt = ui->mbnTxtEdit->toPlainText();
        mbnTxt.remove(strtemp);
        ui->mbnTxtEdit->setText(mbnTxt);
        mbnTxt.remove(mbnTxt.lastIndexOf("\n"),1);
        ui->mbnTxtEdit->append(str);
    }

    str = "UP & DOWN CLAMP IS OPEN" ;
    if((handler->mbnBytes[0] & 0x10) && (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    str = "PEDSTAL LOCK IS OPEN" ;
    if((handler->mbnBytes[0] & 0x08)&& (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    strtemp = "MOBIN PEDSTAL IS COMMING UP" ;
    if((handler->mbnBytes[0] & 0x04)&& (strtemp != lastStMbn) )
        ui->mbnTxtEdit->append(str);


    str = "MOBIN PEDSTAL IS IN OK POSITION" ;
    if((handler->mbnBytes[0] & 0x02) && (str != lastStMbn))
    {
        mbnTxt = ui->mbnTxtEdit->toPlainText();
        mbnTxt.remove(strtemp);
        mbnTxt.remove(mbnTxt.lastIndexOf("\n"),1);
        ui->mbnTxtEdit->setText(mbnTxt);
        ui->mbnTxtEdit->append(str);
    }

    str = "UP CLAMP IS CLOSED" ;
    if((handler->mbnBytes[0] & 0x01)&& (str != lastStMbn))
        ui->mbnTxtEdit->append(str);

    mbnTxt = ui->mbnTxtEdit->toPlainText();
    str = "MOBIN PEDSTAL SETUPED √" ;
    if((handler->mbnBytes[1] & 0x80) && (str != lastStMbn))
    {    ui->mbnTxtEdit->clear();
         ui->mbnTxtEdit->setTextColor(QColor(120,120,120));
         ui->mbnTxtEdit->setText(mbnTxt);
         ui->mbnTxtEdit->setTextColor(QColor(70,70,70));
         ui->mbnTxtEdit->append(str);
    }

    str = "MOBIN PEDSTAL SETUPING STOPPED" ;
    if((handler->mbnBytes[1] & 0x10)&& (str != lastStMbn))
        ui->mbnTxtEdit->append(str);

    str = "SETUP FAULT" ;
    if((handler->mbnBytes[1] & 0x08) && (str != lastStMbn))
        ui->mbnTxtEdit->append(str);

    //    mbnTxt = ui->mbnTxtEdit->toPlainText();

    //===================== Relief =======================//
    str = "MOBIN PEDSTAL IS RELIEFING" ;
    if((handler->mbnBytes[2] & 0x80) && (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    str = "UP & DOWN CLAMP IS OPEN" ;
    if((handler->mbnBytes[2] & 0x40)&& (str != lastStMbn))
        ui->mbnTxtEdit->append(str);

    strtemp = "MOBIN PEDSTAL IS COMMING DOWN" ;
    if((handler->mbnBytes[2] & 0x10)&& (strtemp != lastStMbn))
        ui->mbnTxtEdit->append(strtemp);


    str = "MOBIN PEDSTAL IS IN OK POSITION" ;
    if((handler->mbnBytes[2] & 0x08) && (str != lastStMbn))
    {
        mbnTxt = ui->mbnTxtEdit->toPlainText();
        mbnTxt.remove(strtemp);
        mbnTxt.remove(mbnTxt.lastIndexOf("\n"),1);
        ui->mbnTxtEdit->setText(mbnTxt);
        ui->mbnTxtEdit->append(str);
    }

    str = "DOWN CLAMP CLOSED" ;
    if((handler->mbnBytes[2] & 0x04)&& (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    strtemp = "sunroof closing" ;
    if((handler->mbnBytes[2] & 0x02)&& (strtemp != lastStMbn))
        ui->mbnTxtEdit->append(strtemp);


    str = "SUNROOF CLOSED" ;
    if((handler->mbnBytes[2] & 0x01) && (str != lastStMbn))
    {
        mbnTxt = ui->mbnTxtEdit->toPlainText();
        mbnTxt.remove(strtemp);
        mbnTxt.remove(mbnTxt.lastIndexOf("\n"),1);
        ui->mbnTxtEdit->setText(mbnTxt);
        mbnTxt.append(str);
    }

    str = "UP CLAMP CLOSED" ;
    if((handler->mbnBytes[3] & 0x80) && (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    mbnTxt = ui->mbnTxtEdit->toPlainText();
    str = "MOBIN PEDSTAL RELIEFED √" ;
    if((handler->mbnBytes[3] & 0x40)  && (str != lastStMbn))
    {    ui->mbnTxtEdit->clear();
         ui->mbnTxtEdit->setTextColor(QColor(120,120,120));
         ui->mbnTxtEdit->setText(mbnTxt);
         ui->mbnTxtEdit->setTextColor(QColor(70,70,70));
         ui->mbnTxtEdit->append(str);
    }

    str = "MOBIN PEDSTAL RELIEFING STOPPED" ;
    if((handler->mbnBytes[3] & 0x20) && (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    str = "RELIEF FAULT" ;
    if((handler->mbnBytes[3] & 0x10) && (str != lastStMbn))
        ui->mbnTxtEdit->append(str);


    mbnTxt = ui->mbnTxtEdit->toPlainText();

    if(ui->mbnTxtEdit->document()->lineCount()>7)
    {
        QString txt = ui->mbnTxtEdit->toPlainText();
        mbnTxt.clear();
        ui->mbnTxtEdit->clear();
        QStringList ls =  txt.split('\n',QString::SkipEmptyParts);
        ui->mbnTxtEdit->setText(ls.at(7));
        mbnTxt.append(ls.at(7));
    }
}


void Mobin::checkGPIO()
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
