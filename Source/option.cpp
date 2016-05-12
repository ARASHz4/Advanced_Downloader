#include "option.h"
#include "ui_option.h"


Option::Option(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Option)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Start();
}

Option::~Option()
{
    delete ui; 
}

void Option::Start()
{
    int x, y, w, h;
    std::tie(x, y, w, h) = SLSettings::LoadOptionWindow();

    if(x != 0 || y != 0 || w != 0 || h != 0)
    {
        this->setGeometry(x, y, w, h);
    }

    OK.connect(&OK, SIGNAL(clicked()), this, SLOT(OKButton()));
    Cancel.connect(&Cancel, SIGNAL(clicked()), this, SLOT(CancelButton()));
    Apply.connect(&Apply, SIGNAL(clicked()), this, SLOT(ApplyButton()));
    RestoreDefaults.connect(&RestoreDefaults, SIGNAL(clicked()), this, SLOT(RestoreDefaultsButton()));
    OK.setText(tr("OK"));
    Cancel.setText(tr("Cancel"));
    Apply.setText(tr("Apply"));
    RestoreDefaults.setText(tr("Restore Defaults"));
    OK.setDefault(true);

    ui->OptionButtonBox->addButton(&OK, QDialogButtonBox::AcceptRole);
    ui->OptionButtonBox->addButton(&Cancel, QDialogButtonBox::RejectRole);
    ui->OptionButtonBox->addButton(&Apply, QDialogButtonBox::ApplyRole);
    ui->OptionButtonBox->addButton(&RestoreDefaults, QDialogButtonBox::ResetRole);

    ui->listWidgetOption->setCurrentRow(0);
    ui->LanguageComboBox->insertSeparator(1);

    Load();
}

void Option::on_listWidgetOption_currentRowChanged(int currentRow)
{
    if(currentRow == 0)
    {
        ui->OptionGroupBox->setTitle(tr("General"));

        ui->minimizeToTrayCheckBox->setVisible(true);

        ui->LanguageLabel->setVisible(false);
        ui->LanguageComboBox->setVisible(false);
    }
    else if (currentRow == 1)
    {
        ui->OptionGroupBox->setTitle(tr("Language"));

        ui->LanguageLabel->setVisible(true);
        ui->LanguageComboBox->setVisible(true);

        ui->minimizeToTrayCheckBox->setVisible(false);
    }
}

void Option::Load()
{
    //General
    {
        ui->minimizeToTrayCheckBox->setChecked(SLSettings::MinimizeToTray());
    }

    //Language
    {
        if(SLSettings::AutomaticLanguage())
        {
            ui->LanguageComboBox->setCurrentIndex(0);
        }
        else
        {
            if(SLSettings::Language() == QLocale::English)
            {
                ui->LanguageComboBox->setCurrentIndex(2);
            }
            else if(SLSettings::Language() == QLocale::Persian)
            {
                ui->LanguageComboBox->setCurrentIndex(3);
            }
            else if(SLSettings::Language() == QLocale::Spanish)
            {
                ui->LanguageComboBox->setCurrentIndex(4);
            }
        }
    }
}

void Option::Save()
{
    //General
    {
        SLSettings::setMinimizeToTray(ui->minimizeToTrayCheckBox->isChecked());
    }

    //Language
    {
        if(ui->LanguageComboBox->currentIndex() == 0)
        {
            SLSettings::setLanguage(0);
        }
        else if(ui->LanguageComboBox->currentIndex() == 2)
        {
            SLSettings::setLanguage(QLocale::English);
        }
        else if(ui->LanguageComboBox->currentIndex() == 3)
        {
            SLSettings::setLanguage(QLocale::Persian);
        }
        else if(ui->LanguageComboBox->currentIndex() == 4)
        {
            SLSettings::setLanguage(QLocale::Spanish);
        }
        else if(ui->LanguageComboBox->currentIndex() == 5)
        {
            SLSettings::setLanguage(QLocale::Chinese);
        }

        QTranslator *Translator = new QTranslator;

        if(SLSettings::Language() == 0)
        {
            if(QLocale::system().language() == QLocale::English)
            {
                Translator->load(":/Language/English.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }
            else if(QLocale::system().language() == QLocale::Persian)
            {
                Translator->load(":/Language/Persian.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Persian);
            }
            else if(QLocale::system().language() == QLocale::Spanish)
            {
                Translator->load(":/Language/Spanish.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Spanish);
            }
            else if(QLocale::system().language() == QLocale::Chinese)
            {
                Translator->load(":/Language/Traditional Chinese.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Chinese);
            }
            else
            {
                Translator->load(":/Language/English.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }

            SLSettings::setAutomaticLanguage(true);
        }
        else
        {
            if(SLSettings::Language() == QLocale::English)
            {
                Translator->load(":/Language/English.qm");
                QApplication::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Persian)
            {
                Translator->load(":/Language/Persian.qm");
                QApplication::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Spanish)
            {
                Translator->load(":/Language/Spanish.qm");
                QApplication::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Chinese)
            {
                Translator->load(":/Language/Traditional Chinese.qm");
                QApplication::installTranslator(Translator);
            }

            SLSettings::setAutomaticLanguage(false);
        }

        if(SLSettings::Language() == QLocale::English || SLSettings::Language() == QLocale::Spanish || SLSettings::Language() == QLocale::Chinese)
        {
            QApplication::setLayoutDirection(Qt::LeftToRight);
        }
        else if(SLSettings::Language() == QLocale::Persian)
        {
            QApplication::setLayoutDirection(Qt::RightToLeft);
        }
    }

    SLSettings::SaveSettings();
}

void Option::OKButton()
{
    Save();
    close();
}

void Option::CancelButton()
{
    close();
}

void Option::ApplyButton()
{
    Save();

    int cl = ui->LanguageComboBox->currentIndex();

    ui->retranslateUi(this);
    OK.setText(tr("OK"));
    Cancel.setText(tr("Cancel"));
    Apply.setText(tr("Apply"));
    RestoreDefaults.setText(tr("Restore Defaults"));
    ui->OptionGroupBox->setTitle(tr("Language"));

    ui->LanguageComboBox->insertSeparator(1);
    ui->LanguageComboBox->setCurrentIndex(cl);
}

void Option::RestoreDefaultsButton()
{
    SLSettings::setAutomaticLanguage(true);
    SLSettings::setLanguage(0);

    Load();
}

void Option::closeEvent (QCloseEvent *)
{
    SLSettings::SaveOptionWindow(this->geometry().x(), this->geometry().y(), this->geometry().width(), this->geometry().height());
}
