#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "aux.h"

struct cont {
  size_t length;
  jmp_buf env;
  void* frames;
};

int capture(void* current, void* start, struct cont** v) {
  *v = malloc(sizeof(struct cont));
  (*v)->length = start - current;
  (*v)->frames = malloc((*v)->length);
  memcpy((*v)->frames, current, (*v)->length);
//  return setjmp((*v)->env);
}

void reinstate(void* start, struct cont* v, int value) {
  memcpy(start - v->length, v->frames, v->length);
  longjmp(v->env, value);
}

static void *root = NULL;
static struct cont* CONT = NULL;

void foo(int value)
{
  if (CONT == NULL) {
    // RBP param; can be gotten inside capture by getting the PREVIOUS one
    capture(getrbp(), root, &CONT);
    setjmp(CONT->env);
  } else {
    value = setjmp(CONT->env);
  }

  printf("foo(value=%d)\n", value);

  if (value<0 || value>100) {
    // safety mechanism
    printf("value not in [0,100], exiting\n");
    exit(0);
  }
}

static int n=100;

int main()
{
  printf("main()\n");

  root = getrbp();
  printf("root (in main)   %p\n", root);

  // Call function once ...
  foo(n);
  // ... then reinstate the stack, looping
  reinstate(root, CONT, --n);
  printf("done\n");
  return 0;
}
