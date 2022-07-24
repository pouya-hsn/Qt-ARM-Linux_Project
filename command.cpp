#include "command.h"
#include "ui_command.h"
#include "home.h"
#include "services.h"
#include "pckthandler.h"
#include <QByteArray>
#include <QtDebug>
#include <QTimer>
#include "gpio.h"
#include "activeform.h"

#define cmdHeader 0x1B
#define setup 0xA1
#define relief 0xB1

QTimer *tim = new QTimer();
extern QTimer *gpioTim;

extern pckthandler *handler;
extern home *homeObj;
extern services *srvObj ;
extern QString cmdTxt;
extern QString lastStCmd;
QString lastStCmdtemp ;
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

extern field lastField;
extern bool mbnSetBtnState ;
extern bool mbsSetBtnState ;
extern bool lvlSetBtnState ;

extern bool mbnRlfBtnState ;
extern bool mbsRlfBtnState ;
extern bool lvlRlfBtnState ;


command::command(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::command)
{
    ui->setupUi(this);
    ui->textEdit->setTextColor(QColor(70,70,70));
    ui->textEdit->setText(cmdTxt) ;

    gpioTim = new QTimer() ;
    connect(gpioTim, &QTimer::timeout, this, &command::checkGPIO);
    gpioTim->start(50);

    tim->stop();
    tim = new QTimer();
    connect(tim,SIGNAL(timeout()) , this , SLOT(mbnSetDefState()));
    connect(tim,SIGNAL(timeout()) , this , SLOT(mbnRlfDefState()));

    connect(tim,SIGNAL(timeout()) , this , SLOT(mbsRlfDefState()));
    connect(tim,SIGNAL(timeout()) , this , SLOT(mbsSetDefState()));

    connect(tim,SIGNAL(timeout()) , this , SLOT(lvlRlfDefState()));
    connect(tim,SIGNAL(timeout()) , this , SLOT(lvlSetDefState()));

    connect(handler, &pckthandler::lvlChanged, this, &command::checkLvl) ;
    connect(handler, &pckthandler::mbnChanged, this, &command::checkMbn) ;
    connect(handler, &pckthandler::mbsChanged, this, &command::checkMbs) ;

    tim->start(1000);

}

command::~command()
{
    delete ui;
}

void command::on_bckCmmndBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    this->close();
    tim->stop();
    gpioTim->stop();
}

void command::on_srvicsBtn_clicked()
{
    srvObj = new services();
    srvObj->show();
    this->close();
    tim->stop();
    gpioTim->stop();
}

void command::on_mbstStupBtn_clicked()
{
    handler->serialSend(pcktSend(setup,4));
}

void command::on_mbnStupBtn_clicked()
{
    handler->serialSend(pcktSend(setup,3));
}


void command::on_lvlingStupBtn_clicked()
{
    handler->serialSend(pcktSend(setup,2));
}

void command::on_mbstRlifBtn_clicked()
{
    handler->serialSend(pcktSend(relief,4));
}


void command::on_mbnRlifBtn_clicked()
{
    handler->serialSend(pcktSend(relief,3));
}


void command::on_lvlingRlifBtn_clicked()
{
    handler->serialSend(pcktSend(relief,2));
}

void command::on_nrmlStupModBtn_clicked()
{
    handler->serialSend(pcktSend(setup,6));
}

void command::on_nrmlRlifModBtn_clicked()
{
    handler->serialSend(pcktSend(relief,6));
}

QByteArray command::pcktSend(unsigned char data,unsigned char byteNum)
{
    QByteArray pckt ;
    pckt.append(cmdHeader);
    for(unsigned char i = 2 ; i < 8 ; i++)
    {
        if(i==byteNum)
            pckt.append(data);
        else
            pckt.append(char(0x00));
    }
    return pckt;
}

void command::on_stopBtn_clicked()
{
    handler->serialSend(pcktSend(0xAB,5)) ;
}
void command::getBackStopBtn()
{
    setanIcon(":/stopLvlBtn.png", ui->stopBtn) ;
}

