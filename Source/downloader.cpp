#include "downloader.h"

#include <QDebug>

Downloader::Downloader(QUrl Url, int CDL, QObject *parent) : QObject(parent)
{
    DownloadManager = new QNetworkAccessManager(this);

    reply =  DownloadManager->get(QNetworkRequest(Url));

    CDownload = CDL;

    DownloadTime.start();

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT (fileDownloaded()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
}

Downloader::~Downloader()
{

}

void Downloader::error(QNetworkReply::NetworkError)
{
    DLE = reply->errorString();
    reply->deleteLater();

    emit downloadError();
}

QString Downloader::getDLE() const
{
    qDebug()<<DLE;
    return DLE;
}

void Downloader::updateProgress(qint64 read, qint64 total)
{
    qDebug()<<CDownload;

    double speed = read * 1000.0 / DownloadTime.elapsed();

    QString unit;
    if (speed < 1024)
    {
        unit = tr("bytes/sec");
    }
    else if (speed < 1024*1024)
    {
        speed /= 1024;
        unit = tr("KB/sec");
    }
    else
    {
        speed /= 1024*1024;
        unit = tr("MB/sec");
    }

    DownloadSpeed = QString::number(speed) + " " + unit;

    DLRead = read;
    DLTotal = total;

    emit updatedProgress();
}

qint64 Downloader::getDLRead() const
{
    return DLRead;
}

qint64 Downloader::getDLTotal() const
{
    return DLTotal;
}

QString Downloader::getDownloadSpeed() const
{
    return DownloadSpeed;
}

void Downloader::fileDownloaded()
{
    DownloadedData = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}

int Downloader::getCDownload() const
{
    return CDownload;
}

void Downloader::cancelDownload()
{
    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
    disconnect(reply, SIGNAL(finished()), this, SLOT (fileDownloaded()));

    //mFile->write(mCurrentReply->readAll());

    reply->abort();
}

QByteArray Downloader::downloadedData() const
{
    return DownloadedData;
}
