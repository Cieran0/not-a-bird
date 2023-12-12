#include <db.hpp>

hashedString parseHashedString(std::string str) {
    hashedString value;
    std::istringstream iss(str);
    iss >> value;
    return value;
}

hashedString hash(std::string stringToHash) {
    long hashValue = 1;
    for (char c : stringToHash) {
        double cFloat = c;
        hashValue *= (c) + stringToHash[(int)((cFloat/255) * (double)stringToHash.length())];
    }
    return hashValue;
}

hashedString hashPost(std::string content, hashedString authorID) {
    return hash(content + std::to_string(authorID));
} 

bool validUser(std::string username, std::string password){
    hashedString id = hash(username);
    if(users.find(id) == users.end()) {
        return false;
    }
    if(users[id].passwordHashed != hash(password)) {
        return false;
    }
    return true;
}

bool endsWith(const std::string& fullString, const std::string& ending) {
    if (fullString.length() >= ending.length()) {
        return (fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}