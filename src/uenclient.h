#ifndef uenclient_H
#define uenclient_H

#include <QtGui/QMainWindow>
#include <QTableWidget>
#include <QLabel>

class uenclient : public QMainWindow
{
Q_OBJECT
public:
    uenclient();
    virtual ~uenclient();
    QTabWidget *tabWidget;
    void startSession();

    void setJabberJID(QString get){ jabberJID = get; };
    void setJabberPassword(QString get){ jabberPassword = get; };
    void setJabberNick(QString get){ jabberNick = get; };
    void setApiLogin(QString get){ apiLogin = get; };
    void setApiPassword(QString get){ apiPassword = get; };

    QString getJabberJID(){ return jabberJID; };
    QString getJabberPassword(){ return jabberPassword; };
    QString getJabberNick(){ return jabberNick; };
    QString getApiLogin(){ return apiLogin; };
    QString getApiPassword(){ return apiPassword; };
    
    bool isJabberOn;
    bool isTorrentOn;
    bool isSearchOn;
private:
    QString jabberJID;
    QString jabberPassword;
    QString jabberNick;
    QString apiLogin;
    QString apiPassword; // md5sum
    
    QLabel* displayStatus;
public slots:
    void showTorrentCreateDialog();
    void updateServicesStatus();
};

#endif // uenclient_H
