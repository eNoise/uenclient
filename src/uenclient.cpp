#include "uenclient.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

#include "chatdialog.h"
#include "createtorrentdialog.h"

uenclient::uenclient()
{
    setWindowTitle(tr("UeN Client"));
    QAction* a = new QAction(this);
    a->setText( tr("Quit") );
    connect(a, SIGNAL(triggered()), SLOT(close()) );
    QMenu* fileMenu = menuBar()->addMenu( "File" );

    QAction* b = new QAction(this);
    b->setText(tr("Create Torrent"));
    connect(b, SIGNAL(triggered()), SLOT(showTorrentCreateDialog()));
    
    fileMenu->addAction( b );
    fileMenu->addAction( a );
        
    tabWidget = new QTabWidget;
    tabWidget->addTab(new ChatDialog(this), tr("Chat"));
    setCentralWidget(tabWidget);
}

uenclient::~uenclient()
{}

void uenclient::showTorrentCreateDialog()
{
      (new CreateTorrentDialog())->setVisible(1);
}

#include "uenclient.moc"
