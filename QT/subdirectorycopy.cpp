#include "subdirectorycopy.h"
#include "ui_subdirectorycopy.h"
#include "qfiledialog.h"

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
