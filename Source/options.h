#ifndef OPTIONS_H
#define OPTIONS_H

#include "slsettings.h"

#include <QDialog>
#include <QPushButton>
#include <QTranslator>
#include <QCloseEvent>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

private:
    Ui::Options *ui;

    QPushButton OK, Cancel, Apply, RestoreDefaults;

private slots:
    void Start();

    void on_listWidgetOption_currentRowChanged(int currentRow);

    void Load();

    void Save();

    void OKButton();

    void CancelButton();

    void ApplyButton();

    void RestoreDefaultsButton();

    void closeEvent (QCloseEvent *CloseEvant);

};

#endif // OPTIONS_H
