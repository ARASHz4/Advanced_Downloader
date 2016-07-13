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
    explicit FileIcon(QObject *parent = 0);

    static QPixmap getPixmap(QString suffix, int size);

    static QIcon getIcon(QString suffix);

signals:

private:
    QString suffixIcon;

};

#endif // FILEICON_H
