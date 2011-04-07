#include "uenclient.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

#include "chatdialog.h"

uenclient::uenclient()
{
    setWindowTitle(tr("UeN Client"));
    QAction* a = new QAction(this);
    a->setText( "Quit" );
    connect(a, SIGNAL(triggered()), SLOT(close()) );
    menuBar()->addMenu( "File" )->addAction( a );
    
    tabWidget = new QTabWidget;
    tabWidget->addTab(new ChatDialog(), tr("Chat"));
    setCentralWidget(tabWidget);
}

uenclient::~uenclient()
{}

#include "uenclient.moc"
