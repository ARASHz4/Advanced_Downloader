#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QUrl Url, QObject *parent = 0);
    virtual ~Downloader();
    QByteArray downloadedData() const;

    qint64 getDLTotal() const;

    qint64 getDLRead() const;

    QString getDLE() const;

    //QString getFileSize() const;

public slots:
    void cancellDownload();

private slots:
    void error(QNetworkReply::NetworkError);

    void updateProgress(qint64 read, qint64 total);

    void fileDownloaded();

private:
    QNetworkAccessManager *DownloadManager;
    QNetworkReply *reply;
    QByteArray DownloadedData;
    qint64 DLTotal, DLRead;
    QString DLE;

signals:
    void downloaded();

    void updatedProgress();

    void downloadError();
};

#endif // DOWNLOADER_H
