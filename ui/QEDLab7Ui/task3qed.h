#ifndef TASK3QED_H
#define TASK3QED_H

#include <qlistwidget.h>
#include <QDialog>
#include <optional>
#include "dirty_wrapper.h"
#include "hashtable.h"

namespace Ui {
class Task3QED;
}

class HashSet : public lab::HashTable<int, int> {
 public:
  void Insert(int x) {
    lab::HashTable<int, int>::Insert(x, std::forward<int>(x));
  }
  bool Remove(int x) { return lab::HashTable<int, int>::Remove(x); }
  [[nodiscard]] std::optional<int> Get(int x) const {
    auto &ref = lab::HashTable<int, int>::Get(x);
    if (ref.HasValue()) {
      return ref.Get();
    } else {
      return std::nullopt;
    }
  }

  void RemoveEven() {
    auto *s = const_cast<Slot *>(storage());
    for (size_t i = 0; i < capacity(); ++i) {
      if (s[i].status == Status::kOccupied && s[i].GetKey() % 2 == 0) {
        s[i].status = Status::kTombstone;
        s[i].Destroy();
      }
    }
  }

  void PopulateBuckets(QListWidget *widget) const {
    widget->clear();

    const auto *buckets = storage();
    for (size_t i = 0; i < capacity(); ++i) {
      if (buckets[i].status == HashSet::Status::kFree) {
        widget->addItem("<free>");
      } else if (buckets[i].status == HashSet::Status::kTombstone) {
        widget->addItem(">RIP<");
      } else {
        widget->addItem(QString{"%1"}.arg(buckets[i].GetValue()));
      }
    }
  }
};

class Task3QED : public QDialog {
  Q_OBJECT

 public:
  explicit Task3QED(QWidget *parent = nullptr);
  ~Task3QED() override;

 public slots:
  void UpdateSet();
  void on_pushButton_2_clicked();
  void on_addButton_clicked();
  void on_do_2_clicked();

 private:
  Ui::Task3QED *ui_;
  lab::DirtyWrapper<HashSet> map_;
};

#endif  // TASK3QED_H
