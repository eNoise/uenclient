
#ifndef TORRENTDIALOG_H
#define TORRENTDIALOG_H

#include <QList>
#include <QStringList>
#include <QWidget>
#include <QTreeWidget>
#include <QItemDelegate>
#include <QDragMoveEvent>
#include <QPainter>

QT_BEGIN_NAMESPACE
class QAction;
class QCloseEvent;
class QLabel;
class QProgressDialog;
class QSlider;
QT_END_NAMESPACE

#include "torrentclient.h"

// TorrentView extends QTreeWidget to allow drag and drop.
class TorrentView : public QTreeWidget
{
    Q_OBJECT
public:
    TorrentView(QWidget *parent = 0);

signals:
    void fileDropped(const QString &fileName);

protected:
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};


class TorrentDialog : public QWidget
{
    Q_OBJECT
public:
    TorrentDialog(QWidget *parent = 0);

    QSize sizeHint() const;
    const TorrentClient *clientForRow(int row) const;
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void loadSettings();
    void saveSettings();

    bool addTorrent();
    void removeTorrent();
    void pauseTorrent();
    void moveTorrentUp();
    void moveTorrentDown();

    void torrentStopped();
    void torrentError(TorrentClient::Error error);

    void updateState(TorrentClient::State state);
    void updatePeerInfo();
    void updateProgress(int percent);
    void updateDownloadRate(int bytesPerSecond);
    void updateUploadRate(int bytesPerSecond);

    void setUploadLimit(int bytes);
    void setDownloadLimit(int bytes);

    void about();
    void setActionsEnabled();
    void acceptFileDrop(const QString &fileName);
    
private:
    int rowOfClient(TorrentClient *client) const;
    bool addTorrent(const QString &fileName, const QString &destinationFolder,
                    const QByteArray &resumeState = QByteArray());
    
    TorrentView *torrentView;
    TorrentView *fastView;
    QAction *pauseTorrentAction;
    QAction *removeTorrentAction;
    QAction *upActionTool;
    QAction *downActionTool;
    QSlider *uploadLimitSlider;
    QSlider *downloadLimitSlider;
    QLabel *uploadLimitLabel;
    QLabel *downloadLimitLabel;

    int uploadLimit;
    int downloadLimit;

    struct Job 
    {
        TorrentClient *client;
        QString torrentFileName;
        QString destinationDirectory;
    };
    QList<Job> jobs;
    int jobsStopped;
    int jobsToStop;

    QString lastDirectory;
    QProgressDialog *quitDialog;

    bool saveChanges;
};

// TorrentViewDelegate is used to draw the progress bars.
class TorrentViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    enum ItemRole
    {
	    torrentClient = Qt::UserRole + 200, 
    };
  
    inline TorrentViewDelegate(TorrentDialog* mainWindow) : QItemDelegate(mainWindow) {}
    inline void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};


#endif
