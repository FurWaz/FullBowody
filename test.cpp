#include <iostream>

int main(int argc, char const *argv[])
{
    while (true)
    {
        std::string s;
        std::cin >> s;
        if (s.size() > 0)
            std::cout << "data of size: " << s.size() << std::endl;
    }
    return 0;
}
