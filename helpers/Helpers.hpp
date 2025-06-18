#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <User.hpp>

class Channel;

struct ModeChange {
    char action;
    char mode;
    std::string param;
};

class Helpers {
private:
    static std::vector<std::string> __normalize_mode_arguments(const std::vector<std::string>& args);
    static bool __is_valid_mode_char(char c);
    static bool __mode_needs_param(char mode, char action);
public:
	static void right_trim (std::string& str, const char* delims = " \n\t\r");
    static void left_trim (std::string& str, const char* delims = " \n\t\r");
    static void trim (std::string& str, const char* delims = " \n\t\r");
    static std::vector<std::string> parse_msg (const char* msg);
    static std::vector<std::string> parse_msg (const std::string& msg);

    static std::string merge_from (const std::vector<std::string>& tokens, size_t start);
    static std::string reverseDNS(struct sockaddr_in &clientAddr);
    static std::vector<std::string> split_by_delim(const std::string& input, char delim);
    static std::vector<ModeChange> parse_modes_raw(const std::vector<std::string>& raw_input);
    static std::vector<ModeChange> filter_valid_modes(const User& user, const std::vector<ModeChange>& input, Channel& channel);

    template <typename T>
    static std::string to_string(const T& value);
};

template <typename T>
std::string Helpers::to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif // HELPERS_HPP