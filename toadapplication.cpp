#include "toadapplication.h"
#include "loadtestingbrowser.h"

ToadApplication::ToadApplication(int &argc, char **&argv): QApplication(argc, argv){
};

int ToadApplication::exec() {
    if (arguments().length() < 2) {
        qDebug() << "Wrong arguments received! I expect one arguments - URL to start crawling.";
        return EXIT_FAILURE;
    };

  QUrl url = QUrl(arguments().at(1));

  for (int i= 0;i<7;i++)
  {
    QThread* t = new QThread;
    t->start();

    LoadTestingBrowser* br = new LoadTestingBrowser();
    connect(this,SIGNAL(start()), br, SLOT(start()));

    br->setBaseUrl(url);
    br->moveToThread(t);
  }

  emit start();

  return QApplication::exec();
}
