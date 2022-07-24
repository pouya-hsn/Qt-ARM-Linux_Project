#ifndef MOBINSERVICES_H
#define MOBINSERVICES_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class mobinservices;
}

class mobinservices : public QWidget
{
    Q_OBJECT

public:
    explicit mobinservices(QWidget *parent = nullptr);
    ~mobinservices();
    QByteArray pcktSend(unsigned char data , unsigned char byteNum);

private slots:
    void checkGPIO() ;

    void getBackStopSunroof() ;

    void getBackStopMotor() ;

    void getBackStopDwnSunroof();
    void getBackStopUpSunroof();
    void getBackStopDwnMotor();
    void getBackStopUpMotor();

    void on_bckMbnBtn_clicked();

    void on_upMotorBtn_clicked();

    void on_stopMotorBtn_clicked();

    void on_dwnMotorBtn_clicked();

    void on_upSunroofBtn_clicked();

    void on_stopSunroofBtn_clicked();

    void on_dwnSunroofBtn_clicked();

    void on_upClmpBtn_clicked();

    void on_downClmpBtn_clicked();

    void on_pedClmpBtn_clicked();

    void setStates() ;

    void on_pedParkMbnBtn_clicked();

    void pedParkBtnCheck();

    void on_pedPwrMbnBtn_clicked();

private:
    Ui::mobinservices *ui;
};

#endif // MOBINSERVICES_H
