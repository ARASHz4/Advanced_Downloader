#ifndef ADDDOWNLOAD_H
#define ADDDOWNLOAD_H

#include <downloader.h>

#include <QDialog>

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
    std::tuple<QString, QString, bool> Return();

private slots:
    void on_OKEditPushButton_clicked();

    void SetSize();

    void on_addDlListStartPushButton_clicked();

    void on_addDlListPushButton_clicked();

    void on_cancellPushButton_clicked();

    void on_saveToolButton_clicked();

private:
    Ui::addDownload *ui;

    QString DLUrl, DLFile;

    bool startDownload, editUrl=true;

    Downloader *FileDownload;

};

#endif // ADDDOWNLOAD_H
