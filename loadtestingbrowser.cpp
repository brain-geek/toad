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
    qDebug() << "Restarting at " << QTime::currentTime() << " with message " << error_message;

    QUrl url(base_url);
    page->mainFrame()->load(url);

    timer->start();
}

void LoadTestingBrowser::startTest(QUrl url) {
    this->base_url = url;
    page->mainFrame()->load(url);
}

void LoadTestingBrowser::loadFinished(bool ok) {
    qDebug() << "Opened page: " << page->currentFrame()->title();

    if (!ok) {
        restartTest(QString("its not ok, something wrong happened! (possibly 500) URL: %1").arg(page->currentFrame()->baseUrl().toString()));
     }

    QWebElement document= page->currentFrame()->documentElement();
    QWebElementCollection collection = document.findAll("a[href]");

    QWebElement link;

    if (collection.count() == 0)
        restartTest("No links found on page. Restarting.");

    if (page->currentFrame()->baseUrl().host() != base_url.host())
        restartTest("We've got outside the test site. Restarting.");


    QString link_href;

    // Ignoring javascript and empty links
    do
    {
       link = collection.at(qrand()%collection.count());
       link_href = link.attribute("href");

       qDebug() << "Trying link with href" << link_href;
    }
    while ((link_href.count() == 0) || (link_href.at(0) == '#') || (link_href.contains("javascript:;")));

    qDebug() << "Opening link";
    link.evaluateJavaScript("var evObj = document.createEvent('MouseEvents');evObj.initEvent( 'click', true, true );this.dispatchEvent(evObj);");
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
