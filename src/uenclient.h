#ifndef uenclient_H
#define uenclient_H

#include <QtGui/QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QTimer>

class LoginForm;
class GlooxSession;

class uenclient : public QMainWindow
{
Q_OBJECT
public:
    uenclient();
    virtual ~uenclient();
    QTabWidget *tabWidget;
    void startSession();
    void confirmedLogin();
    void show();
    
    void setJabberJID(QString get){ jabberJID = get; };
    void setJabberPassword(QString get){ jabberPassword = get; };
    void setJabberNick(QString get){ jabberNick = get; };
    void setApiLogin(QString get){ apiLogin = get; };
    void setApiPassword(QString get){ apiPassword = get; };
    void setApiId(QString get){ apiId = get; };

    QString getJabberJID(){ return jabberJID; };
    QString getJabberPassword(){ return jabberPassword; };
    QString getJabberNick(){ return jabberNick; };
    QString getApiLogin(){ return apiLogin; };
    QString getApiPassword(){ return apiPassword; };
    QString getApiId(){ return apiId; };
    
    bool isJabberOn;
    bool isTorrentOn;
    bool isSearchOn;
    bool canStartSession;
    QAction* autoLogin;
protected:
    void closeEvent(QCloseEvent *event);
private:
    bool isSessionStarted;
    QString jabberJID;
    QString jabberPassword;
    QString jabberNick;
    QString apiLogin;
    QString apiPassword; // md5sum
    QString apiId;
    
    QLabel* displayStatus;
    QSystemTrayIcon* tray;
    QTimer* trayBlinkTimer;
    bool isNewMessages;
    bool blinkMessageNow;
    
    LoginForm* loginForm;
    friend class GlooxSession;
public slots:
    void showTorrentCreateDialog();
    void updateServicesStatus();
    void trayAction(QSystemTrayIcon::ActivationReason reason);
    void closeThroughtTray();
    void changedAutoLogin(bool state);
    
    void showLoginForm();
    void trayBlink();
    void setTrayBlink(bool isBlink = false);
private slots:
    void about();
};

#endif // uenclient_H
