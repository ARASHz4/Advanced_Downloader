#include "slsettings.h"
#include "advanceddownloader.h"

#include <QSettings>
#include <QDesktopWidget>

int SLSettings::language;
bool SLSettings::automaticLanguage;

SLSettings::SLSettings()
{

}

void SLSettings::setLanguage(int value)
{
    language = value;
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
    QSettings SettingsAP (AdvancedDownloader::organizationName(), AdvancedDownloader::applicationName());
    SettingsAP.beginGroup("Option");

    //General
    {
        if((QString(SettingsAP.value("KeepAspectRatio").toString()).isEmpty())
                || (QString(SettingsAP.value("KeepAspectRatio").toString())!="true"
                    && QString(SettingsAP.value("KeepAspectRatio").toString())!="false"))
        {
            SettingsAP.setValue("KeepAspectRatio", "true");
        }

        if((QString(SettingsAP.value("LoadPhotosFolder").toString()).isEmpty())
                || (QString(SettingsAP.value("LoadPhotosFolder").toString())!="true"
                    && QString(SettingsAP.value("LoadPhotosFolder").toString())!="false"))
        {
            SettingsAP.setValue("LoadPhotosFolder", "true");
        }
    }

    //Language
    {
        if(!SettingsAP.value("Language").toString().isEmpty() &&
                (SettingsAP.value("Language").toInt() == 0
                 || SettingsAP.value("Language").toInt() == QLocale::English
                 || SettingsAP.value("Language").toInt() == QLocale::Persian
                 || SettingsAP.value("Language").toInt() == QLocale::Spanish
                 || SettingsAP.value("Language").toInt() == QLocale::Chinese))
        {
            SLSettings::setLanguage(SettingsAP.value("Language").toInt());
        }
        else
        {
            SLSettings::setLanguage(0);
            SettingsAP.setValue("Language", SLSettings::Language());
        }

        QTranslator *Translator = new QTranslator;

        if(SLSettings::Language() == 0)
        {
            if(QLocale::system().language() == QLocale::English)
            {
                Translator->load(":/Language/English.qm");
                AdvancedDownloader::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }
            else if(QLocale::system().language() == QLocale::Persian)
            {
                Translator->load(":/Language/Persian.qm");
                AdvancedDownloader::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Persian);
            }
            else if(QLocale::system().language() == QLocale::Spanish)
            {
                Translator->load(":/Language/Spanish.qm");
                AdvancedDownloader::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Spanish);
            }
            else if(QLocale::system().language() == QLocale::Chinese)
            {
                Translator->load(":/Language/Traditional Chinese.qm");
                AdvancedDownloader::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Chinese);
            }
            else
            {
                Translator->load(":/Language/English.qm");
                AdvancedDownloader::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }

            SLSettings::setAutomaticLanguage(true);
        }
        else
        {
            if(SLSettings::Language() == QLocale::English)
            {
                Translator->load(":/Language/English.qm");
                AdvancedDownloader::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Persian)
            {
                Translator->load(":/Language/Persian.qm");
                AdvancedDownloader::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Spanish)
            {
                Translator->load(":/Language/Spanish.qm");
                AdvancedDownloader::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Chinese)
            {
                Translator->load(":/Language/Traditional Chinese.qm");
                AdvancedDownloader::installTranslator(Translator);
            }

            SLSettings::setAutomaticLanguage(false);
        }

        if(SLSettings::Language() == QLocale::English || SLSettings::Language() == QLocale::Spanish || SLSettings::Language() == QLocale::Chinese)
        {
            AdvancedDownloader::setLayoutDirection(Qt::LeftToRight);
        }
        else if(SLSettings::Language() == QLocale::Persian)
        {
            AdvancedDownloader::setLayoutDirection(Qt::RightToLeft);
        }
    }

    SettingsAP.endGroup();
}

void SLSettings::SaveSettings()
{
    QSettings SettingsAP (AdvancedDownloader::organizationName(), AdvancedDownloader::applicationName());
    SettingsAP.beginGroup("Option");

    if(SLSettings::AutomaticLanguage() == true)
    {
        SettingsAP.setValue("Language", 0);
    }
    else
    {
        SettingsAP.setValue("Language", SLSettings::Language());
    }

    SettingsAP.endGroup();
}

