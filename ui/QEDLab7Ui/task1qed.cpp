#include "task1qed.h"
#include <qobject.h>
#include <ctime>
#include "dirty_wrapper.h"
#include "ui_task1qed.h"

Task1QED::Task1QED(QWidget *parent) : QDialog(parent), ui_(new Ui::Task1QED) {
  ui_->setupUi(this);
  connect(a_.GetInvoker(), &lab::QObjectDirtyWrapper::WasDirty, this,
          &Task1QED::UpdateA);
  connect(b_.GetInvoker(), &lab::QObjectDirtyWrapper::WasDirty, this,
          &Task1QED::UpdateB);

  srandom(time(nullptr));
}

Task1QED::~Task1QED() {
  delete ui_;
}

void Task1QED::on_pushButton_clicked() {
  auto a = a_.AsMut();
  auto b = b_.AsMut();
  if (a.Get().empty()) {
    return;
  }

  auto min = a.Get().min();
  auto max = a.Get().max();

  if (a.Get().is_forward_reachable(min, max)) {
    a.Get().replace_range(min, max, b.Get());
  } else {
    a.Get().replace_range(max, min, b.Get());
  }

  b.Get().clear();
}

void Task1QED::UpdateA() {
  ui_->listWidget->clear();
  auto &l = a_.AsConst().Get();
  l.for_each(
      [this](auto item) { ui_->listWidget->addItem(QString{"%1"}.arg(item)); });
  auto *min = l.min();
  ui_->lineEdit->setText(min == nullptr ? "<null>"
                                        : QString{"%1"}.arg(min->item));
  auto *max = l.max();
  ui_->lineEdit_2->setText(max == nullptr ? "<null>"
                                          : QString{"%1"}.arg(max->item));
  a_.Clear();
}
void Task1QED::UpdateB() {
  ui_->listWidget_2->clear();
  auto &l = b_.AsConst().Get();
  l.for_each([this](auto item) {
    ui_->listWidget_2->addItem(QString{"%1"}.arg(item));
  });
  b_.Clear();
}
void Task1QED::on_add_a_clicked() {
  auto rnd = (int)random();
  auto wrapper = a_.AsMut();
  wrapper.Get().push_back(rnd);
}

void Task1QED::on_add_b_clicked() {
  auto rnd = (int)random();
  auto wrapper = b_.AsMut();
  wrapper.Get().push_back(rnd);
}

void Task1QED::on_remove_a_clicked() {
  if (a_.AsConst().Get().empty()) {
    return;
  }
  auto a = a_.AsMut();
  a.Get().pop_front();
}

void Task1QED::on_remove_a_2_clicked() {
  if (b_.AsConst().Get().empty()) {
    return;
  }

  auto b = b_.AsMut();
  b.Get().pop_back();
}
