#include "downloaderwindow.h"
#include "ui_downloaderwindow.h"

#include <QDebug>
#include <QtSql>

DownloaderWindow::DownloaderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DownloaderWindow)
{
    ui->setupUi(this);

    ADTray = new QSystemTrayIcon(this);
}

DownloaderWindow::~DownloaderWindow()
{
    delete ui;
}

void DownloaderWindow::Start()
{
    Retranslate();

    //Downloader Window Size & Post Setting
    {
        int x, y, w, h, toolBarArea;
        bool window_max, window_fuls;
        std::tie(x, y, w, h, toolBarArea, window_max, window_fuls) = SLSettings::LoadDownloaderWindow();

        if(x != 0 || y != 0 || w != 0 || h != 0)
        {
            this->setGeometry(x, y, w, h);
        }

        addToolBar(Qt::ToolBarArea(toolBarArea), ui->mainToolBar);

        if(window_max)
        {
            showMaximized();
        }

        if(window_fuls)
        {
            showFullScreen();
        }
    }

    //Load Other Args Delay
    {
        #if defined(Q_OS_MAC)
        //LoadOtherArgsDelay.setInterval(600);
        //LoadOtherArgsDelay.connect(&LoadOtherPhotosDelay,SIGNAL(timeout()),this,SLOT(LoadOtherPhotos()));
        #endif
    }

    QList<QAction *> ActionList;
    QMenu *TrayMenu = new QMenu(this);

    ActionList <<ui->actionAbout <<TrayMenu->addSeparator() <<ui->actionAdd_a_download
               <<TrayMenu->addSeparator() <<ui->actionRestoreWindow <<ui->actionExit;
    TrayMenu->addActions(ActionList);

    ADTray->connect(ADTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
                    SLOT(RestoreWindowTrigger(QSystemTrayIcon::ActivationReason)));
    ADTray->setIcon(QIcon(":/Icon/Icons/Small Icon.png"));
    ADTray->setContextMenu(TrayMenu);
    ADTray->show();

    show();
}

void DownloaderWindow::Retranslate()
{
    ui->retranslateUi(this);

    /*QList<QAction *> ActionList;

    if (QApplication::layoutDirection() == Qt::LeftToRight)
    {
        ActionList << ui->actionOpen_Photo << ui->mainToolBar->addSeparator() << ui->actionZoomIN
                   << ui->actionZoom1_1 << ui->actionZoomOut << ui->actionFitWindow
                   << ui->mainToolBar->addSeparator() << ui->actionRotateLeft << ui->actionRotateRight
                   << ui->mainToolBar->addSeparator() << ui->actionPrevious_Photo << ui->actionSlideshow
                   << ui->actionNext_Photo << ui->mainToolBar->addSeparator() << ui->actionPhotoInfo
                   << ui->mainToolBar->addSeparator() << ui->actionFullscreen;
    }
    else
    {
        ActionList << ui->actionOpen_Photo << ui->mainToolBar->addSeparator() << ui->actionZoomIN
                   << ui->actionZoom1_1 << ui->actionZoomOut << ui->actionFitWindow
                   << ui->mainToolBar->addSeparator() << ui->actionRotateRight << ui->actionRotateLeft
                   << ui->mainToolBar->addSeparator() << ui->actionNext_Photo << ui->actionSlideshow
                   << ui->actionPrevious_Photo << ui->mainToolBar->addSeparator() << ui->actionPhotoInfo
                   << ui->mainToolBar->addSeparator() << ui->actionFullscreen;
    }

    ui->mainToolBar->clear();
    ui->mainToolBar->addActions(ActionList);*/
}

void DownloaderWindow::OpenArguments(QStringList Arguments)
{

}

void DownloaderWindow::closeEvent (QCloseEvent *CloseEvant)
{
    if(SLSettings::MinimizeToTray())
    {
        CloseEvant->ignore();

        this->hide();
    }
    else
    {
        ADTray->hide();

        SLSettings::SaveDownloaderWindow(this->geometry().x(), this->geometry().y(), this->geometry().width(),
                                    this->geometry().height(), toolBarArea(ui->mainToolBar), isMaximized(), isFullScreen());
    }
}

void DownloaderWindow::RestoreWindowTrigger(QSystemTrayIcon::ActivationReason RW)
{
    if(RW == QSystemTrayIcon::Trigger)
    {
        on_actionRestoreWindow_triggered();
    }
}

void DownloaderWindow::on_actionRestoreWindow_triggered()
{
    if(!isVisible())
    {
        show();
        activateWindow();
    }
    else if (isMinimized())
    {
        setWindowState(windowState() & ~Qt::WindowMinimized);
    }
}

