#include "uenclient.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

#include "chatdialog.h"
#include "createtorrentdialog.h"
#include "torrentdialog.h"
#include "loginform.h"

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
    
    LoginForm* login = new LoginForm(this);
    login->exec();
}

void uenclient::startSession()
{
    tabWidget = new QTabWidget;
    ChatDialog* chat = new ChatDialog(this, jabberJID, jabberPassword, jabberNick);
    tabWidget->addTab(chat, tr("Chat"));
    tabWidget->addTab(new TorrentDialog(this), tr("Transfers"));
    tabWidget->setTabsClosable(true);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), chat, SLOT(onTabClose(int)));
    setCentralWidget(tabWidget);
}

uenclient::~uenclient()
{}

void uenclient::showTorrentCreateDialog()
{
      (new CreateTorrentDialog())->setVisible(1);
}

#include "uenclient.moc"
