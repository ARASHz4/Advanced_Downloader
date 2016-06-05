#include "adddownload.h"
#include "ui_adddownload.h"

addDownload::addDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDownload)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->saveLineEdit->setText(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/");
}

addDownload::~addDownload()
{
    delete ui;
}

void addDownload::on_OKEditPushButton_clicked()
{
    if(editUrl)
    {
        if(!ui->addressLineEdit->text().isEmpty() && QUrl(ui->addressLineEdit->text()).isValid())
        {
            editUrl = false;

            ui->addressLineEdit->setEnabled(false);
            ui->OKEditPushButton->setText(tr("Edit"));

            ui->saveLineEdit->setText(QString(QFileInfo(ui->saveLineEdit->text()).path() + "/"
                                              + QUrl(ui->addressLineEdit->text()).fileName()).replace("//", "/"));

            ui->fileIconLabel->setPixmap(QPixmap(FileIcon::getPixmap(QFileInfo(QUrl(ui->addressLineEdit->text()).fileName()).suffix(), 32)));

            QUrl Url;

            if(!ui->addressLineEdit->text().contains("http://") && !ui->addressLineEdit->text().contains("https://"))
            {
                Url = "http://" + ui->addressLineEdit->text();
            }
            else
            {
                Url = ui->addressLineEdit->text();
            }

            ADDLFS = new FileSize(Url, this);

            connect(ADDLFS, SIGNAL (completed()), this, SLOT (SetSize()));

            ui->saveLabel->setEnabled(true);
            ui->saveLineEdit->setEnabled(true);
            ui->saveToolButton->setEnabled(true);
            ui->addDlListPushButton->setEnabled(true);
            ui->addDlListStartPushButton->setEnabled(true);
            ui->cancelPushButton->setEnabled(true);
        }
    }
    else
    {
        editUrl = true;

        ui->addressLineEdit->setEnabled(true);
        ui->OKEditPushButton->setText(tr("OK"));

        ui->fileIconLabel->setText(" ");
        ui->fileSizeLabel->setText(NULL);

        ui->saveLabel->setEnabled(false);
        ui->saveLineEdit->setEnabled(false);
        ui->saveToolButton->setEnabled(false);
        ui->addDlListPushButton->setEnabled(false);
        ui->addDlListStartPushButton->setEnabled(false);
        ui->cancelPushButton->setEnabled(false);
    }
}

void addDownload::SetSize()
{
    DLSize = ADDLFS->getFSize();
    ADDLFS->deleteLater();
    ui->fileSizeLabel->setText(DLSize);
}

void addDownload::on_addDlListStartPushButton_clicked()
{
    if(QUrl(ui->addressLineEdit->text()).isValid())
    {
        DLUrl = ui->addressLineEdit->text();
        DLFile = ui->saveLineEdit->text();
        startDownload = true;

        close();
    }
}

void addDownload::on_addDlListPushButton_clicked()
{
    if(QUrl(ui->addressLineEdit->text()).isValid())
    {
        DLUrl = ui->addressLineEdit->text();
        DLFile = ui->saveLineEdit->text();
        startDownload = false;

        close();
    }
}

std::tuple<QString, QString, QString, QPixmap, bool> addDownload::Return()
{
    return std::make_tuple(DLUrl, DLFile, DLSize, QPixmap(FileIcon::getPixmap(QFileInfo(QUrl(DLUrl).fileName()).suffix(), 16)), startDownload);
}

void addDownload::on_cancelPushButton_clicked()
{
    close();
}

void addDownload::on_saveToolButton_clicked()
{
    QString SaveAddress = QFileDialog::getSaveFileName(this, tr("Save As"), ui->saveLineEdit->text() + "/"
                                                       + QUrl(ui->addressLineEdit->text()).fileName());

    if(!SaveAddress.isEmpty())
    {
        ui->saveLineEdit->setText(SaveAddress);
    }
}

void addDownload::closeEvent (QCloseEvent *CloseEvant)
{
    CloseEvant->ignore();

    this->hide();
}
