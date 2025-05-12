#include "task3qed.h"
#include <cstdlib>
#include "dirty_wrapper.h"
#include "ui_task3qed.h"

Task3QED::Task3QED(QWidget *parent) : QDialog(parent), ui_(new Ui::Task3QED) {
  ui_->setupUi(this);
  connect(map_.GetInvoker(), &lab::QObjectDirtyWrapper::WasDirty, this,
          &Task3QED::UpdateSet);
}

void Task3QED::UpdateSet() {
  map_.Clear();
}

Task3QED::~Task3QED() {
  delete ui_;
}

void Task3QED::on_addButton_clicked() {
  auto map = map_.AsMut();

  map.Get().Insert((int)random());
}

// remove
void Task3QED::on_pushButton_2_clicked() {
  if (map_.AsConst().Get().Empty()) {
    return;
  }

  auto map = map_.AsMut();
}
