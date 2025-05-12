#include "qed.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  srandom(time(nullptr));

  QApplication a(argc, argv);
  QED w;
  w.show();
  return a.exec();
}
