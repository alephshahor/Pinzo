#include "saveimagedialog.h"
#include "ui_saveimagedialog.h"
#include <QAbstractButton>

SaveImageDialog::SaveImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveImageDialog)
{
    ui -> setupUi(this);
    connect(ui -> buttonBox, &QDialogButtonBox::clicked,
                  this, &SaveImageDialog::getDialogInput);
}

SaveImageDialog::~SaveImageDialog()
{
    delete ui;
}

void SaveImageDialog::getDialogInput()
{
    mFileName = ui -> fileNameInput -> text();
    mFileFormat = ui -> fileNameFormat -> text();
    mFileQuality = ui -> fileNameQuality -> text().toInt();
}

int SaveImageDialog::getFileQuality() const
{
    return mFileQuality;
}

void SaveImageDialog::setFileQuality(int fileQuality)
{
    mFileQuality = fileQuality;
}

QString SaveImageDialog::getFileFormat() const
{
    return mFileFormat;
}

void SaveImageDialog::setFileFormat(const QString &fileFormat)
{
    mFileFormat = fileFormat;
}

QString SaveImageDialog::getFileName() const
{
    return mFileName;
}

void SaveImageDialog::setFileName(const QString &fileName)
{
    mFileName = fileName;
}
