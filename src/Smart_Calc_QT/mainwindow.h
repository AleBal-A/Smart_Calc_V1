#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "depositwindow.h"
#include "finance.h"

#ifdef __cplusplus

extern "C" {
#endif

#include "../smartcalc.h"

#ifdef __cplusplus
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:

  void on_pushButton_x_value_clicked();

 private:
  Ui::MainWindow *ui;
  double x;
  double x_is_pressed;
  double was_error_printed;
  double equal_is_pressed;
  Finance *financewindow;
  DepositWindow *deposit_window;

 private slots:
  void digits_numbers();
  void on_pushButton_clear_clicked();
  void on_pushButton_x_clicked();
  void get_result();
  void on_pushButton_draw_graph_clicked();
  void on_pushButton_open_finance_clicked();
  void on_pushButton_open_debit_clicked();
};
#endif  // MAINWINDOW_H
