#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "calculation-tree.hh"
#include "parsing.hh"
#include "parsing-exceptions.hh"

using namespace calculation;
using namespace infix_parsing;

int main()
{
    init_table();
    std::cout << std::defaultfloat;
    std::string buffer;
    std::shared_ptr<Operand> res;
    do {
        std::cout << "> ";
        std::getline(std::cin, buffer);
        if (buffer.empty())
            break;
        try {
            res = parse_expression(buffer);
        } catch (const ParserError &e) {
            std::cout << buffer << std::endl;
            for (size_t i = 0; i < e.position; ++i)
                std::cout << ' ';
            std::cout << '^' << std::endl;
            std::cout << e.what() << std::endl;
            continue;
        }
        std::cout << res->str() << std::endl;
        std::cout << res->evaluate() << std::endl;
    } while (true);
    return 0;
}
