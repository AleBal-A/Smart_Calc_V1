#include "smartcalc.h"

bool is_operator(char ch) {
  return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' ||
         ch == 'M';
}

bool is_valid_digit(char ch) { return isdigit(ch) || ch == '.'; }

bool x_is_valid(const char *input_str, int *i) {
  if (*i > 0 && !(is_operator(input_str[*i - 1]) || input_str[*i - 1] == '(')) {
    // printf("X: Ошибка перед X.\n");
    return false;
  }
  if (input_str[*i + 1] != '\0' &&
      !(is_operator(input_str[*i + 1]) || input_str[*i + 1] == ')')) {
    // printf("X: Ошибка после X.\n");
    return false;
  }
  // printf("Хроший X.\n");
  return true;
}

bool number_is_valid(const char *input_str, int *i) {
  bool has_decimal_point = false;
  while (isdigit(input_str[*i]) || input_str[*i] == '.') {
    if (input_str[*i] == '.') {
      if (has_decimal_point) {
        return false;
      }
      has_decimal_point = true;
    }
    (*i)++;
  }
  if (isalpha(input_str[*i]) && !(input_str[*i] == 'M')) {
    return false;
  } else if (is_operator(input_str[*i]) || input_str[*i] == ')' ||
             input_str[*i] == '\0') {
    (*i)--;
    return true;
  } else {
    // printf("Число заканчивается точкой или другим невалидным знаком\n");
    return false;
  }
}

bool close_paren_is_valid(const char *input_str, int *i) {
  if (*i == 0) {
    // printf("')' В начале строки.\n");
    return false;
  }
  if (input_str[*i + 1] != '\0' && !is_operator(input_str[*i + 1]) &&
      input_str[*i + 1] != ')') {
    // printf("После ) неправильный символ.\n");
    return false;
  }
  return true;
}

bool open_paren_is_valid(const char *input_str, int *i) {
  if (input_str[*i + 1] == '\0' || input_str[*i + 1] == ')' ||
      input_str[*i + 1] == 'M') {
    // printf("После '(' скобочки идёт '\\0' или ')' или 'M'.\n");
    return false;
  }
  return true;
}

bool log_is_valid(const char *input_str, int *i) {
  if (strncmp(input_str + *i, "LOG", 3) == 0) {
    *i += 2;
    if (input_str[*i + 1] == '(') {
      // After log there is an opening parenthesis, which is valid
      return true;
    } else {
      // printf("LOG != LOG(\n");
      return false;
    }
  } else {
    // printf("L != LOG.\n");
    return false;
  }
}

bool ln_is_valid(const char *input_str, int *i) {
  if (strncmp(input_str + *i, "LN", 2) == 0) {
    *i += 1;
    if (input_str[*i + 1] == '(') {
      return true;
    } else {
      // printf("LN != LN(\n");
      return false;
    }
  } else {
    // printf("L != LN.\n");
    return false;
  }
}

bool a_cst_is_valid(const char *input_str, int *i) {
  if (strncmp(input_str + *i, "ACOS", 4) == 0 ||
      strncmp(input_str + *i, "ASIN", 4) == 0 ||
      strncmp(input_str + *i, "ATAN", 4) == 0) {
    *i += 3;
    if (input_str[*i + 1] == '(') {
      return true;
    } else {
      // printf("ACOS/ASIN != ACOS(/ASIN(\n");
      return false;
    }
  } else {
    // printf("A != ACOS.\n");
    return false;
  }
}

bool tan_is_valid(const char *input_str, int *i) {
  if (strncmp(input_str + *i, "TAN", 3) == 0) {
    *i += 2;
    if (input_str[*i + 1] == '(') {
      return true;
    } else {
      // printf("TAN != TAN(\n");
      return false;
    }
  } else {
    // printf("T != TAN.\n");
    return false;
  }
}

bool cos_is_valid(const char *input_str, int *i) {
  if (strncmp(input_str + *i, "COS", 3) == 0) {
    *i += 2;
    if (input_str[*i + 1] == '(') {
      return true;
    } else {
      // printf("COS != COS(\n");
      return false;
    }
  } else {
    // printf("C != COS.\n");
    return false;
  }
}

