#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <vector>
#include <string>
#include <iostream>

class Debugger {
    static void __debug_start();
    static void __debug_end();
public:
    static void print_tokens (const std::vector<std::string>& tokens);
};

#endif // DEBUGGER_HPP