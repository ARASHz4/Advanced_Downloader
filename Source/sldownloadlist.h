#ifndef SLDOWNLOADLIST_H
#define SLDOWNLOADLIST_H

#include "fileicon.h"

#include <QObject>
#include <QApplication>
#include <QtSql>
#include <QTreeWidgetItem>
#include <QMessageBox>

class SLDownloadList : public QObject
{
    Q_OBJECT
public:
    explicit SLDownloadList(QObject *parent = 0);

    static void CreateDBDownloadList();

    static std::tuple<QList<int>, QList<QTreeWidgetItem *>, QStringList, QStringList, QStringList, QList<int> > LoadDBDownloadList();

    static int SaveDBDownloadList(QString DownloadUrl, QString DownloadFile, QString DownloadSize, int DownloadStatus);

    static void UpdateDBDownloadList(int IDDB, QString DownloadUrl, QString DownloadFile, QString DownloadSize, int DownloadStatus);

    static void DeleteDL(int IDDB);

signals:

public slots:

private:
    static QSqlDatabase DatabaseDownload;
};

#endif // SLDOWNLOADLIST_H