bool sin_sqrt_is_valid(const char *input_str, int *i) {
  if (input_str[*i + 1] == 'I') {
    if (strncmp(input_str + *i, "SIN", 3) == 0) {
      (*i) += 2;

      if (input_str[*i + 1] == '(') {
        return true;
      } else {
        // printf("SIN != SIN(\n");
        return false;
      }
    } else {
      // printf("S != SIN.\n");
      return false;
    }
  } else if (input_str[*i + 1] == 'Q') {
    if (strncmp(input_str + *i, "SQRT", 4) == 0) {
      (*i) += 3;

      if (input_str[*i + 1] == '(') {
        return true;
      } else {
        // printf("SQRT != SQRT(\n");
        return false;
      }
    } else {
      // printf("S != SQRT.\n");
      return false;
    }
  } else {
    // printf("Sin Sqrt фигня какая-то\n");
    return false;
  }
}

bool mod_is_valid(const char *input_str, int *i) {
  if (input_str[*i + 1] != 'O' || input_str[*i + 2] != 'D') {
    // printf("M != MOD\n");
    return false;
  } else if (isdigit(input_str[*i - 1]) || input_str[*i - 1] == ')' ||
             isdigit(input_str[*i + 3]) || input_str[*i + 3] == '(') {
    // printf("MOD Good\n");
    (*i) += 2;
    return true;
  } else {
    // printf("MOD окружен невалидом\n");
    return false;
  }
}

bool multiply_is_valid(const char *input_str, int *i) {
  if (*i == 0 || is_operator(input_str[*i - 1]) || isalpha(input_str[*i - 1])) {
    // printf("*Умножение в начале/после оператора/после буквы.\n");
    return false;
  } else if (isdigit(input_str[*i + 1])) {
    return true;
  } else if (isalpha(input_str[*i + 1])) {
    return true;
  } else if (input_str[*i + 1] == '(') {
    return true;
  } else {
    // printf(" multiply invalid.\n");
    return false;
  }
}

bool plus_is_valid(const char *input_str, int *i) {
  if (*i > 0 && is_operator(input_str[*i - 1])) {
    // printf(" +: Перед плюсом есть оператор.\n");
    return false;
  }

  if (input_str[*i + 1] == '\0') {
    // printf(" +: After plus there must be a digit or a function\n");
    return false;
  }
  if (*i > 0) {
    char prev_char = input_str[*i - 1];
    if (prev_char != '(' && prev_char != ')' && !is_valid_digit(prev_char) &&
        !isalpha(prev_char)) {
      // printf(" +: Унарный знак.\n");
      return false;
    }
  }
  if (!is_valid_digit(input_str[*i + 1]) && !isalpha(input_str[*i + 1]) &&
      input_str[*i + 1] != '(') {
    // printf(" +:Правило: После плюса должно быть число или начало функции, или
    // открытие скобки\n");
    return false;
  }
  // printf("+Valid znak.\n");
  return true;
}

