#include "stdafx.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int accumulate(std::vector<int>& nums)
{
	auto data = nums.data();
	auto n = nums.size();

	__asm
	{
		mov ebx, data
		mov ecx, n
		jcxz exitFn

		xor eax, eax
		xor edi, edi
	}

sumLoop:
	__asm
	{
		add eax, dword ptr [ebx + edi * 4]
		inc edi
		loop sumLoop
	}

exitFn: __asm {}
}

double average(std::vector<int>& nums)
{
	auto data = nums.data();
	auto n = nums.size();
	double avg = 0.0;

	__asm
	{
		mov ebx, data
		mov ecx, n
		mov edx, ecx

		jcxz exitFn

		xor esi, esi
		finit
		fldz
	}

sumLoop:
	__asm
	{
		fild dword ptr[ebx + esi * 4]
		fadd

		inc esi
		loop sumLoop

		push edx
		fild dword ptr[esp]
		pop edx

		fdiv
		fstp avg
	}

exitFn:
	return avg;
}

void exchangeMinMax(std::vector<int>& nums)
{
	auto data = nums.data();
	auto n = nums.size();
	double avg = 0.0;

	__asm
	{
		mov ecx, n
		jcxz exitFn
		
		xor esi, esi
		xor edi, edi

		mov edx, 0x80000000	// Max
		mov ebx, edx
		dec ebx				// Min
	}

xchgLoop:
	__asm
	{
		mov eax, data
		mov eax, dword ptr [eax + ecx * 4 - 4]

		cmp eax, ebx
		jge noNewMin
		mov ebx, eax
		mov esi, ecx

		noNewMin:
		cmp eax, edx
		jle noNewMax
		mov edx, eax
		mov edi, ecx

		noNewMax:

		loop xchgLoop

		dec esi
		dec edi

		mov ebx, dword ptr [data]

		mov ecx, dword ptr[ebx + esi * 4]
		mov edx, dword ptr[ebx + edi * 4]
		mov dword ptr[ebx + esi * 4], edx
		mov dword ptr[ebx + edi * 4], ecx
	}

exitFn: __asm {}
}

int main()
{
	using std::cin;
	using std::cout;
	using std::string;
	using std::vector;

	int n;
	string input; 

	cout << "Input n, then numbers: ";
	
	cin >> n;

	vector<int> numbers;

	for (int i = 0; i < n; ++i)
	{
		int t;
		cin >> t;
		numbers.push_back(t);
	}

	auto sum = accumulate(numbers);

	auto avg = average(numbers);

	exchangeMinMax(numbers);

	cout << "Sum is: " << sum << std::endl;
	cout << "Average is: " << avg << std::endl;
	
	cout << "Range with exchanged min & max: " << std::endl;

	std::for_each(
		numbers.cbegin(),
		numbers.cend(),
		[](int x) { cout << "\t- " << x << std::endl; }
	);

    return 0;
}