#include <stdio.h>

void a()
{
  printf("a()\n");
}

void b()
{
  a();
  printf("b()\n");
}

int main()
{
  b();
}
