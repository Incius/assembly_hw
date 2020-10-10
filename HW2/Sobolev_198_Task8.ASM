
;Соболев Антон БПИ198 Вариант 8
;Создать массив,где B[i]=A[i]+5, если A[i]>5 или B[i]= A[i]-5, если A[i]<5, иначе B[i]=0

format PE console
entry start

include 'win32a.inc'

section '.data' data readable writable
        ;Strings
        strSizeReq     db 'Enter size of A: ', 0
        strIntOut     db '%d', 0
        strA    db 'A[%d]: ', 0
        strResult      db 10,'B:', 10, 0
        strB  db 'B[%d]: %d', 10, 0
        ;Iterators
        i  dd ?
        tmp  dd ?
        ;Array
        arr rd 100
        size dd 0
section '.code' code readable executable
start:
        call Input
        call CreateNewArr
        call Output
finish:
        call [getch]
        push 0
        call [ExitProcess]
Input:
        push strSizeReq
        call [printf]
        add esp, 4
        push size; Size input
        push strIntOut
        call [scanf]
        add esp, 8
        mov eax, [size]
        mov ecx,0
        mov ebx, arr
arrLoop:
        mov [tmp], ebx
        cmp ecx, [size]
        jge endInput;Loop End
        mov [i], ecx
        push ecx
        push strA;Input Request
        call [printf]
        add esp, 8
        push ebx
        push strIntOut;Reading Input
        call [scanf]
        add esp, 8
        mov ecx, [i]
        inc ecx
        mov ebx, [tmp];Saving
        add ebx, 4
        jmp arrLoop
endInput:
        ret
;Change Array
CreateNewArr:
        mov ebx, arr
        mov [i],0
newLoop:
        mov ecx, [i]
        cmp ecx, [size]
        je endCreate
        cmp dword [ebx], -5;Comparing
        jl Decrease
        cmp dword [ebx], 5
        jg Increase
        mov   dword [ebx],0;Zeroing
        inc [i]
        add ebx, 4
        jmp newLoop
;Changing values in arr
Decrease:
        add dword[ebx],-5
        add ebx, 4
        inc [i]
        jmp newLoop
Increase:
        add dword[ebx],5
        add ebx, 4
        inc [i]
        jmp newLoop
endCreate:
        ret
;Output new array
Output:
        push strResult
        call [printf]
        add esp, 4
        mov ecx,0
        mov ebx, arr
putVecLoop:
        mov [tmp], ebx
        cmp ecx, [size]
        je endOutputVector
        mov [i], ecx
        push dword [ebx]
        push ecx
        push strB
        call [printf]
        mov ecx,[i]
        inc ecx
        mov ebx,[tmp]
        add ebx, 4
        jmp putVecLoop
endOutputVector:
        call [getch]
        ret
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'
include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'