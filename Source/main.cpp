#include <QSingleApp/qtsingleapplication.h>
#include "downloaderwindow.h"

int main(int argc, char *argv[])
{
    QtSingleApplication ADApplication(argc, argv);

    if (ADApplication.sendMessage("Wake up!"))
    {
        return 0;
    }

    ADApplication.setActivationWindow(&ADApplication.downloaderwindow);

    ADApplication.setOrganizationName("ARASHz4");
    ADApplication.setApplicationName("Advanced Downloader");
    ADApplication.setApplicationVersion("1.0.0 Alpha");
    ADApplication.setOrganizationDomain("arashz4.com");

    ADApplication.StartApplication();

    return ADApplication.exec();
}
