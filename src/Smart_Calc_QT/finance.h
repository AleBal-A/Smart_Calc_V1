#ifndef FINANCE_H
#define FINANCE_H

#include <QDialog>

#ifdef __cplusplus

extern "C" {
#endif

#include "../smartcalc.h"

#ifdef __cplusplus
}
#endif

namespace Ui {
class Finance;
}

class Finance : public QDialog {
  Q_OBJECT

 public:
  explicit Finance(QWidget *parent = nullptr);
  ~Finance();

 private slots:
  void on_pushButton_calculate_clicked();

 private:
  Ui::Finance *ui;
  double monthly_payment;
  double overpayment_amount;
  double total_payout;
  double min_monthly_payment;
  double max_monthly_payment;
};

#endif  // FINANCE_H
