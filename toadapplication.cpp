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

  LoadTestingBrowser* br = new LoadTestingBrowser(this);

  br->startTest(url, url.host());

  return QApplication::exec();
}
