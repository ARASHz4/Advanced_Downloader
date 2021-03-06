#include "sldownloadlist.h"

SLDownloadList::SLDownloadList(QObject *parent) : QObject(parent)
{
    DatabaseDownload = QSqlDatabase::addDatabase("QSQLITE");
    DatabaseDownload.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "DownloadList.db");
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

    if(DatabaseDownload.open())
    {
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS [DownloadList] ([IDDL] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " [DownloadAddress] VARCHAR NOT NULL, [DownloadSave] VARCHAR NOT NULL,"
                    " [DownloadSize] VARCHAR NOT NULL, [DownloadStatus] INTEGER NOT NULL)");

        if(!qry.exec())
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle(QApplication::applicationName() + tr(" Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(tr("Can't load download list"));
            msg.setInformativeText(qry.lastError().text());
            msg.setButtonText(QMessageBox::Ok, tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle(QApplication::applicationName() + tr(" Error"));
        msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
        msg.setText(tr("Can't load download list"));
        msg.setInformativeText(DatabaseDownload.lastError().text());
        msg.setButtonText(QMessageBox::Ok, tr("OK"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }
}

std::tuple<QList<int>, QList<QTreeWidgetItem *>, QStringList, QStringList, QStringList, QList<int>> SLDownloadList::LoadDBDownloadList()
{
    CreateDBDownloadList();

    if(DatabaseDownload.open())
    {
        QList<QTreeWidgetItem *> DownloadItemList;
        QStringList DownloadListUrl, DownloadListFile, DownloadListSize;
        QList<int>DownloadIDDBList, DownloadListStatus;

        QSqlQuery IDDL, DownloadAddress, DownloadSave, DownloadSize, DownloadStatus;
        IDDL.prepare("SELECT IDDL FROM DownloadList");
        DownloadAddress.prepare("SELECT DownloadAddress FROM DownloadList");
        DownloadSave.prepare("SELECT DownloadSave FROM DownloadList");
        DownloadSize.prepare("SELECT DownloadSize FROM DownloadList");
        DownloadStatus.prepare("SELECT DownloadStatus FROM DownloadList");

        if(IDDL.exec() && DownloadAddress.exec() && DownloadSave.exec() && DownloadSize.exec() && DownloadStatus.exec())
        {
            QSqlRecord rec = IDDL.record();

            int cols = rec.count();

            for(int r=0; IDDL.next(); r++)
            {
                DownloadAddress.next();
                DownloadSave.next();
                DownloadSize.next();
                DownloadStatus.next();

                for( int c=0; c<cols; c++ )
                {
                    QString DLUrl, DLFile, DLSize;
                    int DLIDDB, DLStatus;

                    DLIDDB = IDDL.value(c).toInt();

                    if(DownloadAddress.value(c).isValid())
                    {
                        DLUrl = DownloadAddress.value(c).toString();
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

                    DownloadIDDBList << DLIDDB;
                    DownloadListUrl << DLUrl;
                    DownloadListFile << DLFile;
                    DownloadListSize << DLSize;
                    DownloadListStatus << DLStatus;

                    QTreeWidgetItem *newItem = new QTreeWidgetItem();
                    newItem->setText(4, QString::number(r));
                    newItem->setText(0, QUrl(DLUrl).fileName());
                    newItem->setIcon(0, QIcon(QPixmap(FileIcon::getPixmap(QFileInfo(QUrl(DLUrl).fileName()).suffix(), 16))));
                    newItem->setText(1, DLSize);

                    DownloadItemList << newItem;
                }
            }
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QApplication::applicationName() + tr(" Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(tr("Can't load download list"));
            msg.setInformativeText(IDDL.lastError().text());
            msg.setButtonText(QMessageBox::Ok, tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }

        return std::make_tuple(DownloadIDDBList, DownloadItemList, DownloadListUrl, DownloadListFile, DownloadListSize, DownloadListStatus);
    }
}

int SLDownloadList::SaveDBDownloadList(QString DownloadUrl, QString DownloadFile, QString DownloadSize, int DownloadStatus)
{
    if(DatabaseDownload.open())
    {
        QSqlQuery SaveQry;

        SaveQry.prepare("INSERT INTO DownloadList ([DownloadAddress], [DownloadSave], [DownloadSize], [DownloadStatus])"
                    " VALUES ('" + DownloadUrl + "', '" + DownloadFile + "', '" + DownloadSize + "', '" + QString::number(DownloadStatus) + "')");

        if(SaveQry.exec())
        {
            qDebug("Inserted!");
        }
        else
        {
            qDebug()<<SaveQry.lastError();
        }

        QSqlQuery IDDLDB;
        IDDLDB.prepare("SELECT IDDL FROM DownloadList WHERE IDDL = (SELECT MAX(IDDL) FROM DownloadList)");
        if(IDDLDB.exec())
        {
            IDDLDB.next();

            return IDDLDB.value(0).toInt();
        }
        else
        {

            qDebug()<<IDDLDB.lastError();
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Add download error"));
        msg.setText(tr("Can't add download to list"));
        msg.setInformativeText(DatabaseDownload.lastError().text());
        msg.setButtonText(QMessageBox::Ok, tr("OK"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }
}

void SLDownloadList::UpdateDBDownloadList(int IDDB, QString DownloadUrl, QString DownloadFile, QString DownloadSize, int DownloadStatus)
{
    if(DatabaseDownload.open())
    {
        QSqlQuery UpdateQry;

        UpdateQry.prepare("UPDATE DownloadList SET DownloadStatus = '" + QString::number(DownloadStatus)
                          + "' WHERE IDDL = " + QString::number(IDDB));

        UpdateQry.exec();
    }
}

void SLDownloadList::DeleteDL(int IDDB)
{
    if(DatabaseDownload.open())
    {
        QSqlQuery DeleteDLQry;

        DeleteDLQry.prepare("DELETE FROM DownloadList WHERE IDDL =" + QString::number(IDDB));

        DeleteDLQry.exec();
    }
}
