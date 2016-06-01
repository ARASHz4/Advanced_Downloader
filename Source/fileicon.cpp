#include "fileicon.h"

#include <QDebug>
#include <QLabel>

FileIcon::FileIcon(QObject *parent) : QObject(parent)
{

}

QPixmap FileIcon::getPixmap(QString suffix, int size)
{
    QString AddressFileIcon = QString(QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                                                     + "/ADIF" + QUuid::createUuid().toString() + "." + suffix).replace("//","/");
    QFileIconProvider iconf;
    QPixmap fileIcon;

    if(QFile(AddressFileIcon).open(QIODevice::ReadWrite))
    {
        fileIcon = iconf.icon(QFileInfo(AddressFileIcon)).pixmap(QSize(size, size));

        QFile(AddressFileIcon).remove();
    }

    return fileIcon;
}

QIcon FileIcon::getIcon(QString suffix)
{
    QString AddressFileIcon = QString(QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                                                     + "/ADIF" + QUuid::createUuid().toString() + "." + suffix).replace("//","/");
    QFileIconProvider iconf;
    QIcon fileIcon;

    if(QFile(AddressFileIcon).open(QIODevice::ReadWrite))
    {
        fileIcon = iconf.icon(QFileInfo(AddressFileIcon));

        QFile(AddressFileIcon).remove();
    }

    return fileIcon;
}
