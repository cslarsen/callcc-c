void jmp(void* rbp, void* rsp, void* rdi, void* rip) asm("_jmp");
void* getrbp() asm("_getrbp");
void* getrdi() asm("_getrdi");
void* getrip() asm("_getrip");
void* getrsp() asm("_getrsp");
void* save() asm("_save");
