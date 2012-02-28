#include "subdirectorycopy.h"
#include "ui_subdirectorycopy.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

SubdirectoryCopy::SubdirectoryCopy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubdirectoryCopy)
{
    ui->setupUi(this);
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

void SubdirectoryCopy::copyDirectory(const QString src, const QString dst, const int from, const float range)
{
    QDir().mkdir(dst);
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
                    float step = (ui->progressBar->maximum()-ui->progressBar->minimum())/contents.count();
                    ui->progressBar->setValue(ui->progressBar->minimum());
                    ui->comment->setText("");
                    for(int i=0; i<contents.count(); i++)
                    {
                        ui->progressBar->setValue(ui->progressBar->value()+step);
                        if(dstSubdirs.contains(contents[i]))
                        {
                            copyDirectory(srcDir->absoluteFilePath(contents[i]),
                                          dstDir->absoluteFilePath(contents[i]),
                                          ui->progressBar->value(),
                                          step);
                        }
                    }
                    ui->progressBar->setValue(ui->progressBar->maximum());
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
