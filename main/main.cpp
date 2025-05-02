#include <IRCServer.hpp>
#include <Parsing.hpp>
#include <InputError.hpp>
#include <ServerError.hpp>
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
catch (const IRC::ServerError& ex) {
    std::clog << "ServerError: " << ex.what() << std::endl;
}

// #include <vector>

// int main () try {
//     std::vector<User> vec;

//     vec.push_back(User(1, "asdf", "asdf", "asdf"));
//     vec.push_back(User(2, "valod", "valod", "valod"));
//     vec.push_back(User(3, "gegh", "gegh", "gegh"));
//     vec.push_back(User(4, "art", "art", "art", true));

//     UserTable ut;

//     for (std::size_t i = 0; i < vec.size(); ++i) {
//         ut.set_user(vec[i]);
//     }

//     ut.remove_user(1);
//     ut.set_user_nickname(2, "vayaa");
//     ut.set_user_hostname(2, "iee");
//     ut.remove_user(2);

//     for (std::size_t i = 3; i <= vec.size(); ++i) {
//         std::cout << ut.get_user(i) << std::endl;
//         std::cout << "----------------------" << std::endl;
//     }
// } catch (...) {
//     std::cout << "chelav aper" << std::endl;
// }