#include "stdafx.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int accumulate(std::vector<int> nums)
{
	auto data = nums.data();
	auto n = nums.size();

	__asm
	{
		mov ebx, data
		mov ecx, n
		test ecx, ecx
		jz exitFn

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

double average(std::vector<int> nums)
{
	auto data = nums.data();
	auto n = nums.size();
	double avg = 0.0;

	__asm
	{
		mov ebx, data
		mov ecx, n
		mov edx, ecx

		test ecx, ecx
		jz exitFn

		xor esi, esi
		finit
		fldz
	}

sumLoop:
	__asm
	{
		inc esi

		fild dword ptr[ebx + esi * 4]
		faddp st0, st1
		loop sumLoop

		push edx
		fild dword ptr[esp]
		pop edx

		fdiv
		fst avg
	}

exitFn:
	return avg;
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
	
	cin >> n >> input;

	vector<int> numbers;

	for (int i = 0; i < n; ++i)
	{
		int t;
		cin >> t;
		numbers.push_back(t);
	}

	auto a = accumulate(numbers);

	auto avg = average(numbers);

    return 0;
}