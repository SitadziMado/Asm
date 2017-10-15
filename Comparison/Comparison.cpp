#include "stdafx.h"
#include "Comparison.h"

#define nop _asm {}

namespace Comparison
{
    long max(long lhs, long rhs)
    {
        __asm
        {
            mov eax, dword ptr[lhs]
            mov ebx, dword ptr[rhs]
            cmp eax, ebx
            jge short exitFn

            mov eax, ebx
        }

    exitFn: nop
    }

    long min(long lhs, long rhs)
    {
        __asm
        {
            mov eax, dword ptr[lhs]
            mov ebx, dword ptr[rhs]
            cmp eax, ebx
            jle short exitFn

            mov eax, ebx
        }

    exitFn: nop
    }

    bool fits(
        long brickWidth, 
        long brickHeight, 
        long brickLength, 
        long holeWidth, 
        long holeHeight
    )
    {
        __asm
        {
            // Загружаем параметры функции в регистры и сортируем их
            mov eax, dword ptr[holeWidth]
            mov ebx, dword ptr[holeHeight]
            call sortEaxEbx

            // Перемещаем их в ESI и EDI по возрастанию
            mov esi, eax
            mov edi, ebx

            // Аналогично поступаем с шириной, высотой и длиной
            // параллелепипеда
            mov eax, dword ptr[brickWidth]
            mov ebx, dword ptr[brickHeight]
            mov ecx, dword ptr[brickLength]

            call sortEaxEbx
            call sortEbxEcx
            call sortEaxEbx

            // EDX - флаг того, вмещается ли параллелепипед
            xor edx, edx

            // Если большая сторона параллелепипеда не влезает 
            // сразу выходим
            cmp ecx, edi
            jg negative

            // Сравниваем среднюю сторону и меньшую сторону дыры
            // устанавливаем флаг "меньше или равен" в DL
            cmp ebx, esi
            setle dl

            // Аналогично для самой маленькой стороны параллелепипеда
            // Флаг в DH
            cmp eax, esi
            setle dh

            // Если один из флагов установлен,
            // то в EDX получится ненулевое значение
            or dl, dh

            // Проверяем, установлен ли флаг
            test edx, edx
            jz negative

            // Возвращаем истину
            mov eax, 1

            jmp short exitFn
        }

    sortEaxEbx:
        _asm
        {
            // Сравниваем регистры и переставляем в EAX меньшее
            cmp eax, ebx
            jle short exitSortEaxEbx
            xchg eax, ebx

            exitSortEaxEbx : ret
        }

    sortEbxEcx:
        _asm
        {
            // Сравниваем регистры и переставляем в EBX меньшее
            cmp ebx, ecx
            jle short exitSortEbxEcx
            xchg ebx, ecx

            exitSortEbxEcx : ret
        }

    negative:
        __asm
        {
            // Возвращаем ложь
            xor eax, eax
        }

    exitFn: nop
    }
}