#ifndef TASK1QED_H
#define TASK1QED_H

#include <QDialog>

namespace Ui {
class Task1QED;
}

class Task1QED : public QDialog {
  Q_OBJECT

 public:
  explicit Task1QED(QWidget *parent = nullptr);
  ~Task1QED();

 private:
  Ui::Task1QED *ui;
};

#endif  // TASK1QED_H
