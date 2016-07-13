#include "filesize.h"

FileSize::FileSize(QUrl Url, QObject *parent) : QObject(parent)
{
    FSDownloadManager = new QNetworkAccessManager(this);

    FSReply = FSDownloadManager->head(QNetworkRequest(Url));

    connect(FSReply, SIGNAL(finished()), this, SLOT(readHeaders()));
}

void FileSize::readHeaders()
{
    float num = FSReply->header(QNetworkRequest::ContentLengthHeader).toFloat();

    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }

    FSize = QString().setNum(num,'f',2)+" "+unit;

    emit completed();
}

QString FileSize::getFSize() const
{
    return FSize;
}
