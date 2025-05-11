#include "task3qed.h"
#include "ui_task3qed.h"

Task3QED::Task3QED(QWidget *parent) : QDialog(parent), ui(new Ui::Task3QED) {
  ui->setupUi(this);
}

Task3QED::~Task3QED() {
  delete ui;
}
