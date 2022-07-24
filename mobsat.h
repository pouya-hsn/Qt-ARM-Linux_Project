#ifndef MOBSAT_H
#define MOBSAT_H

#include <QWidget>

namespace Ui {
class Mobsat;
}

class Mobsat : public QWidget
{
    Q_OBJECT

public:
    explicit Mobsat(QWidget *parent = nullptr);
    ~Mobsat();

private slots:
    void checkGPIO() ;

    void on_mbsBackBtn_clicked();

    void on_mbsCmdBtn_clicked();

    void on_mbsSttsBtn_clicked();

    void on_mbsMbnBtn_clicked();

    void on_mbsPwrBtn_clicked();

    void on_mbsClrBtn_clicked();

    void on_mbsTmpBtn_clicked();

    void on_mbsLvlBtn_clicked();

    void checkMobsatSetup() ;

private:
    Ui::Mobsat *ui;
};

#endif // MOBSAT_H
