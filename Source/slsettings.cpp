#include <QSingleApp/qtsingleapplication.h>
#include "slsettings.h"

int SLSettings::language;
bool SLSettings::automaticLanguage, SLSettings::minimizeToTray, SLSettings::runStartUp;

SLSettings::SLSettings()
{

}

void SLSettings::setMinimizeToTray(bool value)
{
    minimizeToTray = value;
}

bool SLSettings::MinimizeToTray()
{
    return minimizeToTray;
}

void SLSettings::setLanguage(int value)
{
    language = value;
}

void SLSettings::setRunStartUp(bool value)
{
    runStartUp = value;
}

bool SLSettings::RunStartUp()
{
    return runStartUp;
}

int SLSettings::Language()
{
    return language;
}

void SLSettings::setAutomaticLanguage(bool value)
{
    automaticLanguage = value;
}

bool SLSettings::AutomaticLanguage()
{
    return automaticLanguage;
}

void SLSettings::LoadSettings()
{
    QSettings SettingsAD (QApplication::organizationName(), QApplication::applicationName());
    SettingsAD.beginGroup("Option");

    //General
    {
        if((QString(SettingsAD.value("MinimizeToTray").toString()).isEmpty())
                || (QString(SettingsAD.value("MinimizeToTray").toString())!="true"
                    && QString(SettingsAD.value("MinimizeToTray").toString())!="false"))
        {
            SettingsAD.setValue("MinimizeToTray", "true");
        }
        SLSettings::setMinimizeToTray(SettingsAD.value("MinimizeToTray").toBool());

        if((QString(SettingsAD.value("RunStartUp").toString()).isEmpty())
                || (QString(SettingsAD.value("RunStartUp").toString())!="true"
                    && QString(SettingsAD.value("RunStartUp").toString())!="false"))
        {
            SettingsAD.setValue("RunStartUp", "true");
        }
        SLSettings::setRunStartUp(SettingsAD.value("RunStartUp").toBool());

        if(RunStartUp())
        {
            #if defined(Q_OS_WIN)
            QSettings settingAddStartUp("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
            settingAddStartUp.setValue(QApplication::applicationName(), "\"" +
                                       QDir::toNativeSeparators(QApplication::applicationFilePath()) + "\" " + "-start");
            #elif defined(Q_OS_MAC)
            QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/LaunchAgents/arashz4.advanceddownloader.plist");
            if(file.open(QIODevice::ReadWrite))
            {
                QTextStream stream(&file);
                stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                          "\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                          "\n<plist version=\"1.0\">"
                          "\n<dict>"
                          "\n	<key>Label</key>"
                          "\n	<string>arashz4.advanceddownloader</string>"
                          "\n	<key>ProgramArguments</key>"
                          "\n	<array>"
                          "\n       <string>" + QApplication::applicationFilePath() + "</string>"
                          "\n       <string>-start</string>"
                          "\n   </array>"
                          "\n   <key>RunAtLoad</key>"
                          "\n   <true/>"
                          "\n</dict>"
                          "\n</plist>" << endl;
            }
            #elif defined(Q_OS_LINUX)
            QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/autostart/AdvancedDownloader.desktop");

            if(!QDir(QFileInfo(file).path()).exists())
            {
                QDir().mkdir(QFileInfo(file).path());
            }

            if(file.open(QIODevice::ReadWrite))
            {
                QTextStream stream(&file);
                stream << "[Desktop Entry]\nType=Application\nExec=" + QApplication::applicationDirPath() + "/AdvancedDownloader.sh -start" +
                          "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\nName=Advanced Downloader" << endl;
            }
            #endif
        }
    }

    //Language
    {
        if(!SettingsAD.value("Language").toString().isEmpty() &&
                (SettingsAD.value("Language").toInt() == 0
                 || SettingsAD.value("Language").toInt() == QLocale::English
                 || SettingsAD.value("Language").toInt() == QLocale::Persian
                 || SettingsAD.value("Language").toInt() == QLocale::Spanish))
        {
            SLSettings::setLanguage(SettingsAD.value("Language").toInt());
        }
        else
        {
            SLSettings::setLanguage(0);
            SettingsAD.setValue("Language", SLSettings::Language());
        }

        QTranslator *Translator = new QTranslator;

        if(SLSettings::Language() == 0)
        {
            if(QLocale::system().language() == QLocale::English)
            {
                Translator->load(":/Language/Language/English.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }
            else if(QLocale::system().language() == QLocale::Persian)
            {
                Translator->load(":/Language/Language/Persian.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Persian);
            }
            else if(QLocale::system().language() == QLocale::Spanish)
            {
                Translator->load(":/Language/Language/Spanish.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Spanish);
            }
            else
            {
                Translator->load(":/Language/Language/English.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }

            SLSettings::setAutomaticLanguage(true);
        }
        else
        {
            if(SLSettings::Language() == QLocale::English)
            {
                Translator->load(":/Language/Language/English.qm");
                QApplication::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Persian)
            {
                Translator->load(":/Language/Language/Persian.qm");
                QApplication::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Spanish)
            {
                Translator->load(":/Language/Language/Spanish.qm");
                QApplication::installTranslator(Translator);
            }

            SLSettings::setAutomaticLanguage(false);
        }

        if(SLSettings::Language() == QLocale::English || SLSettings::Language() == QLocale::Spanish)
        {
            QApplication::setLayoutDirection(Qt::LeftToRight);
        }
        else if(SLSettings::Language() == QLocale::Persian)
        {
            QApplication::setLayoutDirection(Qt::RightToLeft);
        }
    }

    SettingsAD.endGroup();
}

void SLSettings::SaveSettings()
{
    QSettings SettingsAD (QApplication::organizationName(), QApplication::applicationName());
    SettingsAD.beginGroup("Option");

    SettingsAD.setValue("MinimizeToTray", SLSettings::MinimizeToTray());

    SettingsAD.setValue("RunStartUp", SLSettings::RunStartUp());

    if(RunStartUp())
    {
        #if defined(Q_OS_WIN)
        QSettings settingAddStartUp("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        settingAddStartUp.setValue(QApplication::applicationName(), "\"" +
                                   QDir::toNativeSeparators(QApplication::applicationFilePath()) + "\" " + "-start");
        #elif defined(Q_OS_MAC)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/LaunchAgents/arashz4.advanceddownloader.plist");
        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                      "\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                      "\n<plist version=\"1.0\">"
                      "\n<dict>"
                      "\n	<key>Label</key>"
                      "\n	<string>arashz4.advanceddownloader</string>"
                      "\n	<key>ProgramArguments</key>"
                      "\n	<array>"
                      "\n       <string>" + QApplication::applicationFilePath() + "</string>"
                      "\n       <string>-start</string>"
                      "\n   </array>"
                      "\n   <key>RunAtLoad</key>"
                      "\n   <true/>"
                      "\n</dict>"
                      "\n</plist>" << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(QObject::tr("Can't save to startup"));
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
        }
        #elif defined(Q_OS_LINUX)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                   + "/.config/autostart/AdvancedDownloader.desktop");

        if(!QDir(QFileInfo(file).path()).exists())
        {
            QDir().mkdir(QFileInfo(file).path());
        }

        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "[Desktop Entry]\nType=Application\nExec=" + QApplication::applicationDirPath() + "/AdvancedDownloader.sh -start"
                      + "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\nName=Advanced Downloader" << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(QObject::tr("Can't save to startup"));
            msg.setInformativeText(QObject::tr("Please make sure you have the correct permission in :\n")
                                   + "~/.config/autostart/AdvancedDownloader.desktop");
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
        }
        #endif
    }
    else
    {
        #if defined(Q_OS_WIN)
        QSettings settingDeleteStartUp("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        settingDeleteStartUp.remove(QApplication::applicationName());
        #elif defined(Q_OS_MAC)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/LaunchAgents/arashz4.advanceddownloader.plist");
        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                      "\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                      "\n<plist version=\"1.0\">"
                      "\n<dict>"
                      "\n	<key>Label</key>"
                      "\n	<string>arashz4.advanceddownloader</string>"
                      "\n	<key>ProgramArguments</key>"
                      "\n	<array>"
                      "\n       <string>" + QApplication::applicationFilePath() + "</string>"
                      "\n       <string>-start</string>"
                      "\n   </array>"
                      "\n   <key>RunAtLoad</key>"
                      "\n   <false/>"
                      "\n</dict>"
                      "\n</plist>" << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(QObject::tr("Can't save to startup"));
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
        }
        #elif defined(Q_OS_LINUX)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                   + "/.config/autostart/AdvancedDownloader.desktop");

        if(!QDir(QFileInfo(file).path()).exists())
        {
            QDir().mkdir(QFileInfo(file).path());
        }

        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "[Desktop Entry]\nType=Application\nExec=" + QApplication::applicationDirPath() + "/AdvancedDownloader.sh -start"
                      + "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=false\nName=Advanced Downloader" << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Big Icon.png"));
            msg.setText(QObject::tr("Can't save to startup"));
            msg.setInformativeText(QObject::tr("Please make sure you have the correct permission in :\n")
                                   + "~/.config/autostart/AdvancedDownloader.desktop");
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
        #endif
    }

    if(SLSettings::AutomaticLanguage())
    {
        SettingsAD.setValue("Language", 0);
    }
    else
    {
        SettingsAD.setValue("Language", SLSettings::Language());
    }

    SettingsAD.endGroup();
}

std::tuple<int, int, int, int, int, bool, bool> SLSettings::LoadDownloaderWindow()
{
    QRect ScreenSize = QApplication::desktop()->screenGeometry();

    QSettings SettingsAD (QApplication::organizationName(), QApplication::applicationName());
    SettingsAD.beginGroup("PhotoWindowSizePos");

    if(SettingsAD.value("toolBarArea").toInt() != Qt::TopToolBarArea && SettingsAD.value("toolBarArea").toInt() != Qt::RightToolBarArea
    && SettingsAD.value("toolBarArea").toInt() != Qt::LeftToolBarArea && SettingsAD.value("toolBarArea").toInt() != Qt::BottomToolBarArea)
    {
        SettingsAD.setValue("toolBarArea", Qt::TopToolBarArea);
    }

    if((QString(SettingsAD.value("window_max").toString()).isEmpty())
         || (QString(SettingsAD.value("window_max").toString())!="true"
         && QString(SettingsAD.value("window_max").toString())!="false"))
    {
        SettingsAD.setValue("window_max", "false");
    }

    if((QString(SettingsAD.value("window_fuls").toString()).isEmpty())
         || (QString(SettingsAD.value("window_fuls").toString())!="true"
         && QString(SettingsAD.value("window_fuls").toString())!="false"))
    {
        SettingsAD.setValue("window_fuls", "false");
    }

    if((SettingsAD.value("window_posx").toInt() != 0 && SettingsAD.value("window_posy").toInt() != 0) &&
       (SettingsAD.value("window_sizew").toInt() <= ScreenSize.width() && SettingsAD.value("window_sizeh").toInt() <= ScreenSize.height()))
    {
        return std::make_tuple(SettingsAD.value("window_posx").toInt(), SettingsAD.value("window_posy").toInt(),
                               SettingsAD.value("window_sizew").toInt(), SettingsAD.value("window_sizeh").toInt(),
                               SettingsAD.value("toolBarArea").toInt(), SettingsAD.value("window_max").toBool(),
                               SettingsAD.value("window_fuls").toBool());
    }
    else
    {
        return std::make_tuple(0, 0, 0, 0, SettingsAD.value("toolBarArea").toInt(),
                               SettingsAD.value("window_max").toBool(), SettingsAD.value("window_fuls").toBool());
    }

    SettingsAD.endGroup();
}

void SLSettings::SaveDownloaderWindow(int x, int y, int w, int h, int toolBarArea, bool window_max, bool window_fuls)
{
    QSettings SettingsAD (QApplication::organizationName(), QApplication::applicationName());

    SettingsAD.beginGroup("PhotoWindowSizePos");

    if(window_max == false && window_fuls == false)
    {
        SettingsAD.setValue("window_posx", x);
        SettingsAD.setValue("window_posy", y);

        SettingsAD.setValue("window_sizew", w);
        SettingsAD.setValue("window_sizeh", h);
    }

    SettingsAD.setValue("toolBarArea", toolBarArea);

    SettingsAD.setValue("window_max", window_max);
    SettingsAD.setValue("window_fuls", window_fuls);

    SettingsAD.endGroup();
}

std::tuple<int, int, int, int> SLSettings::LoadOptionWindow()
{
    QRect ScreenSize = QApplication::desktop()->screenGeometry();

    QSettings SettingsAD (QApplication::organizationName(), QApplication::applicationName());
    SettingsAD.beginGroup("OptionWindowSizePos");

    if((SettingsAD.value("window_posx").toInt() != 0 && SettingsAD.value("window_posy").toInt() != 0) &&
       (SettingsAD.value("window_sizew").toInt() <= ScreenSize.width() && SettingsAD.value("window_sizeh").toInt() <= ScreenSize.height()))
    {
        return std::make_tuple(SettingsAD.value("window_posx").toInt(), SettingsAD.value("window_posy").toInt(),
                               SettingsAD.value("window_sizew").toInt(), SettingsAD.value("window_sizeh").toInt());
    }
    else
    {
        return std::make_tuple(0, 0, 0, 0);
    }
    SettingsAD.endGroup();
}

void SLSettings::SaveOptionWindow(int x, int y, int w, int h)
{
    QSettings SettingsAD (QApplication::organizationName(), QApplication::applicationName());

    SettingsAD.beginGroup("OptionWindowSizePos");

    SettingsAD.setValue("window_posx", x);
    SettingsAD.setValue("window_posy", y);

    SettingsAD.setValue("window_sizew", w);
    SettingsAD.setValue("window_sizeh", h);

    SettingsAD.endGroup();
}
