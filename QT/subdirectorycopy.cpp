#include "subdirectorycopy.h"
#include "ui_subdirectorycopy.h"

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
