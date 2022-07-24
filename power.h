#ifndef POWER_H
#define POWER_H

#include <QWidget>

namespace Ui {
class power;
}

class power : public QWidget
{
    Q_OBJECT

public:
    explicit power(QWidget *parent = nullptr);
    ~power();

private slots:
    void checkGPIO() ;

    void on_bckBtn_clicked();

    void on_cmmndBtn_clicked();

    void on_sttsBtn_clicked();

    void on_clrBtn_clicked();

    void on_tempClrBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbstBtn_clicked();

    void on_lvlingBtn_clicked();

    void setStatus();

    void on_invDetailBtn_clicked();

    void on_upsDetailBtn_clicked();

private:
    Ui::power *ui;
};

#endif // POWER_H
