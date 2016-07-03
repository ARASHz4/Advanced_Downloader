#ifndef DOWNLLOADPROPERTIES_H
#define DOWNLLOADPROPERTIES_H

#include "fileicon.h"

#include <QDialog>
#include <QUrl>

namespace Ui {
class DownlloadProperties;
}

class DownlloadProperties : public QDialog
{
    Q_OBJECT

public:
    explicit DownlloadProperties(QWidget *parent = 0);
    ~DownlloadProperties();

    void showProperties(QString address, QString saveto, QString size, QString status);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DownlloadProperties *ui;
};

#endif // DOWNLLOADPROPERTIES_H
