#ifndef SMARTCALC_H
#define SMARTCALC_H

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPRESSION_LENGTH 255
#define YES 1
#define NO 0

#define GOOD 0
#define BAD 1

typedef enum {
  NUMBER,
  VARIABLE_X,
  OPERATOR_PLUS,
  OPERATOR_MINUS,
  OPERATOR_MULTIPLY,
  OPERATOR_DIVIDE,
  OPERATOR_POWER,
  OPERATOR_MOD,
  OPERATOR_COS,
  OPERATOR_SIN,
  OPERATOR_TAN,
  OPERATOR_ACOS,
  OPERATOR_ASIN,
  OPERATOR_ATAN,
  OPERATOR_SQRT,
  OPERATOR_LN,  // Основание равное числу "e"
  OPERATOR_LOG,  // Основание указывается самостоятельно
  OPERATOR_OPEN_PAREN,
  OPERATOR_CLOSE_PAREN
} LexemeType;

typedef struct Lexeme {
  double value;  // число (если применимо)
  int priority;  // приоритет оператора/функции
  struct Lexeme *next;  // указатель на следующую структуру
  struct Lexeme *back;  // указатель на предыдущую структуру
  LexemeType type;  // тип лексемы
} Lexeme;

typedef struct {
  int input_length;
  const char *input;  // "строка чаров"
  int *counter;       // "i++"
  Lexeme *lexemeList;
  Lexeme *currentLexeme;
  int error;
  double variable_x;
} Combine;

/// \brief Основная функция расчета
/// \param input_origin указатель на строку с выражением
/// \param answer указатель на переменную для результата выражажения
/// \param x_variable значение для "X"
/// \return код выполнения: 1 - успешно, 0 - ошибка, 3 - невозможные операции с
/// нулём
int smart_calc(const char *input_origin, double *answer, double x_variable);

/// \brief Разбивка входной строки на лексемы
/// \param C указатель на структуру Combine
/// \return void
void parsing_to_lexems(Combine *C);

/// \brief Сканирование символов во входной строке
/// \param input указатель на входную строку
/// \param out указатель на символьный буфер для вывода
/// \param i указатель на индекс текущего символа во входной строке
/// \return 1, если сканирование успешно, 0 в противном случае
int char_scanner(const char *input, char *out, int *i);

/// \brief Преобразование числа в лексему
/// \param C указатель на структуру Combine
void number_to_Lexem(Combine *C);

/// \brief Преобразование оператора в лексему
/// \param C указатель на структуру Combine
void operator_to_Lexem(Combine *C);

/// \brief Преобразование функции в лексему
/// \param C указатель на структуру Combine
void func_to_Lexem(Combine *C);

/// \brief Создание новой лексемы
/// \param value значение лексемы (если применимо)
/// \param operatorType тип оператора
/// \param operatorPriority приоритет оператора
/// \return указатель на созданную лексему
Lexeme *create_lexeme(double value, int operatorType, int operatorPriority);

/// \brief Добавление лексемы в стек
/// \param C указатель на структуру Combine
/// \param newLexeme указатель на новую лексему
void push_to_stack(Combine *C, Lexeme *newLexeme);

/// \brief Освобождение памяти, занятой лексемами
/// \param input указатель на лексему
void my_free(Lexeme *input);

// ============================ Polish natation ============================

/// \brief Преобразование инфиксного выражения в обратную польскую запись
/// \param C указатель на структуру Combine
void s21_convert_to_polish(Combine *C);

/// \brief Извлечение лексемы из стека
/// \param head указатель на указатель на вершину стека
/// \return указатель на извлеченную лексему
Lexeme *pop(Lexeme **head);

/// \brief Помещение лексемы в стек
/// \param stack указатель на указатель на стек
/// \param current_lexeme указатель на лексему для помещения
void poosh_to_stack(Lexeme **stack, Lexeme *current_lexeme);

/// \brief Реверс стека лексем
/// \param stack указатель на указатель на стек
void reverse(Lexeme **stack);

// ============================ Calculate ============================

/// \brief Вычисление результата выражения, представленного в виде лексем
/// \param C указатель на структуру Combine
/// \return результат вычисления
double calculate_lexems(Combine *C);

/// \brief Проверка, требуются ли два операнда для оператора указанного типа
/// \param type тип оператора
/// \return 1, если требуются два операнда, 0 в противном случае
int is_need_two_number(int type);