void DownloaderWindow::on_actionAdd_a_download_triggered()
{
    addDownload ADD(this);
    ADD.exec();

    QString DownloadUrl, DownloadFile, DownloadSize;
    bool startDownload;

    std::tie(DownloadUrl, DownloadFile, DownloadSize, startDownload) = ADD.Return();

    if(!DownloadUrl.isEmpty() && !DownloadFile.isEmpty())
    {
        DownloadListUrl << DownloadUrl;
        DownloadListFile << DownloadFile;
        DownloadListSize << DownloadSize;

        qDebug()<<DownloadUrl;

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QUrl(DownloadUrl).fileName());
        item->setText(1, DownloadSize);
        ui->downloadTreeWidget->addTopLevelItem(item);

        QProgressBar *newPrProgressBar = new QProgressBar;

        downloadProgressBarList << newPrProgressBar;

        ui->downloadTreeWidget->setItemWidget(item, 3, newPrProgressBar);

        newPrProgressBar->setValue(0);

        if(startDownload)
        {
            //on_actionStart_Download_triggered();
        }
    }

    DatabaseDownload = QSqlDatabase::addDatabase("QSQLITE");
    DatabaseDownload.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "DownloadList.db");

    if(!DatabaseDownload.open())
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle(QApplication::applicationName() + " Error");
        msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
        msg.setText(tr("Can't load download list"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }

    QSqlQuery qry;

    qry.prepare("INSERT INTO DownloadList (DownloadAndress, DownloadSave, DownloadSize)"
                " VALUES ('" + DownloadUrl + "', '" + DownloadFile + "', '" + DownloadSize + "')");
    if(qry.exec())
    {
        qDebug("Inserted!");
    }
    else
    {
        qDebug() << qry.lastError();
    }

    qry.prepare("SELECT * FROM DownloadList");
    if(!qry.exec())
      qDebug() << qry.lastError();
    else
    {
      qDebug( "Selected!" );
    }

//      QSqlRecord rec = qry.record();

//      int cols = rec.count();

//      for( int c=0; c<cols; c++ )
//        qDebug() << QString( "Column %1: %2" ).arg( c ).arg( rec.fieldName(c) );

//      for( int r=0; qry.next(); r++ )
//        for( int c=0; c<cols; c++ )
//          qDebug() << QString( "Row %1, %2: %3" ).arg( r ).arg( rec.fieldName(c) ).arg( qry.value(c).toString() );
//    }


//    qry.prepare( "SELECT firstname, lastname FROM names WHERE lastname = 'Roe'" );
//    if( !qry.exec() )
//      qDebug() << qry.lastError();
//    else
//    {
//      qDebug( "Selected!" );

//      QSqlRecord rec = qry.record();
//    }


}

void DownloaderWindow::on_actionAbout_triggered()
{
    About AD(this);
    AD.exec();
}

void DownloaderWindow::on_actionOptions_triggered()
{
    Options OD(this);
    OD.exec();

    Retranslate();
}

void DownloaderWindow::on_actionStart_Download_triggered()
{
    if(ui->downloadTreeWidget->currentIndex().row() >= 0)
    {
        currentDownload = ui->downloadTreeWidget->currentIndex().row();

        QUrl Url;

        if(!DownloadListUrl[currentDownload].contains("http://"))
        {
            Url = "http://" + DownloadListUrl[currentDownload];
        }
        else
        {
            Url = DownloadListUrl[currentDownload];
        }

        FileDownload = new Downloader(Url, this);

        connect(FileDownload, SIGNAL (downloaded()), this, SLOT (Download_Completed()));
        connect(FileDownload, SIGNAL (updatedProgress()), this, SLOT (SetProgress()));
        connect(FileDownload, SIGNAL (downloadError()), this, SLOT (showDownloadError()));
    }
}

void DownloaderWindow::SetProgress()
{
    ui->downloadTreeWidget->itemAt(currentDownload,2)->setText(2,FileDownload->getDownloadSpeed());
    downloadProgressBarList[currentDownload]->setMaximum(FileDownload->getDLTotal());
    downloadProgressBarList[currentDownload]->setValue(FileDownload->getDLRead());
}

void DownloaderWindow::Download_Completed()
{
    QFile *DownloadedFile = new QFile(DownloadListFile[currentDownload]);

    qDebug()<<DownloadListFile[currentDownload];

    if(DownloadedFile->open(QFile::Append))
    {
        DownloadedFile->write(FileDownload->downloadedData());
        DownloadedFile->flush();
        DownloadedFile->close();
    }
}

void DownloaderWindow::showDownloadError()
{
    downloadProgressBarList[currentDownload]->setValue(0);

    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle(tr("Can't download"));
    msg.setText(tr("Can't download ") + DownloadListUrl[currentDownload]);
    msg.setInformativeText(FileDownload->getDLE());
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, tr("OK"));
    msg.setDefaultButton(QMessageBox::Ok);
    msg.exec();
}

void DownloaderWindow::on_actionStop_Download_triggered()
{
    if(ui->downloadTreeWidget->currentIndex().row() >= 0)
    {
        FileDownload->cancelDownload();
    }
}

void DownloaderWindow::on_actionExit_triggered()
{
    ADTray->hide();

    SLSettings::SaveDownloaderWindow(this->geometry().x(), this->geometry().y(), this->geometry().width(),
                                this->geometry().height(), toolBarArea(ui->mainToolBar), isMaximized(), isFullScreen());

    exit(1);
}
