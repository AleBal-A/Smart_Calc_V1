#include "finance.h"

#include "../smartcalc.h"
#include "ui_finance.h"

Finance::Finance(QWidget *parent) : QDialog(parent), ui(new Ui::Finance) {
  ui->setupUi(this);
}

Finance::~Finance() { delete ui; }

void Finance::on_pushButton_calculate_clicked() {
  double credit_ammount = ui->doubleSpinBox_credit_ammount->value();
  int months = ui->spinBox_months->value();
  double rate = ui->doubleSpinBox_rate->value();
  int credit_type = 0;

  if (ui->radioButton_annuitet->isChecked()) {
    credit_type = 1;
  } else if (ui->radioButton_differentet->isChecked()) {
    credit_type = 2;
  }

  if (credit_type == 1) {
    annuitet_credit_calc(credit_ammount, months, rate, &monthly_payment,
                         &overpayment_amount, &total_payout);
    // результаты расчета
    ui->label_overpayment->setText(
        QLocale::system().toString(overpayment_amount, 'f', 2));
    ui->label_totalcost->setText(
        QLocale::system().toString(total_payout, 'f', 2));
    ui->label_monthly_payment->setText(
        QLocale::system().toString(monthly_payment, 'f', 2));
  } else if (credit_type == 2) {
    different_credit_calc(credit_ammount, months, rate, &min_monthly_payment,
                          &max_monthly_payment, &overpayment_amount,
                          &total_payout);
    // результаты расчета
    ui->label_overpayment->setText(
        QLocale::system().toString(overpayment_amount, 'f', 2));
    ui->label_totalcost->setText(
        QLocale::system().toString(total_payout, 'f', 2));
    ui->label_monthly_payment->setText(
        QLocale::system().toString(min_monthly_payment, 'f', 2) + " ... " +
        QLocale::system().toString(max_monthly_payment, 'f', 2));
  }
}
