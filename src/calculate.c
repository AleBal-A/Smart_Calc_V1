#include "smartcalc.h"

int smart_calc(const char *input_origin, double *answer, double x_variable) {
  Combine *C = (Combine *)calloc(1, sizeof(Combine));
  int counter = 0;
  C->counter = &counter;
  // Валидация входной строки
  int error = validation(input_origin);
  C->error = error;
  C->variable_x = x_variable;

  if (C->error == false) {
    free(C);
    return error;
  } else {
    C->input = input_origin;
    C->input_length = strlen(C->input);

    // Идёт разбивка входной строки на лексемы
    parsing_to_lexems(C);
    // Конвертация в ОПН
    s21_convert_to_polish(C);
    *answer = calculate_lexems(C);
    error = C->error;
    if (isnan(*answer)) {
      error = 3;
    }

    free(C);
  }

  return error;
}

double calculate_lexems(Combine *C) {
  Lexeme *buffer_stack = NULL;
  Lexeme *pop_lexeme = NULL;

  Lexeme *a_value = NULL;
  Lexeme *b_value = NULL;
  Lexeme *operation = NULL;

  double a, b, answer = 0;

  // int alone = 1;  // флаг на то, что пришло выражение только из оного числа

  while (C->lexemeList != NULL) {
    if (C->lexemeList->priority == 0) {
      pop_lexeme = pop(&C->lexemeList);
      poosh_to_stack(&buffer_stack, pop_lexeme);
    } else if (C->lexemeList->priority != 0) {
      // alone = 0;

      int chose = is_need_two_number(
          C->lexemeList->type);  // Смотрю нужно ли два чисел для оператора
      if (chose) {
        b_value = pop(&buffer_stack);
        a_value = pop(&buffer_stack);
        operation = pop(&C->lexemeList);

        b = b_value->value;
        a = a_value->value;

        answer = calculator(a, b, operation->type);
        Lexeme *new_lexeme = create_lexeme(answer, NUMBER, 0);
        poosh_to_stack(&C->lexemeList, new_lexeme);

        my_free(b_value);
        my_free(a_value);
        my_free(operation);

      } else {
        a_value = pop(&buffer_stack);
        a = a_value->value;
        operation = pop(&C->lexemeList);

        answer = calculator(a, 0, operation->type);
        Lexeme *new_lexeme = create_lexeme(answer, NUMBER, 0);
        poosh_to_stack(&C->lexemeList, new_lexeme);

        free(a_value);
        free(operation);
      }
    }
  }

  // if (alone) answer = buffer_stack->value; // Выражение из одного элемениа
  // остается равным самому себе

  my_free(C->lexemeList);
  my_free(buffer_stack);

  return answer;
}

int is_need_two_number(int type) {
  return type == OPERATOR_MOD || type == OPERATOR_DIVIDE ||
         type == OPERATOR_MULTIPLY || type == OPERATOR_MINUS ||
         type == OPERATOR_PLUS || type == OPERATOR_POWER;
}

double calculator(double a, double b, int type) {
  double answer = 0;

  switch (type) {
    case OPERATOR_PLUS:
      answer = a + b;
      break;
    case OPERATOR_MINUS:
      answer = a - b;
      break;
    case OPERATOR_MULTIPLY:
      answer = a * b;
      break;
    case OPERATOR_DIVIDE:
      if (b != 0) {
        answer = a / b;
      } else {
        // Деление на ноль
        answer = NAN;
      }
      break;
    case OPERATOR_POWER:
      answer = pow(a, b);
      break;
    case OPERATOR_MOD:
      if (b != 0) {
        answer = fmod(a, b);
      } else {
        // Деление на ноль
        answer = NAN;
      }
      break;
    case OPERATOR_COS:
      answer = cos(a);
      break;
    case OPERATOR_SIN:
      answer = sin(a);
      break;
    case OPERATOR_TAN:
      answer = tan(a);
      break;
    case OPERATOR_ACOS:
      answer = acos(a);
      break;
    case OPERATOR_ASIN:
      answer = asin(a);
      break;
    case OPERATOR_ATAN:
      answer = atan(a);
      break;
    case OPERATOR_SQRT:
      if (a >= 0) {
        answer = sqrt(a);
      } else {
        // Квадрата из отрицательного нет
        answer = NAN;
      }
      break;
    case OPERATOR_LN:
      if (a > 0) {
        answer = log(a);
      } else {
        // Логорифма из отрицательного нет
        answer = NAN;
      }
      break;
    case OPERATOR_LOG:
      if (a > 0) {
        answer = log10(a);
      } else {
        // Логорифма из отрицательного нет
        answer = NAN;
      }
      break;
    default:
      printf("\n Calculator = неизвестная ошибка");
      break;
  }

  return answer;
}