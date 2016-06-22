; Needed for OSX, you may have to tune these for your platform
bits 64

%ifidn __OUTPUT_FORMAT__,macho64
  align 16
%endif

global _getrbp
global _getrdi
global _getrip
global _getrsp
global _jmp
global _save

section .text

_getrbp:
  mov rax, rbp
  ret

_getrip:
  mov rax, [rsp]
  ret

_getrdi:
  mov rax, rdi
  ret

_getrsp:
  mov rax, rbp
  ret

; void jmp(void* rbp, void* rsp, void* rdi, void* rip);
_jmp:
  mov rbp, rdi
  mov rsp, rsi
  mov rdi, rdx
  push rbp ; needed?
  ; set a return value
  mov rax, 50
  jmp rsi

; Saves all registers to input pointer
_save:
  mov qword [rdi+0*8], rbp
  mov qword [rdi+1*8], rsp
  mov qword [rdi+2*8], rax
  mov qword [rdi+3*8], rbx
  mov qword [rdi+4*8], r12
  mov qword [rdi+5*8], r13
  mov qword [rdi+6*8], r14
  mov qword [rdi+7*8], r15

  lea rax, [rsp+1*8]
  mov qword [rdi+8*8], rax

  fnstcw [rdi+9*8]
  stmxcsr [rdi+10*8]
  xor rax, rax
  ret

   ;0x00007fff97c9b334 <+0>: mov    %rbx,(%rdi)
   ;0x00007fff97c9b337 <+3>: mov    %rbp,0x8(%rdi)
   ;0x00007fff97c9b33b <+7>: mov    %r12,0x18(%rdi)
   ;0x00007fff97c9b33f <+11>:  mov    %r13,0x20(%rdi)
   ;0x00007fff97c9b343 <+15>:  mov    %r14,0x28(%rdi)
   ;0x00007fff97c9b347 <+19>:  mov    %r15,0x30(%rdi)
   ;0x00007fff97c9b34b <+23>:  mov    (%rsp),%rax
   ;0x00007fff97c9b34f <+27>:  mov    %rax,0x38(%rdi)
   ;0x00007fff97c9b353 <+31>:  lea    0x8(%rsp),%rax
   ;0x00007fff97c9b358 <+36>:  mov    %rax,0x10(%rdi)
   ;0x00007fff97c9b35c <+40>:  fnstcw 0x4c(%rdi)
   ;0x00007fff97c9b35f <+43>:  stmxcsr 0x48(%rdi)
   ;0x00007fff97c9b363 <+47>:  xor    %eax,%eax
   ;0x00007fff97c9b365 <+49>:  retq
