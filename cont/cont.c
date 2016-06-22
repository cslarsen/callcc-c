#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "aux.h"

// Root of delimited continuation
static void *root = NULL;

// Call stack segment to copy
static void *current = NULL;
static void *continuation = NULL;
static size_t length = 0;
static void *rip = NULL;

static void *saved_rbp, *saved_rsp, *saved_rdi;
static jmp_buf env;

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
    value = setjmp(env);

    // the last thing we do is to get the instruction pointer
    //rip = getrip();
    rip = &&next;
    // TODO: get address of label
//    printf("instruction pointer rip %p\n", rip);
//    TODO: Above line (printf) crashes on OSX, but not Linux.
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

void reinstate(int value)
{
  printf("reinstating call stack:\n");
  printf("  memcpy(%p, %p, %zu)\n", root - length, continuation, length);
  memcpy(root - length, continuation, length);
  // actually, there is more to reinstating; we need to restore rbp, rsp, all
  // the argument registers, and so on
  printf("  jumping to %p\n", rip);
  longjmp(env, value);
//  jmp(saved_rbp, saved_rsp, saved_rdi, rip);
  printf("*** THIS SHOULD NEVER EXECUTE! ***\n");
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
  reinstate(--n);
  printf("done\n");
  return 0;
}

struct saved {
  void* rbp;
  void* rsp;
  void* rax;
  void* rbx;
  void* r12;
  void* r13;
  void* r14;
  void* r15;
  void* rsp_8; // not really needed
  void* fpucw;
  void* mccsr;
};

/*
int main()
{
  struct saved s;
  save(&s);
  printf(" rbp=%p\n", s.rbp);
  printf(" rsp=%p\n", s.rsp);
  printf("rsp8=%p\n", s.rsp_8);
}
*/
