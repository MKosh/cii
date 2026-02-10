#pragma once

#include <setjmp.h>

#define T Except_t
typedef struct T {
  const char* reason;
} T;

// Exported Types
typedef struct ExceptFrame ExceptFrame;
struct ExceptFrame {
  ExceptFrame* prev;
  jmp_buf env;
  const char* file;
  int line;
  const Except_t* exception;
};

enum {
  EXCEPT_ENTERED = 0,
  EXCEPT_RAISED,
  EXCEPT_HANDLED,
  EXCEPT_FINALIZED
};

// Exported variables
extern ExceptFrame* except_stack;
extern const Except_t assert_failed;

// Exported functions
void except_raise(const T* e, const char* file, int line);

// Exported macros
#define RAISE(e) except_raise(&(e), __FILE__, __LINE__)

#define RERAISE except_raise(except_frame.exception, \
    except_frame.file, except_frame.line)

#define RETURN switch (except_stack = &except_frame, 0) default: return

#define TRY do { \
  volatile int except_flag; \
  ExceptFrame except_frame; \
  except_frame.prev = except_stack; \
  except_stack = &except_frame; \
  except_flag = setjmp(except_frame.env); \
  if (except_flag == EXCEPT_ENTERED) {

#define EXCEPT(e) \
    if (except_flag == EXCEPT_ENTERED) except_stack = except_stack->prev; \
  } else if (except_frame.exception == &(e)) { \
    except_flag = EXCEPT_HANDLED;

#define ELSE \
    if (except_flag == EXCEPT_ENTERED) except_stack = except_stack->prev; \
  } else { \
    except_flag = EXCEPT_HANDLED;

#define FINALLY \
    if (except_flag == EXCEPT_ENTERED) except_stack = except_stack->prev; \
  } { \
    if (except_flag == EXCEPT_ENTERED) \
      except_flag = EXCEPT_FINALIZED;

#define END_TRY \
    if (except_flag == EXCEPT_ENTERED) except_stack = except_stack->prev; \
    } if (except_flag == EXCEPT_RAISED) RERAISE; \
} while (0)

#undef T
