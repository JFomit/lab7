#include "qed.h"
#include "task1qed.h"
#include "ui_qed.h"

QED::QED(QWidget *parent) : QMainWindow(parent), ui_(new Ui::QED) {
  ui_->setupUi(this);
}

QED::~QED() {
  delete ui_;
}

void QED::on_actionExit_triggered() {
  QApplication::exit();
}

void QED::on_actionTask_1_triggered() {
  auto *dialog = new Task1QED;
  dialog->exec();
}
