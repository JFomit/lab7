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

 public slots:
  void on_actionExit_triggered();
  void on_actionTask_1_triggered();
  void on_actionTask_2_triggered();

 private:
  Ui::QED *ui_;
};
#endif  // QED_H
