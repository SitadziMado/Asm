#include "stdafx.h"
#include <cstdlib>
void* my_memset(void* dst, char value, size_t sizes)
{
	__asm
	{
		// Загружаем исходные значения в регистры
		mov edi, dword ptr [dst]
		mov al, byte ptr [value]
		mov ecx, dword ptr [sizes]

		// Заполняем память
		cld
		rep stosb
	}
}

char* my_itoa(char* buffer, int number, int base)
{
#define BINARY (2)
#define DECIMAL (10)
	__asm
	{
		xor esi, esi
		mov edi, dword ptr [buffer]
		mov eax, dword ptr [number]
		mov ebx, dword ptr [base]

		// База не может быть нулевой
		test ebx, ebx
		jz short err

		// ...и больше, чем латинских букв
		cmp ebx, 0Ah + ('A' - 'Z')
		ja short err

		cld
		xor ecx, ecx

		cmp ebx, BINARY
		jz short num_to_bin

		cmp ebx, DECIMAL
		jnz short num_to_chars

		cmp eax, 0
		jg skip_neg
		neg eax
		mov esi, '-' - '0'

		skip_neg:

		num_to_dec:
			xor edx, edx
			div ebx
			push edx

			// Увеличиваем счетчик
			inc ecx

		test eax, eax
		jnz short num_to_dec
		
		test esi, esi
		jz short store_dec_chars
		push esi
		inc ecx
		jmp short store_dec_chars

		num_to_chars:
			xor edx, edx
			div ebx
			push edx

			// Увеличиваем счетчик
			inc ecx

		test eax, eax
		jnz short num_to_chars

		store_chars:
			pop eax

			cmp eax, DECIMAL
			jb short skip_al
			add eax, 'A' - '0' - 10
			
			skip_al:
				add eax, '0'
				stosb
		loop store_chars
		jmp short success

		num_to_bin:
			shr eax, 1
			jc short one
			push 0
			jmp short after_one

			one:
				push 1
		
			after_one:
				inc ecx
		test eax, eax
		jnz short num_to_bin

		store_dec_chars:
			pop eax
			add eax, '0'
			stosb
		loop store_dec_chars

		jmp short success

		err:
			xor edi, edi

		success:
			xor eax, eax
			stosb
			mov eax, edi
	}
#undef DECIMAL
#undef BINARY
}

int main()
{
	short r = 0;

	char abc[256];

	my_itoa(abc, 453, 2);

	my_memset(abc, 0, 256);

	return 0;
}