#include <db.hpp>

User readUserFromFile(const std::string& filename) {
    User user;

    std::ifstream file("Users/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        // Handle the error, maybe return a default user or throw an exception
        return user;
    }

    // Reading id, username, and passwordHashed
    if (!(file >> user.id >> user.username >> user.passwordHashed)) {
        std::cerr << "Error reading user data from file." << std::endl;
        // Handle the error, maybe return a default user or throw an exception
        file.close();
        return user;
    }

    // Read likedPosts if available
    unsigned long long likedPost;
    while (file >> likedPost) {
        user.likedPosts.push_back(likedPost);
    }

    file.close();
    return user;
}

Post readPostFromFile(const std::string& filename) {
    Post post;

    std::ifstream file("Posts/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        file.close();
        return post;
    }

    if (!(file >> post.id >> post.authorID >> post.timeSinceEpoc >> post.likeCount)) {
        std::cerr << "Error reading user data from file." << std::endl;
        // Handle the error, maybe return a default user or throw an exception
        return post;
    }

    // Read content line by line
    std::string line;
    std::getline(file, line); // Consume the newline after reading likeCount

    while (std::getline(file, line)) {
        post.content += line + "\n";
    }

    // Remove the extra newline at the end
    if (!post.content.empty()) {
        post.content.pop_back();
    }

    file.close();
    return post;
}

std::vector<std::string> getAllFiles(const std::string& directoryPath) {
    std::vector<std::string> files;

    if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry.path())) {
                files.push_back(entry.path().filename().string());
            }
        }
    } else {
        std::cerr << "Directory does not exist or is not accessible." << std::endl;
    }
    return files;
}

void loadUsers() {
    std::vector<std::string> userFiles = getAllFiles("Users");
    for (std::string file : userFiles)
    {
        User u = readUserFromFile(file);
        users[u.id] = u;
    }
}

void loadPosts() {
    std::vector<std::string> postFiles = getAllFiles("Posts");
    for (std::string file : postFiles)
    {
        Post p = readPostFromFile(file);
        posts[p.id] = p;
    }
}

void addUser(std::string username, std::string password) {
    hashedString id = hash(username);
    hashedString passwordHashed = hash(password);
    User newUser = {
        .id = id,
        .username = username,
        .passwordHashed= passwordHashed,
    };
    users[id] = newUser;
}

void addPost(hashedString authorID, std::string content, hashedString timeStamp) {
    hashedString id = hashPost(content,authorID,timeStamp);
    Post newPost = {
        .id = id,
        .authorID = authorID,
        .timeSinceEpoc = timeStamp,
        .likeCount = 0,
        .content= content,
    };
    posts[id] = newPost;
}

void deleteUser(hashedString id) {
    users.erase(id);
    std::string path = "Users/" + std::to_string(id);
    std::remove(path.c_str());
}

void deletePost(hashedString id) {
    posts.erase(id);
    std::string path = "Posts/" + std::to_string(id);
    std::remove(path.c_str());
}

std::string serializePost(Post post) {
    std::string serializedString = std::to_string(post.id) + "\n" 
    + std::to_string(post.authorID) + "\n" 
    + std::to_string(post.timeSinceEpoc) + "\n" 
    + std::to_string(post.likeCount) + "\n" 
    + post.content;
    return serializedString;
}

std::string serializeUser(User user) {
    std::string serializedString = std::to_string(user.id) + "\n" 
    + user.username + "\n" 
    + std::to_string(user.passwordHashed);
    for (hashedString &likedPost : user.likedPosts)
    {
        serializedString += "\n" + std::to_string(likedPost);
    }
    return serializedString;
}

int saveUser(User user) {
    std::string filename = "Users/" + std::to_string(user.id);
    std::ofstream outputFile(filename);

    if(outputFile.is_open()) {
        outputFile << serializeUser(user);
        outputFile.close();
    } else {
        std::cerr << "Error writing to file " << filename << std::endl;
        return 1;
    }
    return 0;
}

int savePost(Post post) {
    std::string filename = "Posts/" + std::to_string(post.id);
    std::ofstream outputFile(filename);

    if(outputFile.is_open()) {
        outputFile << serializePost(post);
        outputFile.close();
    } else {
        std::cerr << "Error writing to file " << filename << std::endl;
        return 1;
    }
    return 0;
}