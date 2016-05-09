#ifndef ADVANCEDDOWNLOADER_H
#define ADVANCEDDOWNLOADER_H

#include "downloaderwindow.h"
#include "ui_downloaderwindow.h"

#include <QApplication>
#include <QTranslator>

class AdvancedDownloader : public QApplication
{
    Q_OBJECT

public:
    AdvancedDownloader(int &argc, char *argv[]);

private:
    #if defined(Q_OS_MAC)
    bool event(QEvent *event);
    #endif

    QStringList args;

    DownloaderWindow downloaderwindow;

public slots:
    void StartApplication();

};

#endif // ADVANCEDDOWNLOADER_H
