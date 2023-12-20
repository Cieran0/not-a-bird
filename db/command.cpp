#include <db.hpp>
#include <io.hpp>
#include <commands.hpp>

void login(COMMAND_ARGS) {
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

void add_user(COMMAND_ARGS) {
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

void add_post(COMMAND_ARGS) {
    loadUsers();
    loadPosts();
    if(argsCount != 3) {
        std::cout << "Fail" << std::endl;
        std::cout << "Wrong number of args" << std::endl;
        return;
    }
    std::string username = argv[0];
    hashedString authorID = hash(username);
    std::string password = argv[1];
    std::string content = argv[2];
    hashedString timeStamp = timeSinceEpoch();
    hashedString id = hashPost(content,authorID,timeStamp);
    std::cout << "Post id: " << id << std::endl;
    if(posts.find(id) != posts.end()) {
        std::cout << "Fail" << std::endl;
        std::cout << "Post id already exists" << std::endl;
        return;
    }
    if(!validUser(username,password)) {
        std::cout << "Fail" << std::endl;
        std::cout << "User broke" << std::endl;
        return;
    }
    addPost(authorID,content,timeStamp);
    savePost(posts[id]);
    std::cout << "Success" << std::endl;
}

void delete_user(COMMAND_ARGS) {
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

void delete_post(COMMAND_ARGS) {
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

void get_post(COMMAND_ARGS) {
    loadPosts();
    if(argsCount != 1) {
        return;
    }
    std::string postIDString = argv[0];
    hashedString postID = parseHashedString(postIDString);
    if(posts.find(postID) == posts.end()) {
        return;
    }
    std::cout << serializePost(posts[postID]);
}

void get_all_posts(COMMAND_ARGS) { 
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

void get_username(COMMAND_ARGS) {
    loadUsers();
    if(argsCount != 1) {
        return;
    }
    std::string userIDString = argv[0];
    hashedString userID = parseHashedString(userIDString);
    if(users.find(userID) == users.end()) {
        return;
    }
    std::cout << users[userID].username;
}

void ToggleLikePost(hashedString userID, hashedString postID) {
    bool alreadyLiked = false;
    for(unsigned long long &likedID : users[userID].likedPosts) {
        if(likedID == postID) {
            alreadyLiked = true;
            break;
        }
    }
    if(!alreadyLiked) {
        users[userID].likedPosts.push_back(postID);
        posts[postID].likeCount++;
    } else {
        users[userID].likedPosts.remove(postID);
        posts[postID].likeCount--;
    }
    saveUser(users[userID]);
    savePost(posts[postID]);

}

void toggle_like_post(COMMAND_ARGS) {
    loadPosts();
    loadUsers();
    if(argsCount != 3) {
        std::cout << "Fail" << std::endl;
        std::cout << "Wrong number of args" << std::endl;
        return;
    }
    std::string username = argv[0];
    std::string password = argv[1];
    std::string postIDString = argv[2];
    if(!validUser(username,password)) {
        std::cout << "Fail" << std::endl;
        std::cout << "Invalid user" << std::endl;
        return; 
    }
    hashedString postID = parseHashedString(postIDString);
    if(posts.find(postID) == posts.end()) {
        std::cout << "Fail" << std::endl;
        std::cout << "Invalid post" << std::endl;
        return;
    }
    hashedString userID = hash(username);
    ToggleLikePost(userID,postID);
    std::cout << "Success" << std::endl;
}

void get_user(COMMAND_ARGS) {
    loadUsers();
    if(argsCount != 1) {
        return;
    }
    std::string username = argv[0];
    hashedString userID = hash(username);
    if(users.find(userID) == users.end()) {
        return;
    }
    std::cout << serializeUser(users[userID]);
}