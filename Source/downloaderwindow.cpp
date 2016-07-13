#include "downloaderwindow.h"
#include "ui_downloaderwindow.h"
#include "sldownloadlist.h"

#include <QDebug>

DownloaderWindow::DownloaderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DownloaderWindow)
{
    ui->setupUi(this);

    ui->downloadTreeWidget->hideColumn(4);

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
    #if defined(Q_OS_WIN)
    ADTray->setIcon(QIcon(":/Icon/Icons/Small Icon.png"));
    #else
    ADTray->setIcon(QIcon(":/Icon/Icons/Small Icon BW.png"));
    #endif
    ADTray->setContextMenu(TrayMenu);

    std::tie(DownloadIDDBList, DownloadItemList, DownloadListUrl, DownloadListFile,
             DownloadListSize, DownloadListStatus) = SLDownloadList::LoadDBDownloadList();

    ui->downloadTreeWidget->addTopLevelItems(DownloadItemList);

    for(int i=0; i<DownloadItemList.count(); i++)
    {
        QProgressBar *newProgressBar = new QProgressBar;
        newProgressBar->setValue(DownloadListStatus[i]);
        newProgressBar->setGeometry(0,0,50,3);

        ui->downloadTreeWidget->setItemWidget(DownloadItemList[i], 3, newProgressBar);
    }

    ADTray->show();

    if(QApplication::arguments().count() >= 2)
    {
        if(QApplication::arguments()[1] != "-start")
        {
            show();
        }
    }
    else
    {
        show();
    }
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

void DownloaderWindow::closeEvent(QCloseEvent *CloseEvant)
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
    if(RW == QSystemTrayIcon::DoubleClick)
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

    if (!isActiveWindow())
    {
        activateWindow();
    }
}

void DownloaderWindow::on_actionAdd_a_download_triggered()
{
    addDownload ADD(this);
    ADD.exec();

    QString DownloadUrl, DownloadFile, DownloadSize;
    QPixmap DownloadIcon;
    bool startDownload;

    std::tie(DownloadUrl, DownloadFile, DownloadSize, DownloadIcon, startDownload) = ADD.Return();

    if(!DownloadUrl.isEmpty() && !DownloadFile.isEmpty())
    {
        DownloadIDDBList << SLDownloadList::SaveDBDownloadList(DownloadUrl, DownloadFile, DownloadSize, 0);
        DownloadListUrl << DownloadUrl;
        DownloadListFile << DownloadFile;
        DownloadListSize << DownloadSize;
        DownloadListStatus << 0;

        QTreeWidgetItem *newItem = new QTreeWidgetItem();
        newItem->setText(4, QString::number(DownloadItemList.count()));
        newItem->setText(0, QUrl(DownloadUrl).fileName());
        newItem->setIcon(0, QIcon(DownloadIcon));
        newItem->setText(1, DownloadSize);

        DownloadItemList << newItem;

        ui->downloadTreeWidget->addTopLevelItem(newItem);

        QProgressBar *newProgressBar = new QProgressBar;

        ui->downloadTreeWidget->setItemWidget(newItem, 3, newProgressBar);

        newProgressBar->setValue(0);

        if(startDownload)
        {
            //on_actionStart_Download_triggered();
        }
    }
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
        QUrl Url;

        if(!DownloadListUrl[currentDownload].contains("http://") && !DownloadListUrl[currentDownload].contains("https://"))
        {
            Url = "http://" + DownloadListUrl[currentDownload];
        }
        else
        {
            Url = DownloadListUrl[currentDownload];
        }

        FileDownload = new Downloader(Url, currentDownload, this);

        connect(FileDownload, SIGNAL (downloaded()), this, SLOT (Download_Completed()));
        connect(FileDownload, SIGNAL (updatedProgress()), this, SLOT (SetProgress()));
        connect(FileDownload, SIGNAL (downloadError()), this, SLOT (showDownloadError()));
    }
}

void DownloaderWindow::SetProgress()
{
    qDebug()<<FileDownload->getCDownload();

    DownloadItemList[FileDownload->getCDownload()]->setText(2, FileDownload->getDownloadSpeed());

    QProgressBar *DLProgressBar = new QProgressBar;

    ui->downloadTreeWidget->setItemWidget(DownloadItemList[FileDownload->getCDownload()], 3, DLProgressBar);

    double parsent = FileDownload->getDLTotal() / 100.0;

    DownloadListStatus[FileDownload->getCDownload()] = FileDownload->getDLRead() / parsent;

    SLDownloadList::UpdateDBDownloadList(DownloadIDDBList[FileDownload->getCDownload()],
            "url", "file", "size", DownloadListStatus[FileDownload->getCDownload()]);

    DLProgressBar->setMaximum(FileDownload->getDLTotal());
    DLProgressBar->setValue(FileDownload->getDLRead());
}

void DownloaderWindow::Download_Completed()
{
    QFile *DownloadedFile = new QFile(DownloadListFile[FileDownload->getCDownload()]);

    qDebug()<<DownloadListFile[FileDownload->getCDownload()];

    if(DownloadedFile->open(QFile::Append))
    {
        DownloadedFile->write(FileDownload->downloadedData());
        DownloadedFile->flush();
        DownloadedFile->close();
    }
}

void DownloaderWindow::showDownloadError()
{
    QProgressBar *DLProgressBar = new QProgressBar;
    ui->downloadTreeWidget->setItemWidget(DownloadItemList[FileDownload->getCDownload()], 3, DLProgressBar);
    DLProgressBar->setValue(0);

    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle(tr("Can't download"));
    msg.setText(tr("Can't download ") + DownloadListUrl[FileDownload->getCDownload()]);
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

void DownloaderWindow::on_actionDelete_triggered()
{
    if(ui->downloadTreeWidget->currentIndex().row() >= 0)
    {
        SLDownloadList::DeleteDL(DownloadIDDBList[currentDownload]);

        ui->downloadTreeWidget->takeTopLevelItem(currentDownload);
    }
}

void DownloaderWindow::on_downloadTreeWidget_currentItemChanged(QTreeWidgetItem *current)
{
    currentDownload = current->data(4,0).toInt();
}

void DownloaderWindow::on_downloadTreeWidget_itemDoubleClicked(QTreeWidgetItem *item)
{
    DownlloadProperties DLPD(this);
    DLPD.showProperties(DownloadListUrl[item->data(4,0).toInt()], DownloadListFile[item->data(4,0).toInt()], item->data(1,0).toString(),
                        QString::number(5) + "%");
    DLPD.exec();
}

void DownloaderWindow::on_actionPauseResume_Download_triggered()
{
#if defined(Q_OS_WIN)
    QWinTaskbarButton *button = new QWinTaskbarButton(this);
    button->setWindow(this->windowHandle());
    QWinTaskbarProgress *progress = button->progress();
    progress->setVisible(true);
    progress->setValue(50);
    progress->show();
#endif
}
