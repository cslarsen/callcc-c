#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

// Root of delimited continuation
static void *root = NULL;

// Call stack segment to copy
static void *current = NULL;
static void *continuation = NULL;
static size_t length = 0;
static void *rip = NULL;

static void *saved_rbp, *saved_rsp, *saved_rdi;

void foo(int value)
{
  if (!current) {
    current = getrbp();

    // save registers
    saved_rbp = current;
    saved_rsp = getrsp();
    saved_rdi = getrdi();

    printf("current (in foo) %p\n", current);
    assert(current < root); // the stack grows down
    length = root - current;

    continuation = malloc(length);
    printf("allocated continuation at %p\n", continuation);
    printf("copying call stack:\n");
    printf("  memcpy(%p, %p, %zu)\n", continuation, current, length);
    memcpy(continuation, current, length);

    // the last thing we do is to get the instruction pointer
    //rip = getrip();
    rip = &&next;
    // TODO: get address of label
    printf("instruction pointer rip %p\n", rip);
  }

next:
  printf("foo(value=%d)\n", value);

  if (value<0 || value>100) {
    // safety mechanism
    printf("value not in [0,100], exiting\n");
    exit(0);
  }

  printf("returning...\n");
}

void reinstate()
{
  printf("reinstating call stack:\n");
  printf("  memcpy(%p, %p, %zu)\n", root - length, continuation, length);
  memcpy(root - length, continuation, length);
  // actually, there is more to reinstating; we need to restore rbp, rsp, all
  // the argument registers, and so on
  printf("  jumping to %p\n", rip);
  jmp(saved_rbp, saved_rsp, saved_rdi, rip);
  printf("*** THIS SHOULD NEVER EXECUTE! ***\n");
}

int main()
{
  printf("main()\n");

  root = getrbp();
  printf("root (in main)   %p\n", root);

  // Call function once ...
  foo(100);
  // ... then reinstate the stack, looping
  reinstate();
  printf("done\n");
}
