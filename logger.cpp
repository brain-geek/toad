#include "logger.h"

Logger::Logger(QString logging_path) :
    QObject(br)
{
    this->log_file = new QFile(logging_path);
    this->file_opened = this->log_file->open(QFile::WriteOnly);

    if (this->file_opened) {
        QTextStream stream(this->log_file);
        stream << "Msecs taken to process request;URL" << endl;
    } else
    {
        qDebug() << "Unable to use logger!";
    }
}

void Logger::addBrowser(Browser * br) {
    connect(br, SIGNAL(pageLoaded(qint64, QUrl)), this, SLOT(pageLoaded(qint64,QUrl)));
}

void Logger::pageLoaded(qint64 msecs_loaded, QUrl page_url) {
    QTextStream stream(this->log_file);
    stream << msecs_loaded << ";" << page_url.toString() << endl;
}

void Logger::errorHappened(qint64, QUrl) { }
