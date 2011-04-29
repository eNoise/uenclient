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
    a->setIcon(QIcon(QString(CLIENT_DATA_DIR) + "/icons/exit.png"));
    connect(a, SIGNAL(triggered()), SLOT(closeThroughtTray()) );
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

    //Tray icon
    tray = new QSystemTrayIcon(this);
    tray->setToolTip(tr("UeNClient tray menu"));
    tray->setIcon(QIcon(QString(CLIENT_DATA_DIR) + "/icons/uenicon.png"));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayAction(QSystemTrayIcon::ActivationReason)));
    QMenu* trayMenu = new QMenu(this);
    trayMenu->addAction(a);
    tray->setContextMenu(trayMenu);
    tray->show();
    
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Init main window finished"; 
#endif
    
    LoginForm* login = new LoginForm(this);
    if(login->exec() == QDialog::Accepted)
		startSession();
    else
		isSession = false;
}

void uenclient::show()
{
	if(isSession)
		QMainWindow::show();
	else
		close(); // broken session
}

void uenclient::closeEvent(QCloseEvent* event)
{
    if(tray->isVisible())
    {
	    hide();
	    event->ignore();
    }
}

void uenclient::closeThroughtTray()
{
	tray->hide();
	close();
}

void uenclient::trayAction(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
		case QSystemTrayIcon::DoubleClick:
		{
			if(this->isVisible())
				hide();
			else
				show();
			break;
		}
		default:
		;
	}
}


void uenclient::startSession()
{
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Start client session"; 
#endif
    isSession = true;
    tabWidget = new QTabWidget;
    ChatDialog* chat = new ChatDialog(this, jabberJID, jabberPassword, jabberNick);
    tabWidget->addTab(chat, tr("Chat"));
    TorrentDialog* torrent = new TorrentDialog(this); 
    tabWidget->addTab(torrent, tr("Transfers"));
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