// Главная функция
int operator_place_is_valid(const char *input_str, int *i, int *open_bracket) {
  LexemeType lexemeType = 0;
  char ch = input_str[*i];
  if (isdigit(ch)) {
    lexemeType = NUMBER;
  } else if (ch == 'X') {
    lexemeType = VARIABLE_X;
  } else if (ch == '(') {
    lexemeType = OPERATOR_OPEN_PAREN;
  } else if (ch == ')') {
    lexemeType = OPERATOR_CLOSE_PAREN;
  } else if (ch == '+') {
    lexemeType = OPERATOR_PLUS;
  } else if (ch == '-') {
    lexemeType = OPERATOR_MINUS;
  } else if (ch == '*') {
    lexemeType = OPERATOR_MULTIPLY;
  } else if (ch == '/') {
    lexemeType = OPERATOR_DIVIDE;
  } else if (ch == '^') {
    lexemeType = OPERATOR_POWER;
  } else if (ch == 'M') {
    lexemeType = OPERATOR_MOD;
  } else if (ch == '(') {
    lexemeType = OPERATOR_OPEN_PAREN;
  } else if (ch == ')') {
    lexemeType = OPERATOR_CLOSE_PAREN;
  } else if (ch == 'S') {
    if (input_str[*i + 1] == 'I' && input_str[*i + 2] == 'N') {
      lexemeType = OPERATOR_SIN;
    } else if (input_str[*i + 1] == 'Q' && input_str[*i + 2] == 'R' &&
               input_str[*i + 3] == 'T') {
      lexemeType = OPERATOR_SQRT;
    } else {
      // printf("S - дальше каша непонятная\n");
    }
  } else if (ch == 'C') {
    if (input_str[*i + 1] == 'O' && input_str[*i + 2] == 'S') {
      lexemeType = OPERATOR_COS;
    } else {
      // printf("C - дальше каша непонятная\n");
    }
  } else if (ch == 'T') {
    if (input_str[*i + 1] == 'A' && input_str[*i + 2] == 'N') {
      lexemeType = OPERATOR_TAN;
    } else {
      // printf("T - дальше каша непонятная\n");
    }
  } else if (ch == 'A') {
    if (input_str[*i + 1] == 'C' && input_str[*i + 2] == 'O') {
      lexemeType = OPERATOR_ACOS;
    } else if (input_str[*i + 1] == 'S' && input_str[*i + 2] == 'I') {
      lexemeType = OPERATOR_ASIN;
    } else if (input_str[*i + 1] == 'T' && input_str[*i + 2] == 'A') {
      lexemeType = OPERATOR_ATAN;
    } else {
      // printf("A - дальше каша непонятная\n");
    }
  } else if (ch == 'L') {
    if (input_str[*i + 1] == 'O' && input_str[*i + 2] == 'G') {
      lexemeType = OPERATOR_LOG;
    } else if (input_str[*i + 1] == 'N') {
      lexemeType = OPERATOR_LN;
    } else {
      // printf("L - дальше каша непонятная\n");
    }
  } else {
    // printf(" END Unknown operator\n");
  }

  int is_valid_flag = true;
  switch (lexemeType) {
    case NUMBER:
      is_valid_flag = number_is_valid(input_str, i);
      break;
    case VARIABLE_X:
      is_valid_flag = x_is_valid(input_str, i);
      break;
    case OPERATOR_OPEN_PAREN:
      (*open_bracket)++;
      is_valid_flag = open_paren_is_valid(input_str, i);
      break;
    case OPERATOR_CLOSE_PAREN:
      if (*open_bracket != 0) {  // РАЗОБРАТЬСЯ почему работает с *
        is_valid_flag = close_paren_is_valid(input_str, i);
      } else {
        is_valid_flag = false;
      }
      (*open_bracket)--;
      break;
    case OPERATOR_PLUS:
      // printf("Operator: +\n");
      is_valid_flag = plus_is_valid(input_str, i);
      break;
    case OPERATOR_MINUS:
      // printf("Operator: -\n");
      is_valid_flag = plus_is_valid(input_str, i);
      break;
    case OPERATOR_MULTIPLY:
      // printf("Operator: *\n");
      is_valid_flag = multiply_is_valid(input_str, i);
      break;
    case OPERATOR_DIVIDE:
      // printf("Operator: /\n");
      is_valid_flag = multiply_is_valid(input_str, i);
      break;
    case OPERATOR_POWER:
      // printf("Operator: ^\n");
      is_valid_flag = multiply_is_valid(input_str, i);
      break;
    case OPERATOR_MOD:
      // printf("Operator: M\n");
      is_valid_flag = mod_is_valid(input_str, i);
      break;
    case OPERATOR_COS:
      // printf("Operator: COS\n");
      is_valid_flag = cos_is_valid(input_str, i);
      break;
    case OPERATOR_SIN:
      // printf("Operator: SIN\n");
      is_valid_flag = sin_sqrt_is_valid(input_str, i);
      break;
    case OPERATOR_TAN:
      // printf("Operator: TAN\n");
      is_valid_flag = tan_is_valid(input_str, i);
      break;
    case OPERATOR_ACOS:
      // printf("Operator: ACOS\n");
      is_valid_flag = a_cst_is_valid(input_str, i);
      break;
    case OPERATOR_ASIN:
      // printf("Operator: ASIN\n");
      is_valid_flag = a_cst_is_valid(input_str, i);
      break;
    case OPERATOR_ATAN:
      // printf("Operator: ATAN\n");
      is_valid_flag = a_cst_is_valid(input_str, i);
      break;
    case OPERATOR_SQRT:
      // printf("Operator: SQRT\n");
      is_valid_flag = sin_sqrt_is_valid(input_str, i);
      break;
    case OPERATOR_LN:
      // printf("Operator: LN\n");
      is_valid_flag = ln_is_valid(input_str, i);
      break;
    case OPERATOR_LOG:
      // printf("Operator: LOG\n");
      is_valid_flag = log_is_valid(input_str, i);
      break;
    default:
      // printf("SWITCHCASE не нашел кейс - (\"is_valid_flag\" = false)\n");
      is_valid_flag = false;
      break;
  }

  return is_valid_flag;
}

int validation(const char *input) {
  // const char *input = "ATAN(+1^(X+3))/(+0.2)+LOG(-X)";

  int answer = true;
  int lenght = (int)strlen(input);
  int open_bracket = 0;
  for (int i = 0; i < lenght && answer; i++) {
    answer = operator_place_is_valid(input, &i, &open_bracket);
  }
  if (open_bracket > 0) answer = false;

  // printf("\n\n\n=============\n\n Validation = %d\n", answer);

  return answer;
}