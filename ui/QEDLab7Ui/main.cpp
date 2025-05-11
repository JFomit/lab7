#include "qed.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QED w;
  w.show();
  return a.exec();
}
