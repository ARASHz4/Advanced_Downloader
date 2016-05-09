#ifndef SLSETTINGS_H
#define SLSETTINGS_H

#include "advanceddownloader.h"

class SLSettings
{
public:
    SLSettings();

    static void setLanguage(int value);

    static int Language();

    static void setAutomaticLanguage(bool value);

    static bool AutomaticLanguage();

    static void LoadSettings();

    static void SaveSettings();

    static std::tuple<int, int, int, int, int, bool, bool> LoadDownloaderWindow();

    static void SaveDownloaderWindow(int x, int y, int w, int h, int toolBarArea, bool window_max, bool window_fuls);

    static std::tuple<int, int, int, int> LoadOptionWindow();

    static void SaveOptionWindow(int x, int y, int w, int h);

private:
    static bool automaticLanguage;
    static int language;
};

#endif // SLSETTINGS_H
