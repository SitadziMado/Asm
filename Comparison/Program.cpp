#include "stdafx.h"

#include "Comparison.h"

int main()
{
    using std::cin;
    using std::cout;

    cout << "Enter two integers: ";

    long lhs, rhs;

    while (!(cin >> lhs >> rhs))
    {
        cout << "Please enter integers: ";
        
        cin.ignore(std::numeric_limits<long long>::max());
        cin.clear();
    }

    cout << "Minimum is: " << Comparison::min(lhs, rhs) << std::endl;
    cout << "Maximum is: " << Comparison::max(lhs, rhs) << std::endl;

    cout << "Now enter five numbers: " << std::endl <<
        "\t- 3 dimensions of a box" << std::endl <<
        "\t- 2 dimensions of a hole" << std::endl;

    long a, b, c, w, h;

    while (!(cin >> a >> b >> c >> w >> h))
    {
        cout << "Please enter integers: ";

        cin.ignore(std::numeric_limits<long long>::max());
        cin.clear();
    }

    auto fits = Comparison::fits(
        a, b, c,
        w, h
    );

    cout << std::endl << (fits ? "It fits!" : "It does not fit, unfortunately.");

    std::system("pause");

    return 0;
}