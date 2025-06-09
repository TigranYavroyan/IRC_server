#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <sstream>
#include <vector>

class Helpers {
public:
	static void right_trim (std::string& str, const char* delims = " \n\t\r");
    static void left_trim (std::string& str, const char* delims = " \n\t\r");
    static void trim (std::string& str, const char* delims = " \n\t\r");
    static std::vector<std::string> parse_msg (const char* msg);
    static std::vector<std::string> parse_msg (const std::string& msg);

    static std::string merge_from (const std::vector<std::string>& tokens, size_t start);

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