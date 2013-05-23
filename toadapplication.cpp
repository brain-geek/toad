#include "toadapplication.h"
#include "browser.h"
#include "logger.h"

ToadApplication::ToadApplication(int &argc, char **&argv): QApplication(argc, argv){
};

int ToadApplication::exec() {
   if (arguments().length() < 2) {
       qDebug() << "Wrong arguments received! I expect first argument - URL to start crawling, optional second - number of browsers in this thread. And optional third - path to file to log data.";
       return EXIT_FAILURE;
   };

   QUrl url;

   Browser* br;
   Logger* logger;

   if (arguments().length() >= 4)
       logger = new Logger(arguments().at(3), this);

   url = QUrl(arguments().at(1));

   int processes = 1;
   if (arguments().length() >= 3)
       processes = arguments().at(2).toInt();

   qDebug()<< QString("Using %1 browser(s).").arg(processes);

  for (int i= 0;i<processes;i++)
  {
    br = new Browser(this);
    connect(this,SIGNAL(start()), br, SLOT(start()));

    if (arguments().length() >= 4)
        logger->addBrowser(br);

    br->setBaseUrl(url);
  }

  emit start();

  return QApplication::exec();
}
