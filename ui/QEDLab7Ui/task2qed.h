#ifndef TASK2QED_H
#define TASK2QED_H

#include <QDialog>

namespace Ui {
class Task2QED;
}

class Task2QED : public QDialog {
  Q_OBJECT

 public:
  explicit Task2QED(QWidget *parent = nullptr);
  ~Task2QED();

 private:
  Ui::Task2QED *ui;
};

#endif  // TASK2QED_H
