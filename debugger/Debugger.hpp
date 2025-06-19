#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <User.hpp>
#include <Helpers.hpp>

class Channel;

class Debugger {
    static void __debug_start ();
    static void __debug_end ();
    static std::string __debug_msg ();
public:
    static void print_tokens (const std::vector<std::string>& tokens);
    static void client_pass (const User& client);
    static void client_nick (const User& client);
    static void client_user (const User& client);
    static void print_input (const std::vector<std::string>& input);
    static void mode_print_commands (const std::vector<ModeChange>& commands);
    static void exception_msg (const std::exception& ex);
    static void channel_user_count (const Channel& channel);
};

#endif // DEBUGGER_HPP