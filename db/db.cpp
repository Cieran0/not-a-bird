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
    for (size_t i = 0; i < user.likedPosts.size(); i++)
    {
        std::cout << "\t\t" << user.likedPosts[i] << "," << std::endl;
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

    if(command == "login") {
        login(argc-2, argv + 2);
    } else if (command == "add-user") {
        add_user(argc-2, argv + 2);
    } else if (command == "add-post") {
        add_post(argc-2, argv + 2);
    } else if (command == "delete-user") {
        delete_user(argc-2, argv + 2);
    } else if (command == "delete-post") {
        delete_post(argc-2, argv + 2);
    } else if (command == "get-post-content") {
        get_post_content(argc-2, argv + 2);
    } else if (command == "get-all-posts") {
        get_all_posts(argc-2, argv + 2);
    }

    return 0;
}
