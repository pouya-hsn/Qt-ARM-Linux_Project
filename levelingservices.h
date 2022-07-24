#ifndef LEVELINGSERVICES_H
#define LEVELINGSERVICES_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class levelingServices;
}

class levelingServices : public QWidget
{
    Q_OBJECT

public:
    explicit levelingServices(QWidget *parent = nullptr);
    ~levelingServices();
    QByteArray pcktSend(unsigned char data,unsigned char byteNum);
    void onButton(QPushButton *btn);
    void offButton(QPushButton *btn);
    void defState(QPushButton *btn , bool en) ;

private slots:
    void getBackStopJ1() ;
    void getBackStopJ2() ;
    void getBackStopJ3() ;
    void getBackStopJ4() ;
    void getBackStopAllJ() ;

    void getBackStopUpJ1();
    void getBackStopDwnJ1();
    void getBackStopUpJ2();
    void getBackStopDwnJ2();
    void getBackStopUpJ3();
    void getBackStopDwnJ3();
    void getBackStopUpJ4();
    void getBackStopDwnJ4();
    void getBackStopUpAllJ();
    void getBackStopDwnAllJ();

    void checkGPIO();

    void on_bckLvlingBtn_clicked();

    void on_upJack1Btn_clicked();

    void on_stopJack1Btn_clicked();

    void on_dwnJack1Btn_clicked();

    void on_upJack2Btn_clicked();

    void on_stopJack2Btn_clicked();

    void on_dwnJack2Btn_clicked();

    void on_upJack3Btn_clicked();

    void on_stopJack3Btn_clicked();

    void on_dwnJack3Btn_clicked();

    void on_upJack4Btn_clicked();

    void on_stopJack4Btn_clicked();

    void on_dwnJack4Btn_clicked();

    void on_upAllJackBtn_clicked();

    void on_stopAllJackBtn_clicked();

    void on_dwnAllJackBtn_clicked();

    void on_pwrMotorBtn_clicked();

    void on_highSpeedValBtn_clicked();

    void on_lowSpeedValBtn_clicked();

    void setStates() ;

    void XYAnglesShow();

private:
    Ui::levelingServices *ui;
};

#endif // LEVELINGSERVICES_H
