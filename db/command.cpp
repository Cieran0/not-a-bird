#include <db.hpp>
#include <io.hpp>

void login(int argsCount, char const *argv[]) {
    loadUsers();
    if(argsCount != 2) {
        std::cout << "Fail" << std::endl;
        return;
    }
    std::string username = argv[0];
    std::string password = argv[1];
    if(!validUser(username,password)) {
        std::cout << "Fail" << std::endl;
    }
    std::cout << "Success" << std::endl;
}

void add_user(int argsCount, char const *argv[]) {
    loadUsers();
    if(argsCount != 2) {
        std::cout << "Fail" << std::endl;
        return;
    }
    std::string username = argv[0];
    std::string password = argv[1];
    hashedString id = hash(username);
    if(users.find(id) != users.end()) {
        std::cout << "Fail" << std::endl;
        return;
    }
    addUser(username,password);
    saveUser(users[id]);
    std::cout << "Success" << std::endl;
}

void add_post(int argsCount, char const *argv[]) {
    loadUsers();
    loadPosts();
    if(argsCount != 3) {
        std::cout << "Fail" << std::endl;
        return;
    }
    std::string username = argv[0];
    hashedString authorID = hash(username);
    std::string password = argv[1];
    std::string content = argv[2];
    hashedString id = hashPost(content,authorID);
    if(posts.find(id) != posts.end()) {
        std::cout << "Fail" << std::endl;
        return;
    }
    if(!validUser(username,password)) {
        std::cout << "Fail" << std::endl;
        return;
    }
    addPost(authorID,content);
    savePost(posts[id]);
    std::cout << "Success" << std::endl;
}

void delete_user(int argsCount, char const *argv[]) {
    loadUsers();
    if(argsCount != 2) {
        std::cout << "Fail" << std::endl;
        return;
    }
    std::string username = argv[0];
    std::string password = argv[1];
    if(!validUser(username,password)) {
        std::cout << "Fail" << std::endl;
        return;
    }
    deleteUser(hash(username));
    std::cout << "Success" << std::endl;
}

void delete_post(int argsCount, char const *argv[]) {
    loadUsers();
    loadPosts();
    if(argsCount != 3) {
        std::cout << "Fail" << std::endl;
        return;
    }
    std::string username = argv[0];
    std::string password = argv[1];
    std::string postIDString = argv[2];
    if(!validUser(username,password)) {
        std::cout << "Fail" << std::endl;
    }
    hashedString postID = parseHashedString(postIDString);
    if(posts.find(postID) == posts.end()) {
        std::cout << "Fail" << std::endl;
        return;
    }
    deletePost(postID);
    std::cout << "Success" << std::endl;
}

void get_post_content(int argsCount, char const *argv[]) {
    loadPosts();
    if(argsCount != 1) {
        return;
    }
    std::string postIDString = argv[0];
    hashedString postID = parseHashedString(postIDString);
    if(posts.find(postID) == posts.end()) {
        return;
    }
    std::cout << posts[postID].content;
}

void get_all_posts(int argsCount, char const *argv[]) { 
    loadPosts();
    if(argsCount != 0) {
        std::cout << "Fail" << std::endl;
        return;
    }
    for (std::pair<hashedString,Post> pair : posts)
    {
        std::cout << pair.first << std::endl;
    }
}