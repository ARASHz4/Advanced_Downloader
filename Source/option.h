#ifndef OPTION_H
#define OPTION_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class Option;
}

class Option : public QDialog
{
    Q_OBJECT

public:
    explicit Option(QWidget *parent = 0);
    ~Option();

private:
    Ui::Option *ui;

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

    void closeEvent (QCloseEvent *);

};

#endif // OPTION_H
