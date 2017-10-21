#include "stdafx.h"

#include "Convertion.h"

int main()
{
    const double c = 11111111111111111111111111111111111111111111111111.0e+121;

    using std::cin;
    using std::cout;

    std::string input;

    cout << "Input a real number: ";
    cin >> input;

    auto parsed = my_atod(input.data());

    cout << "Parsed number: " << parsed;

    return 0;
}