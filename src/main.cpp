#include <QtGui/QApplication>
#include "uenclient.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    uenclient uenClient;
    uenClient.show();
    
    return app.exec();
}
