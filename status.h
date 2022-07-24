#ifndef STATUS_H
#define STATUS_H

#include <QWidget>

namespace Ui {
class status;
}

class status : public QWidget
{
    Q_OBJECT

public:
    explicit status(QWidget *parent = nullptr);
    ~status();

private slots:
    void checkGPIO() ;

    void on_bckBtn_clicked();

    void on_mbnBigBtn_clicked();

    void on_mbstBigBtn_clicked();

    void on_lvlingBigBtn_clicked();

    void on_cmmndBtn_clicked();

    void on_tempClrBtn_clicked();

    void on_clrBtn_clicked();

    void on_pwrBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbstBtn_clicked();

    void on_lvlingBtn_clicked();

private:
    Ui::status *ui;
};

#endif // STATUS_H
