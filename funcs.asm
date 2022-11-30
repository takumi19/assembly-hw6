section .text
global f1
global f2
global f3
global t_f1
global t_f2
global t_f3
f1:
    push ebp
    mov ebp, esp
    sub esp, 4
    mov dword [esp], 3
    fld qword [ebp + 8]
    fld1
    fsubp
    fld st0
    fmulp
    fld1
    faddp
    fild dword [esp]
    fxch
    fdivp
    mov esp, ebp
    pop ebp
    ret
    
f2:
    push ebp
    mov ebp, esp
    sub esp, 4
    mov dword [esp], 2
    fld1
    fild dword [esp]
    fdivp
    fld qword [ebp + 8]
    faddp
    fsqrt
    mov esp, ebp
    pop ebp
    ret
    
f3:
    push ebp
    mov ebp, esp
    sub esp, 8
    mov dword [esp], 2
    fldl2e
    fild dword [esp]
    fyl2x
    fist dword [ebp - 4]
    fild dword [ebp - 4]
    fsubp
    f2xm1
    fld1
    faddp
    fild dword [ebp - 4]
    fxch
    fscale
    fstp st1
    fld qword [ebp + 8]
    fchs
    fxch
    fyl2x
    fist dword [ebp - 4]
    fild dword [ebp - 4]
    fsubp
    f2xm1
    fld1
    faddp
    fild dword [ebp - 4]
    fxch
    fscale
    fstp st1
    leave 
    ret

t_f1:
    push ebp
    mov ebp, esp
    sub esp, 4
    mov dword [esp], 5
    fld qword [ebp + 8]
    fld qword [ebp + 8]
    fmulp
    fchs
    fild dword [esp]
    faddp
    leave
    ret

t_f2:
    push ebp
    mov ebp, esp
    sub esp, 4
    mov dword [esp], 4
    fld qword[ebp+8]
    fld qword[ebp+8]
    fmulp
    fild dword [esp]
    fmulp
    fld qword [ebp + 8]
    fsubp
    leave
    ret
    
t_f3:
    push ebp
    mov ebp, esp
    sub esp, 4
    mov dword [esp], 2
    fld qword [ebp + 8]
    fld qword [ebp + 8]
    fld qword [ebp + 8]
    fld qword [ebp + 8]
    fmulp
    fmulp
    fmulp
    fild dword [esp]
    faddp
    leave
    ret
    
