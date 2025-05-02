#include <IRCServer.hpp>
#include <Parsing.hpp>
#include <InputError.hpp>
#include <UserTable.hpp>
#include <User.hpp>

int main (int argc, char* argv[]) try {
    std::pair<int, std::string> input = Parsing::validate_input(argc, argv);

    IRCServer server(input.first, input.second);
    server.setupServer();
    server.run();
}
catch (const IRC::InputError& ex) {
    std::clog << "InputError: " << ex.what() << std::endl;
}
catch (const IRC::exception& ex) {
    std::clog << "Error: " << ex.what() << std::endl;
}

// #include <vector>

// int main () try {
//     std::vector<User> vec;

//     vec.push_back(User(1, "asdf", "asdf", "asdf"));
//     vec.push_back(User(2, "valod", "valod", "valod"));
//     vec.push_back(User(3, "gegh", "gegh", "gegh"));
//     vec.push_back(User(4, "art", "art", "art", true));

//     UserTable ut;

//     for (int i = 0; i < vec.size(); ++i) {
//         ut.set_user(vec[i]);
//     }

//     ut.remove_user(1);
//     ut.set_user_nickname(2, "vayaa");
//     ut.set_user_hostname(2, "iee");

//     for (int i = 2; i <= vec.size(); ++i) {
//         std::cout << ut.get_user(i) << std::endl;
//         std::cout << "----------------------" << std::endl;
//     }
// } catch (...) {
//     std::cout << "chelav aper" << std::endl;
// }