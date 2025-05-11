#ifndef TASK3QED_H
#define TASK3QED_H

#include <QDialog>

namespace Ui {
class Task3QED;
}

class Task3QED : public QDialog {
  Q_OBJECT

 public:
  explicit Task3QED(QWidget *parent = nullptr);
  ~Task3QED();

 private:
  Ui::Task3QED *ui;
};

#endif  // TASK3QED_H
