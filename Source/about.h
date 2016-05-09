#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private slots:
    void Start();

    void on_OkButton_clicked();

    void on_QtPushButton_clicked();

    void mouseDoubleClickEvent(QMouseEvent *);

private:
    Ui::About *ui;

    bool ds;
};

#endif // ABOUT_H
