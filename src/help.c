#include "smartcalc.h"

// Переносит числа из строки input в строку out
int char_scanner(const char *input, char *out, int *i) {
  int status = NO;
  int c = 0;
  while ((input[*i] >= '0' && input[*i] <= '9') || input[*i] == '.') {
    out[c++] = input[*i];
    status = YES;
    (*i) += 1;
  }
  (*i) -= 1;
  return status;
}

void reverse(Lexeme **stack) {
  Lexeme *stack_reverse = NULL;
  while (*stack != NULL) {
    Lexeme *current_lexeme = pop(stack);
    current_lexeme->next = NULL;
    poosh_to_stack(&stack_reverse, current_lexeme);
  }
  *stack = stack_reverse;
}

// Идёт разбивка входной строки на лексемы
void parsing_to_lexems(Combine *C) {
  // Проход по всей строке и запись элементов в стек из лексем
  // *C->counter = 0;
  for (int i = 0; i < C->input_length; i++) {
    char charToCompare = C->input[i];
    *C->counter = i;

    switch (charToCompare) {
      case '+':
      case '-':
      case '*':
      case '/':
        operator_to_Lexem(C);
        break;
      case 'X': {
        Lexeme *newLexeme = create_lexeme(C->variable_x, NUMBER, 0);
        push_to_stack(C, newLexeme);
      } break;
      case '^': {
        Lexeme *newLexeme = create_lexeme(0, OPERATOR_POWER, 3);
        push_to_stack(C, newLexeme);
      } break;
      case 'M': {
        i += 2;
        Lexeme *newLexeme = create_lexeme(0, OPERATOR_MOD, 2);
        push_to_stack(C, newLexeme);
      } break;
      case '(': {
        Lexeme *newLexeme = create_lexeme(0, OPERATOR_OPEN_PAREN, 1);
        push_to_stack(C, newLexeme);
      } break;
      case ')': {
        Lexeme *newLexeme = create_lexeme(0, OPERATOR_CLOSE_PAREN, 5);
        push_to_stack(C, newLexeme);
      } break;

      case 'S':
      case 'C':
      case 'T':
      case 'A':
      case 'L':
        func_to_Lexem(C);
        break;

      default:
        if (isdigit(charToCompare)) {
          number_to_Lexem(C);

        } else {
          // printf("\nПарсинг в лексемы остановился...\n");
        }
    }
    i = *C->counter;
  }
}

Lexeme *create_lexeme(double value, int operatorType, int operatorPriority) {
  Lexeme *newLexeme = (Lexeme *)calloc(1, sizeof(Lexeme));
  newLexeme->type = operatorType;
  newLexeme->value = value;  // Не используется для операторов
  newLexeme->priority = operatorPriority;
  newLexeme->next = NULL;
  newLexeme->back = NULL;

  return newLexeme;
}

void push_to_stack(Combine *C, Lexeme *newLexeme) {
  if (C->currentLexeme == NULL) {  // Если лексема создается в первый раз
    C->lexemeList = newLexeme;
    C->currentLexeme = newLexeme;
  } else {
    newLexeme->back = C->currentLexeme;
    C->currentLexeme->next =
        newLexeme;  // Привязывает "next" из прошлого на новую лексему
    C->currentLexeme =
        newLexeme;  // Теперь newLexeme ждёт своей очереди на шаг сверху
  }
}

// Моя очистка памяти в лексемах
void my_free(Lexeme *input) {
  while (input != NULL) {
    Lexeme *temp = input;
    input = input->next;
    free(temp);
  }
}

// Переводит числа в лексемы
void number_to_Lexem(Combine *C) {
  char *for_atof = (char *)calloc(C->input_length, sizeof(char));
  char_scanner(C->input, for_atof, C->counter);

  /* Код, который заполняет новую структуру Lexeme */
  Lexeme *newLexeme = (Lexeme *)calloc(1, sizeof(Lexeme));
  newLexeme->type = NUMBER;
  newLexeme->value = atof(for_atof);
  newLexeme->priority = 0;
  newLexeme->next = NULL;
  newLexeme->back = NULL;
  free(for_atof);
  // Добавляем новую лексему в список
  push_to_stack(C, newLexeme);
}