void command::checkLvl()
{
    if(!ui->textEdit->toPlainText().isEmpty())
    {
        QStringList st = ui->textEdit->toPlainText().split('\n',QString::SkipEmptyParts) ;
        lastStCmd =  st.last() ;
    }
    QString str = "LEVELING IS SETUPING..." ;
    if(((handler->lvlBytes[0]) & 0x80) && (str != lastStCmd))
        ui->textEdit->append(str);


    cmdTxt = ui->textEdit->toPlainText();
    str = "SYSTEM LEVELD √" ;
    if((handler->lvlBytes[1] & 0x10) && (str != lastStCmd))
    {    ui->textEdit->clear();
         ui->textEdit->setTextColor(QColor(120,120,120));
         ui->textEdit->setText(cmdTxt);
         ui->textEdit->setTextColor(QColor(70,70,70));
         ui->textEdit->append(str);
    }

    str = "LEVELING SETUPING STOPPED" ;
    if((handler->lvlBytes[1] & 0x08) && (str != lastStCmd))
        ui->textEdit->append(str);

    str = "SETUPING FAULT" ;
    if((handler->lvlBytes[1] & 0x04) && (str != lastStCmd))
        ui->textEdit->append(str);

    //===================== Leveling Reliefing =======================//

    str = "LEVELING IS RELIEFING..." ;
    if((handler->lvlBytes[2] & 0x80) && (str != lastStCmd))
        ui->textEdit->append(str);

    cmdTxt = ui->textEdit->toPlainText();
    str = "LEVELING RELIEFED √" ;
    if((handler->lvlBytes[3] & 0x40) && (str != lastStCmd))
    {    ui->textEdit->clear();
         ui->textEdit->setTextColor(QColor(120,120,120));
         ui->textEdit->setText(cmdTxt);
         ui->textEdit->setTextColor(QColor(70,70,70));
         ui->textEdit->append(str);
    }

    str = "LEVELING RELIEFING STOPPED" ;
    if((handler->lvlBytes[3] & 0x20) && (str != lastStCmd))
        ui->textEdit->append(str);

    str = "RELIEFING FAULT" ;
    if((handler->lvlBytes[3] & 0x10) && (str != lastStCmd))
        ui->textEdit->append(str);


    cmdTxt = ui->textEdit->toPlainText();

    if(ui->textEdit->document()->lineCount()>7)
    {
        QString txt = ui->textEdit->toPlainText();
        cmdTxt.clear();
        ui->textEdit->clear();
        QStringList ls =  txt.split('\n',QString::SkipEmptyParts);
        ui->textEdit->setText(ls.at(7));
        cmdTxt.append(ls.at(7));
    }
}

void command::checkMbn()
{
    if(!ui->textEdit->toPlainText().isEmpty())
    {
        QStringList st = ui->textEdit->toPlainText().split('\n',QString::SkipEmptyParts) ;
        lastStCmd =  st.last() ;
    }

    QString str = "MOBIN PEDSTAL IS SETUPING..." ;
    if(((handler->mbnBytes[0]) & 0x80) && (str != lastStCmd))
        ui->textEdit->append(str);


    str = "MOBIN PEDSTAL SETUPED √" ;
    cmdTxt = ui->textEdit->toPlainText();
    if((handler->mbnBytes[1] & 0x80) && (str != lastStCmd))
    {
        ui->textEdit->clear();
        ui->textEdit->setTextColor(QColor(120,120,120));
        ui->textEdit->setText(cmdTxt);
        ui->textEdit->setTextColor(QColor(70,70,70));
        ui->textEdit->append(str);
    }

    str = "MOBIN PEDSTAL SETUPING STOPPED" ;
    if((handler->mbnBytes[1] & 0x10) && (str != lastStCmd))
         ui->textEdit->append(str);

    str = "SETUPING FAULT" ;
    if((handler->mbnBytes[1] & 0x08) && (str != lastStCmd))
        ui->textEdit->append(str);

    //======================== Mobin Reliefing =========================//
    str = "MOBIN PEDSTAL IS RELIEFING..." ;
    if((handler->mbnBytes[2] & 0x80) && (str != lastStCmd))
        ui->textEdit->append(str);

    cmdTxt = ui->textEdit->toPlainText() ;
    str = "MOBIN PEDSTAL RELIEFED √" ;
    if((handler->mbnBytes[3] & 0x40) && (str != lastStCmd))
    {
        ui->textEdit->clear();
        ui->textEdit->setTextColor(QColor(120,120,120));
        ui->textEdit->setText(cmdTxt);
        ui->textEdit->setTextColor(QColor(70,70,70));
        ui->textEdit->append(str);
    }

    str = "MOBIN PEDSTAL RELIEFING STOPPED" ;
    if((handler->mbnBytes[3] & 0x20) && (str != lastStCmd))
        ui->textEdit->append(str);


    str = "RELIEF FAULT" ;
    if((handler->mbnBytes[3] & 0x10) && (str != lastStCmd))
        ui->textEdit->append(str);

    cmdTxt = ui->textEdit->toPlainText();
    if(ui->textEdit->document()->lineCount()>7)
    {
        QString txt = ui->textEdit->toPlainText();
        cmdTxt.clear();
        ui->textEdit->clear();
        QStringList ls =  txt.split('\n',QString::SkipEmptyParts);
        ui->textEdit->setText(ls.at(7));
        cmdTxt.append(ls.at(7));
    }

}

