#ifndef DOWNLOADERWINDOW_H
#define DOWNLOADERWINDOW_H

#include "downloader.h"
#include "slsettings.h"
#include "options.h"
#include "about.h"
#include "adddownload.h"
#include "downlloadproperties.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QTreeWidgetItem>
#include <QFile>
#include <QFileInfo>
#include <QProgressBar>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QtSql>

#if defined(Q_OS_WIN)
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif

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

    QList<QTreeWidgetItem *> DownloadItemList;

    QStringList DownloadListUrl, DownloadListFile, DownloadListSize;

    QList<int> DownloadIDDBList, DownloadListStatus;

    int currentDownload;

    QSystemTrayIcon *ADTray;

    QSqlDatabase DatabaseDownload;

private slots:
    void Retranslate();

    void closeEvent (QCloseEvent *CloseEvant);

    void RestoreWindowTrigger(QSystemTrayIcon::ActivationReason RW);

    void on_actionRestoreWindow_triggered();

    void on_actionAdd_a_download_triggered();

    void on_actionAbout_triggered();

    void on_actionOptions_triggered();

    void on_actionStart_Download_triggered();

    void SetProgress();

    void Download_Completed();

    void showDownloadError();

    void on_actionStop_Download_triggered();

    void on_actionExit_triggered();

    void on_actionDelete_triggered();

    void on_downloadTreeWidget_currentItemChanged(QTreeWidgetItem *current);

    void on_downloadTreeWidget_itemDoubleClicked(QTreeWidgetItem *item);

    void on_actionPauseResume_Download_triggered();

public slots:
    void Start();

    void OpenArguments(QStringList Arguments);

};

#endif // DOWNLOADERWINDOW_H
