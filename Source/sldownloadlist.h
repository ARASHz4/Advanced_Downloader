#ifndef SLDOWNLOADLIST_H
#define SLDOWNLOADLIST_H

#include "fileicon.h"

#include <QObject>
#include <QtSql>
#include <QTreeWidgetItem>

class SLDownloadList : public QObject
{
    Q_OBJECT
public:
    explicit SLDownloadList(QObject *parent = 0);

    static void CreateDBDownloadList();

    static std::tuple<QList<QTreeWidgetItem *>, QStringList, QStringList, QStringList, QList<int> > LoadDBDownloadList();

    static void SaveDBDownloadList(QString DownloadUrl, QString DownloadFile, QString DownloadSize);

signals:

public slots:

private:
    static QSqlDatabase DatabaseDownload;
};

#endif // SLDOWNLOADLIST_H
