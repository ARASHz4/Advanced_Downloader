#ifndef FILEICON_H
#define FILEICON_H

#include <QObject>
#include <QStandardPaths>
#include <QUuid>
#include <QFile>
#include <QFileIconProvider>

class FileIcon : public QObject
{
    Q_OBJECT
public:
    explicit FileIcon(QString suffix, QObject *parent = 0);

    QIcon getIcon() const;

    QPixmap getPixmap() const;

signals:

private:
    QString suffixIcon;

};

#endif // FILEICON_H
