#ifndef ADDDOWNLOAD_H
#define ADDDOWNLOAD_H

#include "filesize.h"
#include "fileicon.h"

#include <QDialog>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QStandardPaths>
#include <QCloseEvent>

namespace Ui {
class addDownload;
}

class addDownload : public QDialog
{
    Q_OBJECT

public:
    explicit addDownload(QWidget *parent = 0);
    ~addDownload();

public slots:
    std::tuple<QString, QString, QString, bool> Return();

private slots:
    void on_OKEditPushButton_clicked();

    void SetSize();

    void on_addDlListStartPushButton_clicked();

    void on_addDlListPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_saveToolButton_clicked();

    void closeEvent (QCloseEvent *CloseEvant);

private:
    Ui::addDownload *ui;

    QString DLUrl, DLFile, DLSize;

    bool startDownload, editUrl=true;

    FileSize *ADDLFS;

};

#endif // ADDDOWNLOAD_H