void command::checkMbs()
{
    if(!ui->textEdit->toPlainText().isEmpty())
    {
        QStringList st = ui->textEdit->toPlainText().split('\n',QString::SkipEmptyParts) ;
        lastStCmd =  st.last() ;
    }

    QString str = "MOBSAT PEDSTAL IS SETUPING..." ;
    if(((handler->mbsBytes[0]) & 0x80) && (str != lastStCmd))
    {
        ui->textEdit->append(str);
    }


    cmdTxt = ui->textEdit->toPlainText();
    str = "MOBSAT PEDSTAL SETUPED √" ;
    if((handler->mbsBytes[0] & 0x01) && (str != lastStCmd))
    {    ui->textEdit->clear();
         ui->textEdit->setTextColor(QColor(120,120,120));
         ui->textEdit->setText(cmdTxt);
         ui->textEdit->setTextColor(QColor(70,70,70));
         ui->textEdit->append(str);
    }

    str = "MOBSAT PEDSTAL SETUPING STOPPED" ;
    if((handler->mbsBytes[1] & 0x80) && (str != lastStCmd))
    {
        ui->textEdit->append(str);
    }

    str = "SETUPING FAULT" ;
    if((handler->mbsBytes[1] & 0x40) && (str != lastStCmd))
    {
        ui->textEdit->append(str);
    }

    //======================= Mobsat Reliefing ======================//
    str = "MOBSAT PEDSTAL IS RELIEFING..." ;
    if((handler->mbsBytes[2] & 0x80) && (str != lastStCmd))
    {
        ui->textEdit->append(str);
    }
//QString reliefedStr
    cmdTxt = ui->textEdit->toPlainText();
    str = "MOBSAT PEDSTAL RELIEFED √" ;
    if((handler->mbsBytes[2] & 0x01) && (str != lastStCmd))
    {    ui->textEdit->clear();
         ui->textEdit->setTextColor(QColor(120,120,120));
         ui->textEdit->setText(cmdTxt);
         ui->textEdit->setTextColor(QColor(70,70,70));
         ui->textEdit->append(str);
    }
    str = "MOBSAT PEDSTAL RELIEFING STOPPED" ;
    if((handler->mbsBytes[3] & 0x80) && (str != lastStCmd))
    {
        ui->textEdit->append(str);
    }

    str = "RELIEF FAULT" ;
    if((handler->mbsBytes[3] & 0x40) && (str != lastStCmd))
    {
        ui->textEdit->append(str);
    }

    cmdTxt = ui->textEdit->toPlainText();
    if(ui->textEdit->document()->lineCount()>7)
    {
        QString txt = ui->textEdit->toPlainText();
        cmdTxt.clear();
        ui->textEdit->clear();
        QStringList ls =  txt.split('\n',QString::SkipEmptyParts);
        ui->textEdit->setText(ls.at(7));
        cmdTxt.append(ls.at(7));
    }
}

void command::mbnSetDefState()
{
   if(handler->mbnBytes[0] & 0x80) // blinking
   {
       if(mbnSetBtnState)
       {
            setanIcon(":/mbnStupGrnBtn.png", ui->mbnStupBtn) ;
            mbnSetBtnState = false ;
       }
       else
       {
            setanIcon(":/mbnStupBtn.png" , ui->mbnStupBtn);
            mbnSetBtnState = true ;
       }
   }
   else if(handler->mbnBytes[1] & 0x80) //setuped
       setanIcon(":/mbnStupGrnBtn.png", ui->mbnStupBtn)  ;

   else if(handler->mbnBytes[1] & 0x08) //fault
       setanIcon(":/mbnStupRedBtn.png", ui->mbnStupBtn)  ;
   else
       setanIcon(":/mbnStupBtn.png",ui->mbnStupBtn);
}

void command::mbnRlfDefState()
{
    if(handler->mbnBytes[2] & 0x80) // blinking
    {
        if(mbnRlfBtnState)
        {
             setanIcon(":/mbnRlifBluBtn.png",ui->mbnRlifBtn) ;
             mbnRlfBtnState = false ;
        }
        else
        {
             setanIcon(":/mbnRlifBtn.png",ui->mbnRlifBtn);
             mbnRlfBtnState = true ;
        }
    }
    else if(handler->mbnBytes[3] & 0x40) // reliefed
        setanIcon(":/mbnRlifBluBtn.png", ui->mbnRlifBtn)  ;

    else if(handler->mbnBytes[3] & 0x10) // fault
        setanIcon(":/mbnRlifRedBtn.png", ui->mbnRlifBtn)  ;
    else
        setanIcon(":/mbnRlifBtn.png",ui->mbnRlifBtn);
}

