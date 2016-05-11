#include "fileicon.h"

FileIcon::FileIcon(QString suffix, QObject *parent) : QObject(parent)
{
    suffixIcon = suffix;
}

QPixmap FileIcon::getPixmap() const
{
    QString AddressFileIcon = QString(QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                                                     + "/ADIF" + QUuid::createUuid().toString() + "." + suffixIcon).replace("//","/");

    QFile(AddressFileIcon).open(QIODevice::ReadWrite);

    QFileIconProvider iconf;

    return iconf.icon(QFileInfo(AddressFileIcon)).pixmap(QSize(32, 32));
    QFile(AddressFileIcon).remove();
}

QIcon FileIcon::getIcon() const
{
    QString AddressFileIcon = QString(QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                                                     + "/ADIF" + QUuid::createUuid().toString() + "." + suffixIcon).replace("//","/");

    QFile(AddressFileIcon).open(QIODevice::ReadWrite);

    QFileIconProvider iconf;

    return iconf.icon(QFileInfo(AddressFileIcon));
    QFile(AddressFileIcon).remove();
}