/// \brief Выполнение математической операции между двумя числами
/// \param a первое число
/// \param b второе число
/// \param type тип оператора
/// \return результат операции
double calculator(double a, double b, int type);

// ============================ Finance Calculate ============================

typedef struct {
  double plus_value;   // сумма пополнения
  double minus_value;  // сумма снятия
} Month;

typedef struct deposit_info {
  // Вход:
  // сумма вклада,
  double input_summ;
  // срок размещения,
  int temp_of_placement;
  // процентная ставка,
  double percent;
  // налоговая ставка,
  double tax_rate;
  // периодичность выплат,
  int rfequency_of_payments;
  // капитализация процентов,
  double capitalization_of_interest;
  // список пополнений,
  double list_of_deposits;
  // список частичных снятий
  double list_of_partial_withdrawals;

  // Выход:
  // сумма на вкладе к концу срока
  double result_summ;
  // начисленные проценты,
  double accrued_interest;
  // сумма налога,
  double tax_amount;
} deposit_info;

/// \brief Вычисление аннуитетного кредита
/// \param credit_amount сумма кредита
/// \param month количество месяцев
/// \param procents процентная ставка
/// \param monthly_payment указатель на переменную для ежемесячного платежа
/// \param overpayment_amount указатель на переменную для суммы переплаты
/// \param total_payout указатель на переменную для общей выплаты
void annuitet_credit_calc(double credit_amount, int month, double procents,
                          double *monthly_payment, double *overpayment_amount,
                          double *total_payout);

/// \brief Вычисление дифференцированного кредита
/// \param credit_amount сумма кредита
/// \param months количество месяцев
/// \param interest_rate процентная ставка
/// \param min_monthly_payment указатель на переменную для минимального
/// ежемесячного платежа \param max_monthly_payment указатель на переменную для
/// максимального ежемесячного платежа \param overpayment_amount указатель на
/// переменную для суммы переплаты \param total_payout указатель на переменную
/// для общей выплаты
void different_credit_calc(double credit_amount, int months,
                           double interest_rate, double *min_monthly_payment,
                           double *max_monthly_payment,
                           double *overpayment_amount, double *total_payout);

int s21_deposit_calc_2_0(struct deposit_info *dep, Month *months);

// ==================== Validation====================

/// \brief Проверка валидности входной строки с выражением
/// \param input указатель на входную строку
/// \return 1, если строка валидна, 0 в противном случае
int validation(const char *input);

/// \brief Проверка, является ли символ оператором
/// \param ch символ для проверки
/// \return true, если символ - оператор, false в противном случае
bool is_operator(char ch);

/// \brief Проверка, является ли символ допустимой цифрой
/// \param ch символ для проверки
/// \return true, если символ - допустимая цифра, false в противном случае
bool is_valid_digit(char ch);

/// \brief Проверка валидности переменной "X" во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если "X" валиден, false в противном случае
bool x_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности числа во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если число валидно, false в противном случае
bool number_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности закрывающей скобки ')' во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если закрывающая скобка валидна, false в противном случае
bool close_paren_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности открывающей скобки '(' во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если открывающая скобка валидна, false в противном случае
bool open_paren_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности функции log() во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если функция log() валидна, false в противном случае
bool log_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности функции ln() во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если функция ln() валидна, false в противном случае
bool ln_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности константы "a" во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если константа "a" валидна, false в противном случае
bool a_cst_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности функции tan() во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если функция tan() валидна, false в противном случае
bool tan_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности функции cos() во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если функция cos() валидна, false в противном случае
bool cos_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности функций sin() и sqrt() во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если функции sin() или sqrt() валидны, false в противном
/// случае
bool sin_sqrt_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности оператора mod (%) во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если оператор mod (%) валиден, false в противном случае
bool mod_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности оператора умножения (*) во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если оператор умножения (*) валиден, false в противном случае
bool multiply_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности оператора сложения (+) во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \return true, если оператор сложения (+) валиден, false в противном случае
bool plus_is_valid(const char *input_str, int *i);

/// \brief Проверка валидности размещения операторов и скобок во входной строке
/// \param input_str указатель на входную строку
/// \param i указатель на индекс текущего символа во входной строке
/// \param open_bracket указатель на количество открытых скобок
/// \return 1, если размещение операторов и скобок валидно, 0 в противном случае
int operator_place_is_valid(const char *input_str, int *i, int *open_bracket);

#endif