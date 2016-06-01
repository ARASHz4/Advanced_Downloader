#include "sldownloadlist.h"

#include <QApplication>
#include <QtSql>
#include <QMessageBox>

QSqlDatabase SLDownloadList::DatabaseDownload = QSqlDatabase::addDatabase("QSQLITE");

SLDownloadList::SLDownloadList(QObject *parent) : QObject(parent)
{

}

void SLDownloadList::CreateDBDownloadList()
{
    if(!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).exists())
    {
        if(!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).cdUp())
        {
            QStringList CD (QFileInfo(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).absolutePath().split("/"));
            QString Back;
            for(int i=0; i<CD.count(); i++)
            {
                if(i == 0)
                {
                    Back = CD[i];
                }
               else
                {
                    Back = Back + "/" + CD[i];
                }
            }

            QDir().mkdir(Back);
            QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        }
        else
        {
            QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        }
    }

    DatabaseDownload.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "DownloadList.db");

    if(DatabaseDownload.open())
    {
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS [DownloadList] ([IDDL] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " [DownloadAndress] VARCHAR NOT NULL, [DownloadSave] VARCHAR NOT NULL,"
                    " [DownloadSize] VARCHAR NOT NULL, [DownloadStatus] INTEGER NOT NULL)");

        if(!qry.exec())
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle(QApplication::applicationName() + " Error");
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(QObject::tr("Can't load download list"));
            msg.setInformativeText(qry.lastError().text());
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle(QApplication::applicationName() + " Error");
        msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
        msg.setText(QObject::tr("Can't load download list"));
        msg.setInformativeText(DatabaseDownload.lastError().text());
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }
}

std::tuple<QList<QTreeWidgetItem *>, QStringList, QStringList, QStringList, QList<int>> SLDownloadList::LoadDBDownloadList()
{
    CreateDBDownloadList();

    QList<QTreeWidgetItem *> downloadItemList;
    QStringList DownloadListUrl, DownloadListFile, DownloadListSize;
    QList<int> DownloadListStatus;

    if(DatabaseDownload.isOpen())
    {
        QSqlQuery IDDL, DownloadAndress, DownloadSave, DownloadSize, DownloadStatus;
        IDDL.prepare("SELECT IDDL FROM DownloadList");
        DownloadAndress.prepare("SELECT DownloadAndress FROM DownloadList");
        DownloadSave.prepare("SELECT DownloadSave FROM DownloadList");
        DownloadSize.prepare("SELECT DownloadSize FROM DownloadList");
        DownloadStatus.prepare("SELECT DownloadStatus FROM DownloadList");

        if(IDDL.exec() && DownloadAndress.exec() && DownloadSave.exec() && DownloadSize.exec() && DownloadStatus.exec())
        {
            QSqlRecord rec = IDDL.record();

            int cols = rec.count();

            for(int r=0; IDDL.next(); r++)
            {
                DownloadAndress.next();
                DownloadSave.next();
                DownloadSize.next();
                DownloadStatus.next();

                for( int c=0; c<cols; c++ )
                {
                    QString DLUrl, DLFile, DLSize;
                    int DLStatus;

                    if(DownloadAndress.value(c).isValid())
                    {
                        DLUrl = DownloadAndress.value(c).toString();
                    }
                    else
                    {
                        DLUrl = "";
                    }

                    if(DownloadSave.value(c).isValid())
                    {
                        DLFile = DownloadSave.value(c).toString();
                    }
                    else
                    {
                        DLFile = "";
                    }

                    if(DownloadSize.value(c).isValid())
                    {
                        DLSize = DownloadSize.value(c).toString();
                    }
                    else
                    {
                        DLSize = "";
                    }

                    if(DownloadStatus.value(c).isValid())
                    {
                        DLStatus = DownloadStatus.value(c).toInt();
                    }
                    else
                    {
                        DLStatus = 0;
                    }

                    DownloadListUrl << DLUrl;
                    DownloadListFile << DLFile;
                    DownloadListSize << DLSize;
                    DownloadListStatus << DLStatus;

                    QTreeWidgetItem *newItem = new QTreeWidgetItem();
                    newItem->setText(4, QString::number(r));
                    newItem->setText(0, QUrl(DLUrl).fileName());
                    newItem->setIcon(0, QIcon(QPixmap(FileIcon::getPixmap(QFileInfo(QUrl(DLUrl).fileName()).suffix(), 16))));
                    newItem->setText(1, DLSize);

                    downloadItemList << newItem;
                }
            }
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle(QApplication::applicationName() + " Error");
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(QObject::tr("Can't load download list"));
            msg.setInformativeText(IDDL.lastError().text());
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }

        return std::make_tuple(downloadItemList, DownloadListUrl, DownloadListFile, DownloadListSize, DownloadListStatus);
    }
}

void SLDownloadList::SaveDBDownloadList(QString DownloadUrl, QString DownloadFile, QString DownloadSize)
{
    QSqlQuery qry;

    qry.prepare("INSERT INTO DownloadList ([DownloadAndress], [DownloadSave], [DownloadSize], [DownloadStatus])"
                " VALUES ('" + DownloadUrl + "', '" + DownloadFile + "', '" + DownloadSize + "', '" + DownloadSize + "')");
    if(qry.exec())
    {
        qDebug("Inserted!");
    }
    else
    {
        qDebug() << qry.lastError();
    }
}
