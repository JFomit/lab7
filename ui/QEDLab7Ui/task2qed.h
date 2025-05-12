#ifndef TASK2QED_H
#define TASK2QED_H

#include <qobject.h>
#include <qtreewidget.h>
#include <QDialog>
#include "dirty_wrapper.h"
#include "tree.h"

namespace Ui {
class Task2QED;
}

class CustomTreeMap : public lab::TreeMap<int, QString> {
 public:
  void PopulateTreeWidget(QTreeWidget *widget) const;
  void TraverseTree(QTreeWidgetItem *current,
                    const CustomTreeMap::Node *const node) const;
  [[nodiscard]] int Count2Children() const;
};

class Task2QED : public QDialog {
  Q_OBJECT

 public:
  explicit Task2QED(QWidget *parent = nullptr);
  ~Task2QED() override;
 public slots:

 private slots:
  void on_nonsense_clicked();
  void on_toTree_clicked();
  void on_balance_clicked();
  void UpdateTree();

 private:
  Ui::Task2QED *ui_;
  lab::DirtyWrapper<CustomTreeMap> map_;
};

#endif  // TASK2QED_H
