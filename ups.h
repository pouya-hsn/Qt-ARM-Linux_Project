#ifndef UPS_H
#define UPS_H

#include <QWidget>

namespace Ui {
class UPS;
}

class UPS : public QWidget
{
    Q_OBJECT

public:
    explicit UPS(QWidget *parent = nullptr);
    ~UPS();

private slots:
    void on_backBtn_clicked();

    void checkGPIO() ;

    void on_cmdBtn_clicked();

    void on_statusBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbsBtn_clicked();

    void on_lvlBtn_clicked();

    void on_coolerBtn_clicked();

    void on_tmpHmdBtn_clicked();

    void setStatus() ;

private:
    Ui::UPS *ui;
};

#endif // UPS_H
