#include "qed.h"
#include "ui_qed.h"

QED::QED(QWidget *parent) : QMainWindow(parent), ui_(new Ui::QED) {
  ui_->setupUi(this);
}

QED::~QED() {
  delete ui_;
}
