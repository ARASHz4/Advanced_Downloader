#include "downloaderwindow.h"
#include "ui_downloaderwindow.h"
#include "advanceddownloader.h"
#include "slsettings.h"
#include "option.h"
#include "about.h"
#include "addDownload.h"

#include <QFile>
#include <QFileInfo>
#include <QProgressBar>
#include <QMessageBox>

#include <QDebug>

DownloaderWindow::DownloaderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DownloaderWindow)
{
    ui->setupUi(this);
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
        LoadOtherPhotosDelay.setInterval(600);
        LoadOtherPhotosDelay.connect(&LoadOtherPhotosDelay,SIGNAL(timeout()),this,SLOT(LoadOtherPhotos()));
        #endif
    }

    show();
}

void DownloaderWindow::Retranslate()
{
    ui->retranslateUi(this);

    QList<QAction *> ActionList;

    if (AdvancedDownloader::layoutDirection() == Qt::LeftToRight)
    {
//        ActionList << ui->actionOpen_Photo << ui->mainToolBar->addSeparator() << ui->actionZoomIN
//                   << ui->actionZoom1_1 << ui->actionZoomOut << ui->actionFitWindow
//                   << ui->mainToolBar->addSeparator() << ui->actionRotateLeft << ui->actionRotateRight
//                   << ui->mainToolBar->addSeparator() << ui->actionPrevious_Photo << ui->actionSlideshow
//                   << ui->actionNext_Photo << ui->mainToolBar->addSeparator() << ui->actionPhotoInfo
//                   << ui->mainToolBar->addSeparator() << ui->actionFullscreen;
    }
    else
    {
//        ActionList << ui->actionOpen_Photo << ui->mainToolBar->addSeparator() << ui->actionZoomIN
//                   << ui->actionZoom1_1 << ui->actionZoomOut << ui->actionFitWindow
//                   << ui->mainToolBar->addSeparator() << ui->actionRotateRight << ui->actionRotateLeft
//                   << ui->mainToolBar->addSeparator() << ui->actionNext_Photo << ui->actionSlideshow
//                   << ui->actionPrevious_Photo << ui->mainToolBar->addSeparator() << ui->actionPhotoInfo
//                   << ui->mainToolBar->addSeparator() << ui->actionFullscreen;
    }

//    ui->mainToolBar->clear();
//    ui->mainToolBar->addActions(ActionList);
}

void DownloaderWindow::OpenArguments(QStringList Arguments)
{

}

void DownloaderWindow::closeEvent (QCloseEvent *)
{
    SLSettings::SaveDownloaderWindow(this->geometry().x(), this->geometry().y(), this->geometry().width(),
                                this->geometry().height(), toolBarArea(ui->mainToolBar), isMaximized(), isFullScreen());
}

void DownloaderWindow::on_actionAdd_a_download_triggered()
{
    addDownload ADD(this);
    ADD.exec();

    QString DownloadUrl, DownloadFile;
    bool startDownload;

    std::tie(DownloadUrl, DownloadFile, startDownload) = ADD.Return();

    if(!DownloadUrl.isEmpty() && !DownloadFile.isEmpty())
    {
        DownloadListUrl << DownloadUrl;
        DownloadListFile << DownloadFile;

        qDebug()<<DownloadUrl;

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QUrl(DownloadUrl).fileName());
        ui->downloadTreeWidget->addTopLevelItem(item);

        QProgressBar *newPrProgressBar = new QProgressBar;

        downloadProgressBarList << newPrProgressBar;

        ui->downloadTreeWidget->setItemWidget(item, 2, newPrProgressBar);

        newPrProgressBar->setValue(0);

        if(startDownload)
        {
            on_actionStart_Download_triggered();
        }
    }
}

void DownloaderWindow::on_actionAbout_triggered()
{
    About AD(this);
    AD.exec();
}

void DownloaderWindow::on_actionOption_triggered()
{
    Option OD(this);
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
    //qDebug()<<FileDownload->getDLTotal();
    downloadProgressBarList[currentDownload]->setMaximum(FileDownload->getDLTotal());
    downloadProgressBarList[currentDownload]->setValue(FileDownload->getDLRead());
}

void DownloaderWindow::Download_Completed()
{
    QFile *DownloadedFile = new QFile(DownloadListFile[currentDownload]);

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
    FileDownload->cancelDownload();

    qDebug()<<ui->downloadTreeWidget->currentIndex().row();
}
