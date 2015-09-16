/*
 * Undelimited continuations in C
 * Broken code by Christian Stigen Larsen
 *
 * Work in progress ... trying to figure this out myself, instead of cheating
 * (too much). The general strategy is to just copy the entire call stack and
 * then reinstate it. (I *know* this works, because others have done it).
 */

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t* base_rbp = 0;

struct continuation {
  const uint8_t* rip;
  const uint8_t* rbp;
  size_t size;
  uint8_t* frames;
};

static void cont_assert_init()
{
  assert(base_rbp != 0 && "Uninitialized; cont_init() not called");
}

struct continuation* cont_new()
{
  cont_assert_init();
  struct continuation *c = malloc(sizeof(struct continuation));
  memset(c, 0, sizeof(struct continuation));
  return c;
}

void cont_destroy(struct continuation* c)
{
  if ( c->frames )
    free(c->frames);
  free(c);
}

void cont_alloc_frames(struct continuation* c, const size_t size)
{
  printf("allocing frame size %zu\n", size);
  c->size = size;
  c->frames = malloc(size);
}

void cont_init()
{
  assert(sizeof(size_t) == 8 && "Only lp64 architectures supported");
  assert(sizeof(void*) == 8 && "Only lp64 architectures supported");

  __asm__ __volatile__ (
    "movq %%rbp, %[rbp]\n"
    : [rbp] "=r" (base_rbp));

  printf("base_rbp=%p\n", base_rbp);
}

struct continuation* cont_reify()
{
  struct continuation* c = cont_new();

  __asm__ __volatile__ (
    "movq %%rbp, %[rbp]\n"
    : [rbp] "=r" (c->rbp));

  assert(base_rbp > c->rbp && "Call stack grows wrong way; unsupported");
  const size_t size = base_rbp - c->rbp;

  printf("base=%p\n", base_rbp);
  printf(" rbp=%p\n", c->rbp);
  printf("%zu bytes\n", size);

  cont_alloc_frames(c, size);
  printf("copy %zu bytes from %p\n", size, c->rbp);
  memcpy(c->frames, c->rbp, size);

  // get rip
  __asm__ __volatile__ (
      "    call _cont_reify_rip\n"
      "_cont_reify_rip:\n"
      "    pop %%rax\n"
      "    add $0x8, %%rax\n"
      "    mov %%rax, %[rip]\n"
      : [rip] "=r" (c->rip));

  printf("done reify, rip is %p\n", c->rip);

  return c;
}

void cont_reinstate(struct continuation* c)
{
  printf("reinstating %zu bytes %p\n", c->size, c);

  // copy stack frames back
//  memcpy((uint8_t*)c->rbp, c->frames, c->size);

  printf("setting rbp and jumping back\n");

  // set rbp and jump back
  __asm__ __volatile__ (
      "movq %[rip], %%rcx\n"
      "movq %[rbp], %%rbp\n"
      "jmpq *%%rcx"
      :
      : [rip] "r" (c->rip),
        [rbp] "r" (c->rbp));

  printf("*** should never been reached ***\n");
}

struct continuation* test()
{
  printf("test start\n");
  struct continuation* c = cont_reify();
  printf("test end\n");
  return c;
}

int main()
{
  cont_init();

  printf("main start\n");
  struct continuation* c = test();
  cont_reinstate(c);

  printf("main end\n");
  return 0;
}
