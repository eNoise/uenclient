#include "uenclient.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QToolBar>

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
    
    QToolBar *bottomBar = new QToolBar(tr("Services status"));
    addToolBar(Qt::BottomToolBarArea, bottomBar);
    displayStatus = new QLabel();
    bottomBar->addWidget(displayStatus);
    isJabberOn = false;
    isSearchOn = false;
    isTorrentOn = false;
    updateServicesStatus();

#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Init main window finished"; 
#endif
    
    LoginForm* login = new LoginForm(this);
    login->exec();
}

void uenclient::startSession()
{
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Start client session"; 
#endif
    tabWidget = new QTabWidget;
    ChatDialog* chat = new ChatDialog(this, jabberJID, jabberPassword, jabberNick);
    tabWidget->addTab(chat, tr("Chat"));
    tabWidget->addTab(new TorrentDialog(this), tr("Transfers"));
    tabWidget->setTabsClosable(true);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), chat, SLOT(onTabClose(int)));
    setCentralWidget(tabWidget);
}

void uenclient::updateServicesStatus()
{
	QString yes = tr("enabled");
	QString no = tr("disabled");
	displayStatus->setText(
				tr("Jabber client: ") + ((isJabberOn) ? yes : no) + "   "
				+ tr("BitTorrent client: ") + ((isTorrentOn) ? yes : no) + "   "
				+ tr("Search client: ") + ((isSearchOn) ? yes : no) + "      "
			      );
}

uenclient::~uenclient()
{}

void uenclient::showTorrentCreateDialog()
{
      (new CreateTorrentDialog())->setVisible(1);
}

#include "uenclient.moc"
