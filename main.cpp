#include <QtGui/QApplication>
#include "uenclient.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    uenclient foo;
    foo.show();
    return app.exec();
}
