#ifndef SUBDIRECTORYCOPY_H
#define SUBDIRECTORYCOPY_H

#include <QDialog>
#include "qerrormessage.h"

namespace Ui {
class SubdirectoryCopy;
}

class SubdirectoryCopy : public QWidget
{
    Q_OBJECT
    
public:
    explicit SubdirectoryCopy(QWidget *parent = 0);
    ~SubdirectoryCopy();

private slots:
    void srcButtonClicked();
    void dstButtonClicked();
    void makeCopy();
    
private:
    Ui::SubdirectoryCopy *ui;
    void fireMessage(const QString &message);
    void copyDirectory(const QString src, const QString dst);
};

#endif // SUBDIRECTORYCOPY_H
