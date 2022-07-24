#ifndef TEMP_H
#define TEMP_H

#include <QWidget>

namespace Ui {
class temp;
}

class temp : public QWidget
{
    Q_OBJECT

public:
    explicit temp(QWidget *parent = nullptr);
    ~temp();

private slots:
    void checkGPIO() ;

    void on_bckBtn_clicked();

    void on_cmmndBtn_clicked();

    void on_sttsBtn_clicked();

    void on_clrBtn_clicked();

    void on_pwrBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbstBtn_clicked();

    void on_lvlingBtn_clicked();

    void setTempValue();

private:
    Ui::temp *ui;
};

#endif // TEMP_H
