#include "task2qed.h"
#include <qobject.h>
#include <qtablewidget.h>
#include <qtreewidget.h>
#include <array>
#include <cstdlib>
#include "dirty_wrapper.h"
#include "tree.h"
#include "ui_task2qed.h"

Task2QED::Task2QED(QWidget *parent) : QDialog(parent), ui_(new Ui::Task2QED) {
  ui_->setupUi(this);
  ui_->tableWidget->clearContents();
  ui_->tableWidget->setColumnWidth(
      1, ui_->tableWidget->width() - ui_->tableWidget->columnWidth(0));
  connect(map_.GetInvoker(), &lab::QObjectDirtyWrapper::WasDirty, this,
          &Task2QED::UpdateTree);
}

Task2QED::~Task2QED() {
  delete ui_;
}

static const auto kNames =
    std::to_array({"Tim", "Tom", "GarbAge", "Thy_end_is_now!", "Neo",
                   "Mr Smith", "Blake3", "Blaze3D"});

void Task2QED::on_nonsense_clicked() {
  ui_->tableWidget->clear();
  ui_->tableWidget->setRowCount(0);

  for (size_t i = 0; i < 10; ++i) {
    auto row = ui_->tableWidget->rowCount();
    ui_->tableWidget->setRowCount(row + 1);
    auto *name = kNames[random() % kNames.size()];
    ui_->tableWidget->setItem(
        row, 0, new QTableWidgetItem(QString{"%1"}.arg(random())));
    ui_->tableWidget->setItem(row, 1, new QTableWidgetItem(name));
  }
}

void Task2QED::on_toTree_clicked() {
  auto map = map_.AsMut();
  map.Get().Clear();
  for (int i = 0; i < ui_->tableWidget->rowCount(); ++i) {
    auto key = ui_->tableWidget->item(i, 0);
    auto value = ui_->tableWidget->item(i, 1);
    map.Get().Insert(key->text().toInt(), value->text());
  }
}

void Task2QED::UpdateTree() {
  map_.AsConst().Get().PopulateTreeWidget(ui_->treeWidget);
  map_.Clear();
  ui_->treeWidget->expandAll();

  auto count = map_.AsConst().Get().Count2Children();
  ui_->lineEdit->setText(QString{"%1"}.arg(count));

  ui_->pre_line->setText(
      map_.AsConst().Get().Fold<lab::TreeTraversal::kPreOrder>(
          QString(), [](QString seed, const auto &value) {
            return seed.append(' ').append(value);
          }));
  ui_->post_line->setText(
      map_.AsConst().Get().Fold<lab::TreeTraversal::kPostOrder>(
          QString(), [](QString seed, const auto &value) {
            return seed.append(' ').append(value);
          }));
  ui_->in_line->setText(map_.AsConst().Get().Fold<lab::TreeTraversal::kInOrder>(
      QString(), [](QString seed, const auto &value) {
        return seed.append(' ').append(value);
      }));
}

void CustomTreeMap::TraverseTree(QTreeWidgetItem *parent,
                                 const CustomTreeMap::Node *const node) const {
  auto *self = new QTreeWidgetItem();
  self->setText(0, QString{"%1: %2"}.arg(node->key).arg(node->data));
  parent->addChild(self);

  if (node->left != nullptr) {
    TraverseTree(self, node->left);
  }
  if (node->right != nullptr) {
    TraverseTree(self, node->right);
  }
}

void CustomTreeMap::PopulateTreeWidget(QTreeWidget *widget) const {
  widget->clear();
  auto widget_root = widget->invisibleRootItem();

  TraverseTree(widget_root, Root());
}

void Task2QED::on_balance_clicked() {
  auto map = map_.AsMut();
  map.Get().Balance();
}
int CustomTreeMap::Count2Children() const {
  auto count = FoldNodes(
      0,
      [](int seed, const CustomTreeMap::Node *node) {
        if (node->left != nullptr && node->right != nullptr) {
          return seed + 1;
        }

        return seed;
      },
      Root());

  return count;
}