std::tuple<int, int, int, int, int, bool, bool> SLSettings::LoadDownloaderWindow()
{
    QRect ScreenSize = AdvancedDownloader::desktop()->screenGeometry();

    QSettings SettingsAP (AdvancedDownloader::organizationName(), AdvancedDownloader::applicationName());
    SettingsAP.beginGroup("PhotoWindowSizePos");

    if(SettingsAP.value("toolBarArea").toInt() != Qt::TopToolBarArea && SettingsAP.value("toolBarArea").toInt() != Qt::RightToolBarArea
    && SettingsAP.value("toolBarArea").toInt() != Qt::LeftToolBarArea && SettingsAP.value("toolBarArea").toInt() != Qt::BottomToolBarArea)
    {
        SettingsAP.setValue("toolBarArea", Qt::TopToolBarArea);
    }

    if((QString(SettingsAP.value("window_max").toString()).isEmpty())
         || (QString(SettingsAP.value("window_max").toString())!="true"
         && QString(SettingsAP.value("window_max").toString())!="false"))
    {
        SettingsAP.setValue("window_max", "false");
    }

    if((QString(SettingsAP.value("window_fuls").toString()).isEmpty())
         || (QString(SettingsAP.value("window_fuls").toString())!="true"
         && QString(SettingsAP.value("window_fuls").toString())!="false"))
    {
        SettingsAP.setValue("window_fuls", "false");
    }

    if((SettingsAP.value("window_posx").toInt() != 0 && SettingsAP.value("window_posy").toInt() != 0) &&
       (SettingsAP.value("window_sizew").toInt() <= ScreenSize.width() && SettingsAP.value("window_sizeh").toInt() <= ScreenSize.height()))
    {
        return std::make_tuple(SettingsAP.value("window_posx").toInt(), SettingsAP.value("window_posy").toInt(),
                               SettingsAP.value("window_sizew").toInt(), SettingsAP.value("window_sizeh").toInt(),
                               SettingsAP.value("toolBarArea").toInt(), SettingsAP.value("window_max").toBool(),
                               SettingsAP.value("window_fuls").toBool());
    }
    else
    {
        return std::make_tuple(0, 0, 0, 0, SettingsAP.value("toolBarArea").toInt(),
                               SettingsAP.value("window_max").toBool(), SettingsAP.value("window_fuls").toBool());
    }

    SettingsAP.endGroup();
}

void SLSettings::SaveDownloaderWindow(int x, int y, int w, int h, int toolBarArea, bool window_max, bool window_fuls)
{
    QSettings SettingsAP (AdvancedDownloader::organizationName(), AdvancedDownloader::applicationName());

    SettingsAP.beginGroup("PhotoWindowSizePos");

    if(window_max == false && window_fuls == false)
    {
        SettingsAP.setValue("window_posx", x);
        SettingsAP.setValue("window_posy", y);

        SettingsAP.setValue("window_sizew", w);
        SettingsAP.setValue("window_sizeh", h);
    }

    SettingsAP.setValue("toolBarArea", toolBarArea);

    SettingsAP.setValue("window_max", window_max);
    SettingsAP.setValue("window_fuls", window_fuls);

    SettingsAP.endGroup();
}

std::tuple<int, int, int, int> SLSettings::LoadOptionWindow()
{
    QRect ScreenSize = AdvancedDownloader::desktop()->screenGeometry();

    QSettings SettingsAP (AdvancedDownloader::organizationName(), AdvancedDownloader::applicationName());
    SettingsAP.beginGroup("OptionWindowSizePos");

    if((SettingsAP.value("window_posx").toInt() != 0 && SettingsAP.value("window_posy").toInt() != 0) &&
       (SettingsAP.value("window_sizew").toInt() <= ScreenSize.width() && SettingsAP.value("window_sizeh").toInt() <= ScreenSize.height()))
    {
        return std::make_tuple(SettingsAP.value("window_posx").toInt(), SettingsAP.value("window_posy").toInt(),
                               SettingsAP.value("window_sizew").toInt(), SettingsAP.value("window_sizeh").toInt());
    }
    else
    {
        return std::make_tuple(0, 0, 0, 0);
    }
    SettingsAP.endGroup();
}

void SLSettings::SaveOptionWindow(int x, int y, int w, int h)
{
    QSettings SettingsAP (AdvancedDownloader::organizationName(), AdvancedDownloader::applicationName());

    SettingsAP.beginGroup("OptionWindowSizePos");

    SettingsAP.setValue("window_posx", x);
    SettingsAP.setValue("window_posy", y);

    SettingsAP.setValue("window_sizew", w);
    SettingsAP.setValue("window_sizeh", h);

    SettingsAP.endGroup();
}