void func_to_Lexem(Combine *C) {
  int operatorPriority = 4;
  int operatorType = 0;
  char ch = C->input[*(C->counter)];

  if (ch == 'S') {
    if (C->input[*(C->counter) + 1] ==
        'I' /* && C->input[*(C->counter)+2] == 'N'*/) {
      operatorType = OPERATOR_SIN;
      (*C->counter) += 2;
    } else if (C->input[*(C->counter) + 1] == 'Q' &&
               C->input[*(C->counter) + 2] ==
                   'R' /*&& C->input[*(C->counter)+3]) == 'T'*/) {
      operatorType = OPERATOR_SQRT;
      (*C->counter) += 3;
    } else {
      // printf("S - ERROR func_to_Lexem\n");
    }
  } else if (ch == 'C') {
    if (C->input[*(C->counter) + 1] == 'O' &&
        C->input[*(C->counter) + 2] == 'S') {
      operatorType = OPERATOR_COS;
      (*C->counter) += 2;
    } else {
      // printf("C ERROR func_to_Lexem\n");
    }
  } else if (ch == 'T') {
    if (C->input[*(C->counter) + 1] == 'A' &&
        C->input[*(C->counter) + 2] == 'N') {
      operatorType = OPERATOR_TAN;
      (*C->counter) += 2;
    } else {
      // printf("T ERROR func_to_Lexem\n");
    }
  } else if (ch == 'A') {
    if (strncmp(C->input + *(C->counter), "ACOS", 4) == 0) {
      operatorType = OPERATOR_ACOS;
      (*C->counter) += 3;
    } else if (strncmp(C->input + *(C->counter), "ASIN", 4) == 0) {
      operatorType = OPERATOR_ASIN;
      (*C->counter) += 3;
    } else if (strncmp(C->input + *(C->counter), "ATAN", 4) == 0) {
      operatorType = OPERATOR_ATAN;
      (*C->counter) += 3;
    } else {
      // printf("A - ERROR func_to_Lexem\n");
    }
  } else if (ch == 'L') {
    if (C->input[*(C->counter) + 1] == 'O' &&
        C->input[*(C->counter) + 2] == 'G') {
      operatorType = OPERATOR_LOG;
      (*C->counter) += 2;
    } else if (C->input[*(C->counter) + 1] == 'N') {
      operatorType = OPERATOR_LN;
      (*C->counter) += 1;
    } else {
      // printf("L - ERROR func_to_Lexem\n");
    }
  } else {
    // printf("END Unknown operator\n");
  }

  Lexeme *newLexeme = (Lexeme *)calloc(1, sizeof(Lexeme));

  newLexeme->type = operatorType;
  newLexeme->value = 0;  // Не используется для операторов
  newLexeme->priority = operatorPriority;
  newLexeme->next = NULL;
  newLexeme->back = NULL;
  push_to_stack(C, newLexeme);
}

void operator_to_Lexem(Combine *C) {
  int operatorPriority = 0;
  int operatorType = 0;
  if (strncmp(&(C->input[*(C->counter)]), "+", 1) == 0) {
    operatorType = OPERATOR_PLUS;
    // Условие для унарного знака
    if (*(C->counter) == 0 || C->input[*(C->counter) - 1] == '(') {
      Lexeme *zero_newLexeme =
          create_lexeme(0, NUMBER, 0);  // Содается лексема с нулём
      push_to_stack(C, zero_newLexeme);
    }
    operatorPriority = 1;
  } else if (strncmp(&(C->input[*(C->counter)]), "-", 1) == 0) {
    operatorType = OPERATOR_MINUS;
    // Условие для унарного знака
    if (*(C->counter) == 0 || C->input[*(C->counter) - 1] == '(') {
      Lexeme *zero_newLexeme =
          create_lexeme(0, NUMBER, 0);  // Содается лексема с нулём
      push_to_stack(C, zero_newLexeme);
    }
    operatorPriority = 1;
  } else if (strncmp(&(C->input[*(C->counter)]), "*", 1) == 0) {
    operatorType = OPERATOR_MULTIPLY;
    operatorPriority = 2;
  } else if (strncmp(&(C->input[*(C->counter)]), "/", 1) == 0) {
    operatorType = OPERATOR_DIVIDE;
    operatorPriority = 2;
  }
  /* Код, который заполняет новую структуру Lexeme */
  Lexeme *newLexeme = (Lexeme *)calloc(1, sizeof(Lexeme));

  newLexeme->type = operatorType;
  newLexeme->value = 0;  // Не используется для операторов
  newLexeme->priority = operatorPriority;
  newLexeme->next = NULL;
  newLexeme->back = NULL;

  // Добавляем новую лексему в список
  push_to_stack(C, newLexeme);
}
