#include "downloader.h"

#include <QDebug>

Downloader::Downloader(QUrl Url, QObject *parent) : QObject(parent)
{
    DownloadManager = new QNetworkAccessManager(this);

    reply =  DownloadManager->get(QNetworkRequest(Url));

    DownloadTime.start();

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT (fileDownloaded()));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT (sslErrors(const QList<QSslError>&)));
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

void Downloader::cancelDownload()
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

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
    qDebug()<<"arash";
#ifndef QT_NO_SSL
    foreach (const QSslError &error, sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}
