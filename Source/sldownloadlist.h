#ifndef SLDOWNLOADLIST_H
#define SLDOWNLOADLIST_H

#include "fileicon.h"

#include <QObject>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QUrl>
#include <QDir>
#include <QTreeWidgetItem>
#include <QMessageBox>

#include <QDebug>

class SLDownloadList : public QObject
{
    Q_OBJECT
public:
    explicit SLDownloadList(QObject *parent = 0);

    void CreateDBDownloadList();

    std::tuple<QList<int>, QList<QTreeWidgetItem *>, QStringList, QStringList, QStringList, QList<int> > LoadDBDownloadList();

    int SaveDBDownloadList(QString DownloadUrl, QString DownloadFile, QString DownloadSize, int DownloadStatus);

    void UpdateDBDownloadList(int IDDB, QString DownloadUrl, QString DownloadFile, QString DownloadSize, int DownloadStatus);

    void DeleteDL(int IDDB);

signals:

public slots:

private:
    QSqlDatabase DatabaseDownload;
};

#endif // SLDOWNLOADLIST_H
