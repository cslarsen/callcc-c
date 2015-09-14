#include <assert.h>
#include <stdio.h>

void __cdecl a(int n)
{
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wuninitialized"
  register const unsigned long rsp asm ("rsp");
  register const unsigned long rbp asm ("rbp");

  printf("a(%d) rsp=%lx rbp=%lx\n", n, rsp, rbp);
  #pragma GCC diagnostic pop
}

void __cdecl b(int n)
{
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wuninitialized"
  register const unsigned long rsp asm ("rsp");
  register const unsigned long rbp asm ("rbp");

  printf("b(%d) rsp=%lx rbp=%lx\n", n, rsp, rbp);

  a(n+1);
  #pragma GCC diagnostic pop
}

int main()
{
  assert(sizeof(unsigned long) == 8);
  b(0);
}
