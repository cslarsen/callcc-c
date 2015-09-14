#include <assert.h>
#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

#define GET_RBP(x) asm("movq %%rbp, %0\n" : "=r" (x));
#define GET_RSP(x) asm("movq %%rsp, %0\n" : "=r" (x));

// Does not work inside functions (or outside, for that matter):
// register unsigned long rsp asm ("rsp");

void __cdecl a(int n)
{
  unsigned long rsp, rbp;
  GET_RSP(rsp);
  GET_RBP(rbp);

  printf("a(%d) rsp=%lx rbp=%lx\n", n, rsp, rbp);
}

void __cdecl b(int n)
{
  unsigned long rsp, rbp;
  GET_RSP(rsp);
  GET_RBP(rbp);

  printf("b(%d) rsp=%lx rbp=%lx\n", n, rsp, rbp);

  a(n+1);
}

int main()
{
  assert(sizeof(unsigned long) == 8);
  b(0);
}

#pragma GCC diagnostic pop
