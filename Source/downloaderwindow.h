#ifndef DOWNLOADERWINDOW_H
#define DOWNLOADERWINDOW_H

#include "downloader.h"
#include "slsettings.h"
#include "option.h"
#include "about.h"
#include "adddownload.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QTreeWidgetItem>
#include <QFile>
#include <QFileInfo>
#include <QProgressBar>
#include <QMessageBox>
#include <QSystemTrayIcon>

namespace Ui {
class DownloaderWindow;
}

class DownloaderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DownloaderWindow(QWidget *parent = 0);
    ~DownloaderWindow();

private:
    Ui::DownloaderWindow *ui;

    Downloader *FileDownload;

    QList<QProgressBar *> downloadProgressBarList;

    QStringList DownloadListUrl, DownloadListFile, DownloadListSize;

    int currentDownload;

    QSystemTrayIcon *tray;

private slots:
    void Retranslate();

    void closeEvent (QCloseEvent *CloseEvant);

    void RestoreWindow();

    void on_actionAdd_a_download_triggered();

    void on_actionAbout_triggered();

    void on_actionOption_triggered();

    void on_actionStart_Download_triggered();

    void SetProgress();

    void Download_Completed();

    void showDownloadError();

    void on_actionStop_Download_triggered();

public slots:
    void Start();

    void OpenArguments(QStringList Arguments);

};

#endif // DOWNLOADERWINDOW_H
