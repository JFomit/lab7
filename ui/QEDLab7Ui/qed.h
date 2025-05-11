#ifndef QED_H
#define QED_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class QED;
}
QT_END_NAMESPACE

class QED : public QMainWindow {
  Q_OBJECT

 public:
  QED(QWidget *parent = nullptr);
  ~QED() override;

 private:
  Ui::QED *ui_;
};
#endif  // QED_H
