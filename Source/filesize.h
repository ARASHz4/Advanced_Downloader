#ifndef FILESIZE_H
#define FILESIZE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileSize : public QObject
{
    Q_OBJECT
public:
    explicit FileSize(QUrl Url, QObject *parent = 0);

    QString getFSize() const;

private slots:
    void readHeaders();

private:
    QNetworkAccessManager *FSDownloadManager;
    QNetworkReply *FSReply;

    QString FSize;

signals:
    void completed();

};

#endif // FILESIZE_H
