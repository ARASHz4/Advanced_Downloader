#include "downlloadproperties.h"
#include "ui_downlloadproperties.h"

DownlloadProperties::DownlloadProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownlloadProperties)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

DownlloadProperties::~DownlloadProperties()
{
    delete ui;
}

void DownlloadProperties::showProperties(QString address, QString saveto, QString size, QString status)
{
    ui->DPIconLabel->setPixmap(QPixmap(FileIcon::getPixmap(QFileInfo(QUrl(address).fileName()).suffix(), 16)));
    ui->DPNameLabel->setText(QUrl(address).fileName());
    ui->DPAddressLabel->setText(address);
    ui->DPSaveLineEdit->setText(saveto);
    ui->DPSizeLabel->setText(size);
    ui->DPStatusLabel->setText(status);
}

void DownlloadProperties::on_buttonBox_accepted()
{
    close();
}
