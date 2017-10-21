.586
.MODEL FLAT, C
.STACK
.DATA

$base dd 10.0

.CODE

PUBLIC my_atod

BASE EQU 10
FRACTION_SIZE EQU 52
EXP_BIAS EQU 1023
AND_ROUND EQU 0F3FFh
OR_TRUNCATE EQU 00C00h

my_atod PROC string : PTR BYTE
LOCAL $result : QWORD, \
      $sign : DWORD, \
      $exp : DWORD, \
      $digit : DWORD, \
      $cw : WORD
    
; Load up string pointer and check if it is zero
                mov esi, string
                xor eax, eax
                mov al, [esi]
                cmp eax, 0
                jz err
                
                cld
                xor ebx, ebx
                xor ecx, ecx
                xor edx, edx
                mov edi, BASE
                
                lodsb
                
                call skipWs
                test eax, eax
                jz err
                
                cmp al, '-'
                jnz noMinus
                mov $sign, 1
                lodsb
                call skipWs
                jmp prepare
                
    noMinus:    cmp al, '+'
                jnz noSign
                lodsb
                call skipWs
                
    noSign:     mov $sign, 0
                
    prepare:    test eax, eax
                jz err
                mov $exp, -1

                finit
                fld $base   ; 10
                fldz        ; 0, 10
                
    intCycle:   fmul st(0), st(1) ; 0, 10
                
                cmp al, '0'
                jb err
                cmp al, '9'
                ja err
                
                sub eax, '0'
                mov $digit, eax
                fild $digit ; a, 0, 10
                fadd        ; a + 0, 10
                
                lodsb
                cmp al, 0
                jz form
                cmp al, '.'
                jz delimeter
                cmp al, 'e'
                jz exp
                cmp al, 'E'
                jz exp

                jmp intCycle
                
    delimeter:  fxch st(1)    ; 10, acc
                fldz
                lodsb
                
    fracCycle:  fmul st(0), st(1) ; 0, 10, acc
    
                cmp al, '0'
                jb err
                cmp al, '9'
                ja err
                
                sub eax, '0'
                mov $digit, eax
                fild $digit
                fadd
                inc ecx
                
                lodsb
                cmp al, 0
                jz fform
                cmp al, 'e'
                jz fexp
                cmp al, 'E'
                jz fexp

                jmp fracCycle
            
    fexp:       ; Parse exp
                
                ; acc2, 10, acc
    fform:      fxch st(1)          ; 10, acc2, acc
                mov $digit, ecx
                fild $digit         ; ecx, 10, acc2, acc
                fxch st(1)          ; 10, ecx, acc2, acc
                fyl2x               ; ecx * log2(10), acc2, acc
                fld1                ; 1, ecx * log2(10), acc2, acc
                fscale              ; 10 ^ ecx, ecx * log2(10), acc2, acc
                fxch st(1)          ; ecx * log2(10), 10 ^ ecx, acc2, acc
                
                fld st(0)
                fstcw $cw
                mov ax, $cw
                and ax, AND_ROUND
                or ax, OR_TRUNCATE
                mov $cw, ax
                fldcw $cw
                frndint
                and ax, AND_ROUND
                mov $cw, ax
                fldcw $cw
                fsub
                ;fxtract
                ;fincstp             ; frac, ecx * log2(10), 10 ^ ecx, acc2, acc
                ;fxch st(1)
                ;fstp $result ; just pop             ; frac, 10 ^ ecx, acc2, acc
                
                f2xm1               ; 2 ^ frac - 1, 10 ^ ecx, acc2, acc
                fld1                ; 1, 2 ^ frac - 1, 10 ^ ecx, acc2, acc
                fadd                ; 2 ^ frac, 10 ^ ecx, acc2, acc
                fmul                ; 10 ^ ecx, acc2, acc
                
                fdiv                ; acc2 / 10 ^ ecx, acc
                fadd
				fld st(0)

                jmp form
            
    exp:        ; Parse exp
                
                
    form:       ;  acc, 10
				fxch st(1)
                fstp $result
                fstp $result
                jmp exit
                
    finale:     mov DWORD PTR [$result], eax
                mov DWORD PTR [$result + 1], edx
                jmp exit
    
    skipWs:     cmp al, ' '
                jnz exitSkipWs
                lodsb
                jmp skipWs
    exitSkipWs: retn
    
; Store NaN in this bih
    err:        xor eax, eax
                mov DWORD PTR [$result], 0FFFFFFFFh
                mov DWORD PTR [$result + 1], 07FFFFFFFh
        
    exit:       fld $result
				cmp $sign, 0
				jz exitFn
				fchs

    exitFn:     ret
    
my_atod ENDP

END