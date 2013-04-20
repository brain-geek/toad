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

   int processes = 1;
   if (arguments().length() >= 3)
       processes = arguments().at(2).toInt();

   qDebug()<< QString("Using %1 browser(s).").arg(processes);

  for (int i= 0;i<processes;i++)
  {
    LoadTestingBrowser* br = new LoadTestingBrowser();
    connect(this,SIGNAL(start()), br, SLOT(start()));

    br->setBaseUrl(url);
  }

  emit start();

  return QApplication::exec();
}
