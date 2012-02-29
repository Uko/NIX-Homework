#include "subdirectorycopy.h"
#include "ui_subdirectorycopy.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

SubdirectoryCopy::SubdirectoryCopy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubdirectoryCopy)
{
    ui->setupUi(this);
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
}

SubdirectoryCopy::~SubdirectoryCopy()
{
    delete ui;
}

void SubdirectoryCopy::fireMessage(const QString &message)
{
    QMessageBox::warning(this, "Warning!", message);
}

void SubdirectoryCopy::srcButtonClicked()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("Source Directory"), (new QFileInfo(ui->sourceField->text()))->isDir() ? ui->sourceField->text() : QDir::homePath());
    if (!path.isNull() && !path.isEmpty()) ui->sourceField->setText(path);
}

void SubdirectoryCopy::dstButtonClicked()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("Target Directory"), (new QFileInfo(ui->targetField->text()))->isDir() ? ui->targetField->text() : QDir::homePath());
    if (!path.isNull() && !path.isEmpty()) ui->targetField->setText(path);
}

void SubdirectoryCopy::copyDirectory(const QString src, const QString dst)
{
    QDir().mkdir(dst);

    QDir srcDir = QDir(src);
    QStringList contents = srcDir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot|QDir::Readable);
    for(int i=0; i<contents.count(); i++)
        copyDirectory(srcDir.absoluteFilePath(contents[i]),
                      QDir(dst).absoluteFilePath(contents[i]));

    contents = srcDir.entryList(QDir::Files|QDir::Readable);
    for(int i=0; i<contents.count(); i++)
        QFile(srcDir.absoluteFilePath(contents[i])).copy(QDir(dst).absoluteFilePath(contents[i]));


}

void SubdirectoryCopy::makeCopy()
{
    QString src = ui->sourceField->text();
    QString dst = ui->targetField->text();
    if(!src.isEmpty() && !dst.isEmpty())
    {
        QDir *dstDir = new QDir(dst);
        if(dstDir->exists())
        {
            QStringList dstSubdirs = dstDir->entryList(QDir::AllDirs|QDir::NoDotAndDotDot);
            QDir *srcDir = new QDir(src);
            if(srcDir->exists())
            {
                QStringList contents = srcDir->entryList(QDir::AllDirs|QDir::NoDotAndDotDot|QDir::Readable);
                if(contents.count()>0)
                {
                    ui->comment->setText("");
                    for(int i=0; i<contents.count(); i++)
                        if(!dstSubdirs.contains(contents[i]))
                            copyDirectory(srcDir->absoluteFilePath(contents[i]),
                                          dstDir->absoluteFilePath(contents[i]));
                    ui->comment->setText("Done !");
                }
            }
            else
                fireMessage("Source directory: \"" + src + "\" does not exist");
        }
        else
            fireMessage("Target directory: \"" + dst + "\" does not exist");

        delete dstDir;
    }
    else
        fireMessage("Please enter your source and target directories!");
}
