#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "loadtestingbrowser.h"

class Logger : public QObject
{
    Q_OBJECT
public:
    Logger(QString logging_path);
    void addBrowser(LoadTestingBrowser *);
    
private slots:
    void pageLoaded(qint64 msecs_loaded, QUrl page_url);
    void errorHappened(qint64 msecs_loaded, QUrl page_url);

protected:
    QFile* log_file;
    bool file_opened;
    LoadTestingBrowser* br;
};

#endif // LOGGER_H
