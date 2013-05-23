#include "browser.h"

Browser::Browser(QObject *parent) :
    QObject(parent)
{
    page = new QWebPage(this);
    page->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);

    timeout_countdown = new QTimer(this);
    timeout_countdown->setInterval(10000);
    timeout_countdown->setSingleShot(true);

    connect(timeout_countdown, SIGNAL(timeout()), this, SLOT(restartByTimer()));

    connect(page, SIGNAL(loadFinished(bool)),this, SLOT(loadFinished(bool)));
    connect(page, SIGNAL(loadProgress(int)),this, SLOT(loadProgress(int)));
    connect(page, SIGNAL(linkClicked(const QUrl&)),this, SLOT(linkClicked(const QUrl&)));
}

void Browser::restartTest(QString error_message = "") {
    if (error_message.length() > 0)
        qDebug() << this << QString("Restarting at %1 with message '%2'.").arg(QTime::currentTime().toString("H:m:s")).arg(error_message);
    else
        qDebug() << this << QString("Starting at %1").arg(QTime::currentTime().toString("H:m:s"));

    this->last_url = QUrl("http://non-existant");
    page->mainFrame()->load(base_url);

    timeout_countdown->start();
    page_load_time.start();
}

void Browser::setBaseUrl(QUrl url) {
    this->base_url = url;
}

void Browser::loadFinished(bool ok) {
    QUrl current_url(page->currentFrame()->baseUrl());

    // events trottling
    if (current_url == this->last_url)
    {
        qDebug() << this << QString("%1, loadFinished trottled.").arg(QDateTime::currentDateTime().toTime_t());
        return;
    }

    this->last_url = current_url;

    qDebug() << this << QString("%1, '%2', %3 ms.").arg(QDateTime::currentDateTime().toTime_t()).arg(current_url.toString()).arg(page_load_time.elapsed());

    if (!ok) {
        emit(errorHappened(page_load_time.elapsed(), page->currentFrame()->baseUrl()));
        restartTest(QString("its not ok, something wrong happened! (possibly 500) URL: %1").arg(page->currentFrame()->baseUrl().toString()));
        return;
     }

    QWebElement document= page->currentFrame()->documentElement();

    if (page->currentFrame()->baseUrl().host() != base_url.host())
    {
       restartTest("We've got outside the test site. Restarting.");
       return;
    }

    emit(pageLoaded(page_load_time.elapsed(), page->currentFrame()->baseUrl()));

    QWebElementCollection collection = document.findAll("a[href]");
    QWebElement link; QString link_href;

    if (collection.count() == 0)
    {
        restartTest("No links found on page. Restarting.");
        return;
    }

    // Ignoring javascript and empty links
    do
    {
       link = collection.at(qrand()%collection.count());
       link_href = link.attribute("href");
    }
    while ((link_href.count() == 0) || (link_href.at(0) == '#') || (link_href.contains("javascript:;")));

    link.evaluateJavaScript("var evObj = document.createEvent('MouseEvents');evObj.initEvent( 'click', true, true );this.dispatchEvent(evObj);");

    page_load_time.start();
    timeout_countdown->start();
}

void Browser::restartByTimer() {
    restartTest("Restarting by timer.");
}

void Browser::loadProgress(int) {
    timeout_countdown->start();
}

void Browser::linkClicked(const QUrl & url) {
    restartTest(QString("We've got some problem - unknown link click handled! Link: ").append(url.toString()));
}

void Browser::start() {
    restartTest();
}
