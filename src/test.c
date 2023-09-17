#include <check.h>

#include "smartcalc.h"

START_TEST(test_0) {
  double res = 0;

  int code = validation(
      "ATAN(1)+TAN(1)-SIN(1)*ASIN(1)+6+3+LOG(1)+ACOS(1)*COS(8)+LN(7)+SQRT(12)+"
      "6.22^4-4+5+X");
  ck_assert_int_eq(code, 1);
  smart_calc(
      "ATAN(1)+TAN(1)-SIN(1)*ASIN(1)+6+3+LOG(1)+ACOS(1)*COS(8)+LN(7)+SQRT(12)+"
      "6.22^4-4+5+X",
      &res, 2);
  ck_assert_float_eq_tol(res, 1515.223333, 1e-7);
}
END_TEST

START_TEST(test_1) {
  double monthly_payment = 0;
  double overpayment_amount = 0;
  double total_payout = 0;

  annuitet_credit_calc(1000000, 12, 10, &monthly_payment, &overpayment_amount,
                       &total_payout);

  ck_assert_float_eq(monthly_payment, 87915.89);
  ck_assert_float_eq(overpayment_amount, 54990.65);
  ck_assert_float_eq(total_payout, 1054990.65);

  monthly_payment = 0;
  overpayment_amount = 0;
  total_payout = 0;

  double min_monthly_payment = 0;
  double max_monthly_payment = 0;
  different_credit_calc(1000000, 12, 10, &min_monthly_payment,
                        &max_monthly_payment, &overpayment_amount,
                        &total_payout);

  ck_assert_double_eq_tol(overpayment_amount, 54166.7, 10);
  ck_assert_float_eq_tol(total_payout, 1.05417e+06, 10);
}
END_TEST

START_TEST(test_2) {
  deposit_info *dep = calloc(1, sizeof(deposit_info));
  Month *months = calloc(12, sizeof(Month));

  months[1].plus_value = 1000;

  dep->input_summ = 1000000;
  dep->temp_of_placement = 12;
  dep->percent = 10;
  dep->tax_rate = 13;
  dep->rfequency_of_payments = 6;

  dep->capitalization_of_interest = 0;

  s21_deposit_calc_2_0(dep, months);

  ck_assert_int_eq(dep->accrued_interest, 100083);
  ck_assert_int_eq(dep->result_summ, 1088072);
  ck_assert_int_eq(dep->tax_amount, 13010);

  dep->capitalization_of_interest = 1;

  dep->accrued_interest = 0;
  dep->result_summ = 0;
  dep->tax_amount = 0;

  s21_deposit_calc_2_0(dep, months);

  ck_assert_float_eq_tol(dep->accrued_interest, 16752.1, 10);
  ck_assert_int_eq(dep->result_summ, 1025045);
  ck_assert_int_eq(dep->tax_amount, 13151);

  free(dep);
  free(months);
}
END_TEST

START_TEST(test_3) {
  double res = 0;

  int code = validation("(3MOD)+2/0");
  ck_assert_int_eq(code, 1);
  smart_calc("2/0", &res, 2);
  ck_assert_double_nan(res);
}
END_TEST

START_TEST(test_4) {
  double res = 0;

  int code = validation("((XX))");
  ck_assert_int_eq(code, 0);
  smart_calc("((XX))", &res, 0);
  ck_assert_float_eq(res, 0);
}
END_TEST

START_TEST(test_5) {
  double res = 0;

  int code = validation("4MOD3*2");
  ck_assert_int_eq(code, 1);
  smart_calc("4MOD3*2", &res, 0);
  ck_assert_float_eq_tol(res, 2, 1e-7);
}
END_TEST

Suite *smartcalc(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("\033s21_smartcalc\033");
  tc = tcase_create("case_all_test");

  tcase_add_test(tc, test_0);
  tcase_add_test(tc, test_1);
  tcase_add_test(tc, test_2);
  tcase_add_test(tc, test_3);
  tcase_add_test(tc, test_4);
  tcase_add_test(tc, test_5);

  suite_add_tcase(s, tc);
  return s;
}

int main() {
  int fail = 0;
  SRunner *sr;

  sr = srunner_create(smartcalc());

  srunner_run_all(sr, CK_NORMAL);
  fail = srunner_ntests_failed(sr);
  srunner_free(sr);
  // case_test(smartcalc, &fail);
  // srunner_free(sr);
  return fail;
}
