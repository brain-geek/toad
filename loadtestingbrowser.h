#ifndef LOADTESTINGBROWSER_H
#define LOADTESTINGBROWSER_H

#include <QDebug>
#include <QtWebKit>
#include <QWebElementCollection>
#include <QTimer>

class LoadTestingBrowser : public QObject
{
    Q_OBJECT
public:
    explicit LoadTestingBrowser(QObject *parent = 0);
    void setBrowserObject(QWebPage* page);
    void restartTest(QString error_message);
    void setBaseUrl(QUrl);
    
signals:
    
public slots:
    void start();
    void restartByTimer();
    void loadFinished ( bool ok);
    void loadProgress ( int progress);
    void linkClicked  ( const QUrl&);

protected:
    QWebPage* page;
    QTimer* timer;
    QElapsedTimer page_load_time;
    QUrl base_url;
};

#endif // LOADTESTINGBROWSER_H
