#ifndef SERVICES_H
#define SERVICES_H

#include <QWidget>

namespace Ui {
class services;
}

class services : public QWidget
{
    Q_OBJECT

public:
    explicit services(QWidget *parent = nullptr);
    ~services();

private slots:
    void checkGPIO() ;

    void on_bckBtn_clicked();

    void on_mbstBigBtn_clicked();

    void on_mbnBigBtn_clicked();

    void on_lvlingBigBtn_clicked();

    void on_sttsBtn_clicked();

    void on_pwrBtn_clicked();

    void on_clrBtn_clicked();

    void on_tempClrBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbstBtn_clicked();

    void on_lvlingBtn_clicked();

private:
    Ui::services *ui;
};

#endif // SERVICES_H
