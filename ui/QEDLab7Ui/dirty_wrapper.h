#ifndef LAB_DIRTY_WRAPPER_H_
#define LAB_DIRTY_WRAPPER_H_

#include <qobject.h>
#include <qtmetamacros.h>
namespace lab {

class QObjectDirtyWrapper : public QObject {
  Q_OBJECT;

 signals:
  void WasDirty();
};

template <typename T>
class DirtyWrapper {

 public:
  struct ConstWrapper {
    ConstWrapper(const DirtyWrapper<T> *ptr) : ptr_(ptr) {}
    ~ConstWrapper() = default;

    const T &Get() const { return ptr_->field_; }

   private:
    const DirtyWrapper<T> *ptr_;
  };
  struct MutWrapper {
    MutWrapper(DirtyWrapper<T> *ptr) : ptr_(ptr) {}
    ~MutWrapper() {
      if (accessed_) {
        ptr_->SetDirty();
      }
    }

    T &Get() {
      accessed_ = true;
      return ptr_->field_;
    }

   private:
    bool accessed_{};
    DirtyWrapper<T> *ptr_;
  };

  ConstWrapper AsConst() const { return {this}; }
  MutWrapper AsMut() { return {this}; }

  [[nodiscard]] bool IsDirty() const { return is_dirty_; }
  void Clear() { is_dirty_ = false; }

  [[nodiscard]] const QObjectDirtyWrapper *GetInvoker() const { return &obj_; }

 private:
  friend class ConstWrapper;
  friend class MutWrapper;

  void WasDirty() { obj_.WasDirty(); }
  void SetDirty() {
    if (is_dirty_ == false) {
      is_dirty_ = true;
      WasDirty();
    }
  }

  bool is_dirty_;
  T field_;

  QObjectDirtyWrapper obj_;
};
}  // namespace lab

#endif  // LAB_DIRTY_WRAPPER_H_