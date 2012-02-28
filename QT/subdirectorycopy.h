#ifndef SUBDIRECTORYCOPY_H
#define SUBDIRECTORYCOPY_H

#include <QDialog>

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
    
private:
    Ui::SubdirectoryCopy *ui;
};

#endif // SUBDIRECTORYCOPY_H
