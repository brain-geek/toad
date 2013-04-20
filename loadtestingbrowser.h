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
    void startTest(QUrl);
    void restartTest(QString error_message);
    
signals:
    
public slots:
    void restartByTimer();
    void loadFinished ( bool ok);
    void loadProgress ( int progress);
    void linkClicked  ( const QUrl&);

protected:
    QWebPage* page;
    QTimer* timer;
    QUrl base_url;
};

#endif // LOADTESTINGBROWSER_H
