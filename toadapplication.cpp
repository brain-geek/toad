#include "toadapplication.h"
#include "browser.h"
#include "logger.h"

#define BROWSERS_DEFAULT_NUMBER 1
#define MIN_LOAD_SECONDS 0

ToadApplication::ToadApplication(int &argc, char **&argv): QApplication(argc, argv){
};

int ToadApplication::exec() {
   if (arguments().length() < 2) {
       qDebug() << "Wrong arguments received! I expect first argument - URL to start crawling, " <<
                   "optional second - number of browsers in this thread, optional third - number of seconds to bench (by default - indefinite)," <<
                   "optional fourth - path to file to log data.";
       return EXIT_FAILURE;
   };


   // Logger-related
   Logger* logger; bool logger_enabled;

   logger_enabled = (arguments().length() >= 5);

   if (logger_enabled)
   {
       logger = new Logger(arguments().at(4), this);
       qDebug()<< QString("Logs will be put to %1.").arg(arguments().at(4));
   }

   // Defining number of browsers to run
   int browsers = BROWSERS_DEFAULT_NUMBER;
   if (arguments().length() >= 3)
       browsers = arguments().at(2).toInt();

   qDebug()<< QString("Using %1 browser(s).").arg(browsers);

   // Initialising some stuff before loop
   QUrl url;
   Browser* br;
   url = QUrl(arguments().at(1));

  for (int i= 0;i<browsers;i++)
  {
    br = new Browser(this);
    connect(this,SIGNAL(start()), br, SLOT(start()));

    if (logger_enabled)
        logger->addBrowser(br);

    br->setBaseUrl(url);
  }

  // Self destruction :)

  if (arguments().length() >= 4)
  {
      int seconds = arguments().at(3).toInt();

      if (seconds > MIN_LOAD_SECONDS)
      {
          qDebug() << "Load testing will last for " << seconds << " seconds.";
          QTimer::singleShot(seconds * 1000, this, SLOT(quit()));
      } else
      {
          qDebug() << "Load testing will have no time limit.";
      }
  }

  emit start();

  return QApplication::exec();
}
