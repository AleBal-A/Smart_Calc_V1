#ifndef DEPOSITWINDOW_H
#define DEPOSITWINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTableWidget>

#ifdef __cplusplus

extern "C" {
#endif

#include "../smartcalc.h"

#ifdef __cplusplus
}
#endif

namespace Ui {
class DepositWindow;
}

class DepositWindow : public QDialog {
  Q_OBJECT

 public:
  explicit DepositWindow(QWidget *parent = nullptr);
  ~DepositWindow();

 private slots:
  void on_pushButton_calc_clicked();

  void on_pushButton_add_clicked();

  void on_pushButton_clear_clicked();

 private:
  Ui::DepositWindow *ui;
};

#endif  // DEPOSITWINDOW_H
