#include "advanceddownloader.h"

int main(int argc, char *argv[])
{
    AdvancedDownloader ADApplication(argc, argv);

    ADApplication.setOrganizationName("ARASHz4");
    ADApplication.setApplicationName("Advanced Downloader");
    ADApplication.setApplicationVersion("1.0.0 Alpha");
    ADApplication.setOrganizationDomain("ARASHz4");

    ADApplication.StartApplication();

    return ADApplication.exec();
}
