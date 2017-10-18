#include "stdafx.h"

double my_atod(char *string);

int main()
{
	using std::cin;
	using std::cout;

	std::string input;

	cin >> input;

	my_atod(input.data());

    return 0;
}

double my_atod(const char* string)
{
	double radix = 10.0;
	double rv = 0.0;

	__asm
	{
		cld
		mov esi, string

		lodsb
		test al, al
		jz exitFn

		call skipWs

		xor edi, edi // Sign register

		cmp al, '-'
		jnz noMinus
		inc edi
		lodsb
		call skipWs

		///noMinus:
		cmp al, '+'
		jnz noSign
		lodsb
		call skipWs

		///noSign:

		xor eax, eax
		xor ebx, ebx
		xor ecx, ecx
		xor edx, edx

		finit
		fld dword ptr [radix]
		fldz
	}

parseLoop:
	__asm
	{
		mul st, st2 // 

		cmp al, 0
		jz exitParseLoop
		cmp al, '.'
		jz dotParse
		cmp al, '0'
		jb exitFN
		cmp al, '9'
		ja exitFn

		sub al, '0'
		push eax
		fild dword ptr [esp]
		pop eax
	}

dotParse:
	__asm
	{

	}

exitParseLoop:
	__asm
	{

	}

skipWs:
	__asm
	{
		test al, ' '
		jnz exitSkipWs
		lodsb
		jmp skipWs

		///exitSkipWs:
		ret
	}

exitFn:
	return rv;
}