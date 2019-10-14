#ifndef SAVEIMAGEDIALOG_H
#define SAVEIMAGEDIALOG_H

#include <QDialog>

/* TODO: Force the user to introduce all the fields before
      pressing ok. */

namespace Ui {
class SaveImageDialog;
}

class SaveImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveImageDialog(QWidget *parent = nullptr);
    ~SaveImageDialog();

    QString getFileName() const;
    void setFileName(const QString &fileName);

    QString getFileFormat() const;
    void setFileFormat(const QString &fileFormat);

    int getFileQuality() const;
    void setFileQuality(int fileQuality);

private:
    /*!
     * \brief gets the values from the QLineEdit items
     * in the Widget and stores them in the class
     * attributes.
     */
    void getDialogInput();

private:

    QString mFileName;
    QString mFileFormat;
    int     mFileQuality;

    Ui::SaveImageDialog *ui;
};

#endif // SAVEIMAGEDIALOG_H
