#ifndef LEVELING_H
#define LEVELING_H

#include <QWidget>

namespace Ui {
class Leveling;
}

class Leveling : public QWidget
{
    Q_OBJECT

public:
    explicit Leveling(QWidget *parent = nullptr);
    ~Leveling();

private slots:
    void checkGPIO();

    void on_lvlCmdBtn_clicked();

    void on_lvlBackBtn_clicked();

    void on_lvlSttsBtn_clicked();

    void on_lvlMbnBtn_clicked();

    void on_lvlMbsBtn_clicked();

    void on_lvlClrBtn_clicked();

    void on_lvlTmpBtn_clicked();

    void on_lvlPwrBtn_clicked();

    void checkLvlSetup();

    void angleValuesSetting();
private:
    Ui::Leveling *ui;
};

#endif // LEVELING_H