void command::mbsSetDefState()
{
    if(handler->mbsBytes[0] & 0x80) // blinking Setuping
    {
        if(mbsSetBtnState)
        {
             setanIcon(":/mbstStupGrnBtn.png", ui->mbstStupBtn) ;
             mbsSetBtnState = false ;
        }
        else
        {
             setanIcon(":/mbstStupBtn.png", ui->mbstStupBtn);
             mbsSetBtnState = true ;
        }
    }
    else if(handler->mbsBytes[0] & 0x01) // setuped
        setanIcon(":/mbstStupGrnBtn.png", ui->mbstStupBtn)  ;

    else if(handler->mbsBytes[1] & 0x40) // fault
        setanIcon(":/mbstStupRedBtn.png", ui->mbstStupBtn)  ;
    else
        setanIcon(":/mbstStupBtn.png",ui->mbstStupBtn);
}

void command::mbsRlfDefState()
{
    if(handler->mbsBytes[2] & 0x80) // blinking
    {
        if(mbsRlfBtnState)
        {
             setanIcon(":/mbstRlifBluBtn.png", ui->mbstRlifBtn) ;
             mbsRlfBtnState = false ;
        }
        else
        {
             setanIcon(":/mbstRlifBtn.png", ui->mbstRlifBtn);
             mbsRlfBtnState = true ;
        }
    }
    else if(handler->mbsBytes[2] & 0x01) // reliefed
        setanIcon(":/mbstRlifBluBtn.png", ui->mbstRlifBtn)  ;

    else if(handler->mbsBytes[3] & 0x40) // fault
        setanIcon(":/mbstRlifRedBtn.png", ui->mbstRlifBtn)  ;
    else
        setanIcon(":/mbstRlifBtn.png",ui->mbstRlifBtn);
}

void command::lvlSetDefState()
{
    if(handler->lvlBytes[0] & 0x80) // blinking
    {
        if(lvlSetBtnState)
        {
             setanIcon(":/lvlingStupGrnBtn.png", ui->lvlingStupBtn) ;
             lvlSetBtnState = false ;
        }
        else
        {
             setanIcon(":/lvlingStupBtn.png",ui->lvlingStupBtn);
             lvlSetBtnState = true ;
        }
    }
    else if(handler->lvlBytes[1] & 0x10) //setuped
        setanIcon(":/lvlingStupGrnBtn.png",ui->lvlingStupBtn)  ;

    else if(handler->lvlBytes[1] & 0x04) // fault
        setanIcon(":/lvlingStupRedBtn.png", ui->lvlingStupBtn)  ;
    else
        setanIcon(":/lvlingStupBtn.png", ui->lvlingStupBtn);
}

void command::lvlRlfDefState()
{
    if(handler->lvlBytes[2] & 0x80) // blinking
    {
        if(lvlRlfBtnState)
        {
             setanIcon(":/lvlingRlifBluBtn.png", ui->lvlingRlifBtn) ;
             lvlRlfBtnState = false ;
        }
        else
        {
             setanIcon(":/lvlingRlifBtn.png", ui->lvlingRlifBtn);
             lvlRlfBtnState = true ;
        }
    }
    else if(handler->lvlBytes[3] & 0x40) // reliefed
        setanIcon(":/lvlingRlifBluBtn.png", ui->lvlingRlifBtn)  ;

    else if(handler->lvlBytes[3] & 0x10) // fault
        setanIcon(":/lvlingRlifRedBtn.png",ui->lvlingRlifBtn)  ;
    else
        setanIcon(":/lvlingRlifBtn.png",ui->lvlingRlifBtn);
}

void command::setanIcon(QString path, QPushButton *btn)
{
    QPixmap pic(path);
    QIcon icon(pic);
    btn->setIcon(icon);
    btn->setIconSize(QSize(pic.width() , pic.height()));
}

void command::checkGPIO()
{
    if(!gp1.getState()) this->on_mbstStupBtn_clicked();
    if(!gp2.getState()) this->on_mbnStupBtn_clicked();
    if(!gp3.getState()) this->on_lvlingStupBtn_clicked();
    if(!gp4.getState()) this->on_nrmlStupModBtn_clicked() ;
    if(!gp5.getState()) this->on_stopBtn_clicked();
//    if(!gp6.getState()) this->on_dfctionModBtn_clicked();
    if(!gp7.getState()) this->on_nrmlRlifModBtn_clicked();
    if(!gp8.getState()) this->on_lvlingRlifBtn_clicked();
    if(!gp9.getState()) this->on_mbnRlifBtn_clicked();
    if(!gp10.getState()) this->on_mbstRlifBtn_clicked() ;
}
