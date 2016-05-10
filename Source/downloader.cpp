#include "downloader.h"

#include <QDebug>

Downloader::Downloader(QUrl Url, QObject *parent) : QObject(parent)
{
    DownloadManager = new QNetworkAccessManager(this);

    reply =  DownloadManager->get(QNetworkRequest(Url));

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT (fileDownloaded()));
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
    return DLE;
}

void Downloader::updateProgress(qint64 read, qint64 total)
{
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

void Downloader::fileDownloaded()
{
    DownloadedData = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}

void Downloader::cancellDownload()
{
    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
    disconnect(reply, SIGNAL(finished()), this, SLOT (fileDownloaded()));

    reply->abort();
}

QByteArray Downloader::downloadedData() const
{
    return DownloadedData;
}
