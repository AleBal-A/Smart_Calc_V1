#include "smartcalc.h"

void annuitet_credit_calc(double credit_amount, int month, double procents,
                          double *monthly_payment, double *overpayment_amount,
                          double *total_payout) {
  double monthly_interest_rate = procents / 12 / 100;
  // ежемесячный платеж ануительный
  *monthly_payment =
      credit_amount *
      ((monthly_interest_rate * pow((1 + monthly_interest_rate), month)) /
       ((pow((1 + monthly_interest_rate), month)) - 1));
  // переплата ануительный
  *overpayment_amount = month * (*monthly_payment) - credit_amount;
  // общая выплата ануительный
  *total_payout = *overpayment_amount + credit_amount;
}

void different_credit_calc(double credit_amount, int months,
                           double interest_rate, double *min_monthly_payment,
                           double *max_monthly_payment,
                           double *overpayment_amount, double *total_payout) {
  double for_total_pay = credit_amount;
  // Месячная процентная ставка  (доля от годовой ставки)
  double basic_duty = interest_rate / 12 / 100;
  // Переплата
  *overpayment_amount = 0.0;

  double fixed_payment = credit_amount / months;  // фиксированный платеж
  double max_payment = 0.0;  // Максимальный платеж

  double monthly_procents = 0;
  // Подсчет в каждом месяце
  for (int i = 0; i < months; i++) {
    // Проценты на остаток кредита
    monthly_procents = credit_amount * basic_duty;
    // Общая переплата увеличивается на текущие проценты
    *overpayment_amount += monthly_procents;
    // Уменьшаю оставшуюся сумму кредита на размер ежемесячного платежа (Basic
    // duty)
    credit_amount -= fixed_payment;
    // Пересчитываю максимальный платеж
    max_payment = fmax(max_payment, monthly_procents + fixed_payment);
  }

  *min_monthly_payment = fixed_payment + monthly_procents;
  *max_monthly_payment = max_payment;
  // Общая выплата
  *total_payout = for_total_pay + *overpayment_amount;
}

/// \brief Вычисление параметров депозита на основе предоставленной информации
/// \param dep указатель на структуру deposit_info, содержащую информацию о
/// депозите \param months указатель на массив структур Month, представляющих
/// месяцы \return код выполнения: 1 - успешно, 0 - ошибка, 3 - невозможные
/// операции с нулём
int s21_deposit_calc_2_0(struct deposit_info *dep, Month *months) {
  int status = 1;  // 1 - всё ОК, 0 - ошибка;

  // Рассчитываю сумму налога на доход
  double tax_coeficent = 0;
  if (dep->tax_rate != 0.00) {
    tax_coeficent = dep->tax_rate / 100;
  }

  double monthly_rate =
      dep->percent / 12 / 100;  // Процент начисляемый раз в месяц
  double korobochka =
      0;  // Тут будут храниться начисленные проценты и ждать своей выплаты
  double deposit = dep->input_summ;

  for (int i = 0; i < dep->temp_of_placement && status; ++i) {
    deposit += months[i].plus_value;
    deposit -= months[i].minus_value;

    // Капитализация или нет
    if (dep->capitalization_of_interest == 0) {
      korobochka = dep->input_summ * monthly_rate;
      dep->input_summ -= months[i].minus_value;
      dep->input_summ += months[i].plus_value;
    } else {
      korobochka = deposit * monthly_rate;
    }
    dep->tax_amount +=
        korobochka * tax_coeficent;  // Подсчет общей суммы налогов

    // Выплаты в зависимости от условий
    if (dep->rfequency_of_payments > 1 && i > 0 &&
        dep->capitalization_of_interest) {  // Для сложного процента
      if ((i % dep->rfequency_of_payments) == 0) {
        deposit = deposit + korobochka -
                  (korobochka * tax_coeficent);  // с вычетом налогов
        dep->accrued_interest +=
            korobochka;  // начисленные проценты без учета налогов
        korobochka = 0;
      }
    } else {
      deposit = deposit + korobochka -
                (korobochka * tax_coeficent);  // с вычетом налогов
      dep->accrued_interest +=
          korobochka;  // начисленные проценты без учета налогов
      korobochka = 0;
    }

    if (deposit < 0 || dep->input_summ < 0) status = 0;  // Депозит ушел в минус
  }

  // сумма на вкладе к концу срока:
  dep->result_summ = deposit + korobochka;

  return status;
}