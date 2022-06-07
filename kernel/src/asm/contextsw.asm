[EXTERN THREAD]
[EXTERN THREAD_NEXT]
[GLOBAL _context_switch]

_context_switch:
    mov eax, [THREAD]
    
    mov [eax + 0],  edi
    mov [eax + 4],  esi
    mov [eax + 8],  ebp
    mov [eax + 12], esp
    mov [eax + 16], ebx
    mov [eax + 20], edx
    mov [eax + 24], ecx
    mov [eax + 28], eax

    pushf
    pop ecx
    mov [eax + 32], ecx

    mov eax, [THREAD_NEXT]
    mov [THREAD], eax

    mov ecx, [eax + 32]
    push ecx
    popf

    mov edi, [eax + 0]
    mov esi, [eax + 4]
    mov ebp, [eax + 8]
    mov esp, [eax + 12]
    mov ebx, [eax + 16]
    mov edx, [eax + 20]
    mov ecx, [eax + 24]
    mov eax, [eax + 28]

    sti
    ret