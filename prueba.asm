\\DATA 100
\\EXTRA 100
\\STACK 100

            PUSH    4
            push    3
            call    func
            add     sp,8
    func:   push    bp
            mov     bp, sp
    sigue:  cmp     edx,[bp+8]
            jz      fin
            mul     eax,[bp+12]
            add     edx,1
            jmp     sigue
    fin:    pop     edx
            pop     ebx
            mov     sp, bp
            pop     bp
            ret