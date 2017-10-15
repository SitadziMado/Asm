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
            // ��������� ��������� ������� � �������� � ��������� ��
            mov eax, dword ptr[holeWidth]
            mov ebx, dword ptr[holeHeight]
            call sortEaxEbx

            // ���������� �� � ESI � EDI �� �����������
            mov esi, eax
            mov edi, ebx

            // ���������� ��������� � �������, ������� � ������
            // ���������������
            mov eax, dword ptr[brickWidth]
            mov ebx, dword ptr[brickHeight]
            mov ecx, dword ptr[brickLength]

            call sortEaxEbx
            call sortEbxEcx
            call sortEaxEbx

            // EDX - ���� ����, ��������� �� ��������������
            xor edx, edx

            // ���� ������� ������� ��������������� �� ������� 
            // ����� �������
            cmp ecx, edi
            jg negative

            // ���������� ������� ������� � ������� ������� ����
            // ������������� ���� "������ ��� �����" � DL
            cmp ebx, esi
            setle dl

            // ���������� ��� ����� ��������� ������� ���������������
            // ���� � DH
            cmp eax, esi
            setle dh

            // ���� ���� �� ������ ����������,
            // �� � EDX ��������� ��������� ��������
            or dl, dh

            // ���������, ���������� �� ����
            test edx, edx
            jz negative

            // ���������� ������
            mov eax, 1

            jmp short exitFn
        }

    sortEaxEbx:
        _asm
        {
            // ���������� �������� � ������������ � EAX �������
            cmp eax, ebx
            jle short exitSortEaxEbx
            xchg eax, ebx

            exitSortEaxEbx : ret
        }

    sortEbxEcx:
        _asm
        {
            // ���������� �������� � ������������ � EBX �������
            cmp ebx, ecx
            jle short exitSortEbxEcx
            xchg ebx, ecx

            exitSortEbxEcx : ret
        }

    negative:
        __asm
        {
            // ���������� ����
            xor eax, eax
        }

    exitFn: nop
    }
}