#include "task1qed.h"
#include "ui_task1qed.h"

Task1QED::Task1QED(QWidget *parent) : QDialog(parent), ui(new Ui::Task1QED) {
  ui->setupUi(this);
}

Task1QED::~Task1QED() {
  delete ui;
}
