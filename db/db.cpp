#include <db.hpp>
#include <commands.hpp>

std::unordered_map<hashedString,User> users;
std::unordered_map<hashedString,Post> posts;

void printUser(User user) {
    std::cout << "{" << std::endl;
    std::cout << "\tid: " << "\"" << user.id << "\"," << std::endl;
    std::cout << "\tusername: " << "\"" << user.username << "\"," << std::endl;
    std::cout << "\tpasswordHashed: " << "\"" << user.passwordHashed << "\"," << std::endl;
    std::cout << "\tlikedPosts: [" << std::endl;
    for (unsigned long long &likedPost :  user.likedPosts)
    {
        std::cout << "\t\t" << likedPost << "," << std::endl;
    }
    
    std::cout << "\t]," << std::endl;
    std::cout << "};" << std::endl;
}

int main(int argc, char const *argv[])
{
    if(argc < 2) {
        return 0;
    }

    if (std::filesystem::current_path().filename().string() != "db") {
        std::string dbDirectoryName = (std::filesystem::current_path() / "db");
        if (std::filesystem::exists(dbDirectoryName) && std::filesystem::is_directory(dbDirectoryName)) { 
            std::filesystem::current_path(std::filesystem::current_path() / "db");
        } else {
            std::cout << (std::filesystem::current_path() / "db") << std::endl;
            std::cout << "db directory doesn't exist" << std::endl;
        }
    }

    std::string command = argv[1];
    const int ARG_COUNT = argc - 2;
    const char** ARG_VALUES = argv + 2;
    #define ARGS ARG_COUNT, ARG_VALUES

    if(command == "login") {
        login(ARGS);
    } else if (command == "add-user") {
        add_user(ARGS);
    } else if (command == "add-post") {
        add_post(ARGS);
    } else if (command == "delete-user") {
        delete_user(ARGS);
    } else if (command == "delete-post") {
        delete_post(ARGS);
    } else if (command == "get-post") {
        get_post(ARGS);
    } else if (command == "get-all-posts") {
        get_all_posts(ARGS);
    } else if (command == "get-username") {
        get_username(ARGS);
    } else if (command == "toggle-like-post") {
        toggle_like_post(ARGS);
    } else if (command == "get-user") {
        get_user(ARGS);
    } else {
        std::cout << "Invalid command!" << std::endl;
    }

    return 0;
}
