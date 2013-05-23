#ifndef LOADTESTINGBROWSER_H
#define LOADTESTINGBROWSER_H

#include <QDebug>
#include <QtWebKit>
#include <QWebElementCollection>
#include <QTimer>

class Browser : public QObject
{
    Q_OBJECT
public:
    explicit Browser(QObject *parent = 0);
    void setBrowserObject(QWebPage* page);
    void restartTest(QString error_message);
    void setBaseUrl(QUrl);
    
signals:
    void pageLoaded(qint64 msecs_loaded, QUrl page_url);
    void errorHappened(qint64 msecs_loaded, QUrl page_url);
    
public slots:
    void start();
    void restartByTimer();
    void loadFinished ( bool ok);
    void loadProgress ( int progress);
    void linkClicked  ( const QUrl&);

protected:
    QWebPage* page;
    QTimer* timeout_countdown;
    QElapsedTimer page_load_time;
    QUrl base_url;

    QUrl last_url;
};

#endif // LOADTESTINGBROWSER_H
