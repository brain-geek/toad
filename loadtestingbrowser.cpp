#include "loadtestingbrowser.h"

LoadTestingBrowser::LoadTestingBrowser(QObject *parent) :
    QObject(parent)
{
    page = new QWebPage(this);
    page->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);

    timer = new QTimer(this);
    timer->setInterval(10000);
    timer->setSingleShot(true);

    connect(timer, SIGNAL(timeout()), this, SLOT(restartByTimer()));

    connect(page, SIGNAL(loadFinished(bool)),this, SLOT(loadFinished(bool)));
    connect(page, SIGNAL(loadProgress(int)),this, SLOT(loadProgress(int)));
    connect(page, SIGNAL(linkClicked(const QUrl&)),this, SLOT(linkClicked(const QUrl&)));
}

void LoadTestingBrowser::restartTest(QString error_message = "") {
    if (error_message.length() > 0)
        qDebug() << QString("Restarting at %1 with message '%2'.").arg(QTime::currentTime().toString("H:m:s")).arg(error_message);
    else
        qDebug() << QString("(RE)Starting at %1").arg(QTime::currentTime().toString("H:m:s"));

    page->mainFrame()->load(base_url);

    timer->start();
    page_load_time.start();
}

void LoadTestingBrowser::setBaseUrl(QUrl url) {
    this->base_url = url;
}

void LoadTestingBrowser::loadFinished(bool ok) {
    qDebug() << QString("Thread: %1: Opened page with by URL '%2'. It took %3 miliseconds.").arg(QThread::currentThreadId()).arg(page->currentFrame()->baseUrl().toString()).arg(page_load_time.elapsed());

    if (!ok) {
        restartTest(QString("its not ok, something wrong happened! (possibly 500) URL: %1").arg(page->currentFrame()->baseUrl().toString()));
        return;
     }

    QWebElement document= page->currentFrame()->documentElement();
    QWebElementCollection collection = document.findAll("a[href]");

    QWebElement link;

    if (collection.count() == 0)
    {
        restartTest("No links found on page. Restarting.");
        return;
    }

    if (page->currentFrame()->baseUrl().host() != base_url.host())
    {
       restartTest("We've got outside the test site. Restarting.");
       return;
    }

    QString link_href;

    // Ignoring javascript and empty links
    do
    {
       link = collection.at(qrand()%collection.count());
       link_href = link.attribute("href");

//       qDebug() << "Trying link with href" << link_href;
    }
    while ((link_href.count() == 0) || (link_href.at(0) == '#') || (link_href.contains("javascript:;")));

//    qDebug() << "Opening link";
    link.evaluateJavaScript("var evObj = document.createEvent('MouseEvents');evObj.initEvent( 'click', true, true );this.dispatchEvent(evObj);");

    page_load_time.start();
    timer->start();
}

void LoadTestingBrowser::restartByTimer() {
    restartTest("Restarting by timer.");
}

void LoadTestingBrowser::loadProgress(int) {
    timer->start();
}

void LoadTestingBrowser::linkClicked(const QUrl & url) {
    restartTest(QString("We've got some problem - unknown link click handled! Link: ").append(url.toString()));
}

void LoadTestingBrowser::start() {
    restartTest();
}
