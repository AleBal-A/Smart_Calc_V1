#include "smartcalc.h"

Lexeme* pop(Lexeme** head) {
  Lexeme* out;
  if (head == NULL) {
    return NULL;
  } else {
    out = *head;
    *head = out->next;
    out->next = NULL;
    return out;
  }
}

void poosh_to_stack(Lexeme** stack, Lexeme* current_lexeme) {
  if (*stack == NULL) {
    *stack = current_lexeme;
  } else {
    current_lexeme->next = *stack;
    *stack = current_lexeme;
  }
}

// Конвертация в ОПН
void s21_convert_to_polish(Combine* C) {
  Lexeme* ready_stack = NULL;
  Lexeme* support_stack = NULL;
  Lexeme* pop_lexeme = NULL;

  while (C->lexemeList != NULL) {
    // Если приоритет == 0
    if (C->lexemeList->priority == 0) {
      pop_lexeme = pop(&C->lexemeList);
      poosh_to_stack(&ready_stack, pop_lexeme);
      // ЗАКРЫВАЮЩАЯ СКОКА
    } else if (C->lexemeList->type == OPERATOR_CLOSE_PAREN) {
      while (support_stack != NULL ||
             support_stack->type !=
                 OPERATOR_OPEN_PAREN) {  // Перекладываю лексемы до '('
        Lexeme* pop_from_sup_stack = pop(&support_stack);
        if (pop_from_sup_stack->type == OPERATOR_OPEN_PAREN) {
          // Перетираю лексему с '(' на next
          free(pop_from_sup_stack);
          break;
        } else {  //
          poosh_to_stack(&ready_stack, pop_from_sup_stack);
        }
      }
      // Перетираю лексему с ')' на next
      Lexeme* close_paren = pop(&C->lexemeList);
      free(close_paren);
      // Если приоритет больше 0
    } else if (C->lexemeList->priority > 0) {
      // Первое вложение в support
      if (support_stack == NULL) {
        pop_lexeme = pop(&C->lexemeList);
        poosh_to_stack(&support_stack, pop_lexeme);
        // Следующие значения в support
      } else {
        pop_lexeme = pop(&C->lexemeList);
        // Если у текущего приоритет больше, чем в саппорте
        if (pop_lexeme->priority > support_stack->priority ||
            pop_lexeme->type == OPERATOR_OPEN_PAREN) {
          poosh_to_stack(&support_stack, pop_lexeme);
        } else if (support_stack->type == OPERATOR_OPEN_PAREN) {
          poosh_to_stack(&support_stack, pop_lexeme);
        } else {
          while (support_stack != NULL &&
                 pop_lexeme->priority <= support_stack->priority &&
                 support_stack->type !=
                     OPERATOR_OPEN_PAREN) {  // Перекладываю более важные
                                             // приоритеты в ready
            Lexeme* pop_from_sup_stack = pop(&support_stack);  //
            poosh_to_stack(&ready_stack, pop_from_sup_stack);  //
          }
          poosh_to_stack(&support_stack, pop_lexeme);
        }
      }
    }
  }

  // Дочищаю саппорт_стек
  while (support_stack != NULL) {
    pop_lexeme = pop(&support_stack);
    poosh_to_stack(&ready_stack, pop_lexeme);
  }

  reverse(&ready_stack);

  C->lexemeList = ready_stack;
}