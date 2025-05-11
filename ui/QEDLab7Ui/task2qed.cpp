#include "task2qed.h"
#include "ui_task2qed.h"

Task2QED::Task2QED(QWidget *parent) : QDialog(parent), ui(new Ui::Task2QED) {
  ui->setupUi(this);
}

Task2QED::~Task2QED() {
  delete ui;
}
