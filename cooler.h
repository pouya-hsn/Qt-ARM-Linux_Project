#ifndef COOLER_H
#define COOLER_H

#include <QWidget>
#include <QPushButton>

#define coolHeader 0x6B

namespace Ui {
class cooler;
}

class cooler : public QWidget
{
    Q_OBJECT

public:
    explicit cooler(QWidget *parent = nullptr);
    ~cooler();
    QByteArray pcktSend(unsigned char data,unsigned char byteNum) ;

signals:
    void kab1CoolerStatus();
    void kab2CoolerStatus();

private slots:
    void on_bckBtn_clicked();

    void on_cmmndBtn_clicked();

    void on_sttsBtn_clicked();

    void on_pwrBtn_clicked();

    void on_tempClrBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbstBtn_clicked();

    void on_lvlingBtn_clicked();

    void on_kab1Btn_clicked();

    void on_kab2Btn_clicked();

    void onButton(QPushButton *p) ;

    void offButton(QPushButton *p) ;

    void showTemp();

    void checkGPIO();

//    void showHmd(unsigned char hmd) ;

private:
    Ui::cooler *ui;
};

#endif // COOLER_H
