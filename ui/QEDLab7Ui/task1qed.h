#ifndef TASK1QED_H
#define TASK1QED_H

#include <QDialog>
#include "dirty_wrapper.h"
#include "list.h"

namespace Ui {
class Task1QED;
}

class Task1QED : public QDialog {
  Q_OBJECT

 public:
  explicit Task1QED(QWidget *parent = nullptr);
  ~Task1QED() override;

 public slots:
  void on_pushButton_clicked();
  void UpdateA();
  void UpdateB();

  void on_add_a_clicked();
  void on_add_b_clicked();

  void on_remove_a_clicked();
  void on_remove_a_2_clicked();

 private:
  lab::DirtyWrapper<lab::List<int>> a_;
  lab::DirtyWrapper<lab::List<int>> b_;

  Ui::Task1QED *ui_;
};

#endif  // TASK1QED_H
