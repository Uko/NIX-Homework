#include <QtGui/QApplication>
#include "subdirectorycopy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SubdirectoryCopy w;
    w.show();
    
    return a.exec();
}
