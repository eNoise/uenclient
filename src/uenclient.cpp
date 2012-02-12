#include "uenclient.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QToolBar>
#include <QSettings>

#include "chatdialog.h"
#include "createtorrentdialog.h"
#include "torrentdialog.h"
#include "loginform.h"

uenclient::uenclient()
{
    setWindowTitle(tr("Uruchie eNoise Client"));
    QAction* quit = new QAction(this);
    quit->setText( tr("Quit") );
    quit->setIcon(QIcon(QString(CLIENT_DATA_DIR) + "/icons/exit.png"));
    connect(quit, SIGNAL(triggered()), SLOT(closeThroughtTray()) );
    QMenu* fileMenu = menuBar()->addMenu( "File" );

    //QAction* b = new QAction(this);
    //b->setText(tr("Create Torrent"));
    //connect(b, SIGNAL(triggered()), SLOT(showTorrentCreateDialog()));
    
    //fileMenu->addAction( b );
    
    autoLogin = new QAction(this);
    autoLogin->setCheckable(true);
    autoLogin->setText(tr("Auto-login"));
    connect(autoLogin, SIGNAL(toggled(bool)), SLOT(changedAutoLogin(bool)));
    
    fileMenu->addAction(autoLogin);
    fileMenu->addAction( quit );
    
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
    trayMenu->addAction(quit);
    tray->setContextMenu(trayMenu);
    tray->show();
    
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Init main window finished"; 
#endif
    
    // Session setting
    isSessionStarted = false;
    canStartSession = false;
    
    loginForm = new LoginForm(this);
    loginForm->show();
}

void uenclient::confirmedLogin()
{
	canStartSession = true;
	show();
	startSession();
}

void uenclient::showLoginForm()
{
    if(loginForm == NULL)
	return;
    loginForm->show();
}

void uenclient::show()
{
	if(canStartSession)
		QMainWindow::show();
	else
		close(); // Temporary close session
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

void uenclient::about()
{
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(QString(CLIENT_DATA_DIR) + "/icons/uenlogo_350.png"));
    
    QLabel *text = new QLabel;
    text->setWordWrap(true);
    text->setText(
		tr("UeNClient Version: ")
		+ QString(GIT_VERSION) + " (" + QString(GIT_DESCRIBE) + ") " + " [r" + GIT_REVISION + "]"  
		+ "\n\n"
		+ tr("Developers:\n")
		+ tr(" Alexey DEg Kasyanchuk - main developer (first alpha and later)\n")
		+ tr(" Sergey Happ Svirsky - maintainer and developer (first alpha and later)\n")
		+ tr(" Egor Nagg Bogatov - maintainer and support (first alpha and later)\n")
		+ tr(" Un1c0rn - ideological leader and support (first alpha and later)\n")
		+ tr(" Leto - designer (first alpha and later)\n")
		+ tr(" Rabbid - tester (first alpha and later)\n")
		+ tr(" Jelu - tester (first alpha and later)\n")
    );

    QPushButton *quitButton = new QPushButton("OK");

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setMargin(15);
    topLayout->setSpacing(15);
    topLayout->addWidget(icon);
    topLayout->addWidget(text);
    topLayout->setAlignment(text,Qt::AlignTop);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(quitButton);
    bottomLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    QDialog about(this);
    about.setModal(true);
    about.setWindowTitle(tr("About Uruchie eNoise Client"));
    about.setLayout(mainLayout);

    connect(quitButton, SIGNAL(clicked()), &about, SLOT(close()));

    about.exec();
}

void uenclient::startSession()
{
    if(!isSessionStarted)
    {
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Start client session"; 
#endif
	      isSessionStarted = true;
	      tabWidget = new QTabWidget;
	      ChatDialog* chat = new ChatDialog(this, jabberJID, jabberPassword, jabberNick);
	      tabWidget->addTab(chat, tr("Chat"));
	      TorrentDialog* torrent = new TorrentDialog(this); 
	      tabWidget->addTab(torrent, tr("Transfers"));
	      tabWidget->setTabsClosable(true);
	      connect(tabWidget, SIGNAL(tabCloseRequested(int)), chat, SLOT(onTabClose(int)));
	      setCentralWidget(tabWidget);
	      
	      // add about menu
	      // Help menu
	      QMenu *helpMenu = this->menuBar()->addMenu(tr("&Help"));
	      helpMenu->addAction(tr("&About"), this, SLOT(about()));
	      //helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
    }
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

void uenclient::changedAutoLogin(bool state)
{
	QSettings settings("eNoise", "UeNclient");
	settings.setValue("IsAutoLogin", state);
	settings.sync();
}

#include "uenclient.moc"
