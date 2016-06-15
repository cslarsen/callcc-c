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
