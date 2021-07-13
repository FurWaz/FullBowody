#include <iostream>
#include <sstream>

int main(int argc, char const *argv[])
{
    int i = 621;
    unsigned long long addr = (unsigned long long)&i;
    std::stringstream str;
    str << &i;
    std::cout << std::addressof(i) << std::endl;
    std::cout << &i << std::endl;
    std::cout << addr << std::endl;
    std::cout << str.str().substr(2, str.str().size()) << std::endl;
    std::cin.get();
    return 0;
}