#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <User.hpp>

class Debugger {
    static void __debug_start ();
    static void __debug_end ();
    static std::string __debug_msg ();
public:
    static void print_tokens (const std::vector<std::string>& tokens);
    static void client_pass (const User& client);
    static void client_nick (const User& client);
    static void client_user (const User& client);
};

#endif // DEBUGGER_HPP