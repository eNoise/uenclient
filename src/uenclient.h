#ifndef uenclient_H
#define uenclient_H

#include <QtGui/QMainWindow>
#include <QTableWidget>

class uenclient : public QMainWindow
{
Q_OBJECT
public:
    uenclient();
    virtual ~uenclient();
    QTabWidget *tabWidget;
private:
public slots:
    void showTorrentCreateDialog();
};

#endif // uenclient_H
