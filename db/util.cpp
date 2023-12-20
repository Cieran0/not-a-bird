#include <db.hpp>

hashedString parseHashedString(std::string str) {
    hashedString value;
    std::istringstream iss(str);
    iss >> value;
    return value;
}

hashedString hash(std::string stringToHash) {
    const hashedString MOD = 1e9 + 7; // Choose a suitable modulo value

    hashedString hashValue = 1;
    for (char c : stringToHash) {
        double cFloat = c;
        hashedString multiplier = static_cast<hashedString>(c) + stringToHash[static_cast<int>((cFloat / 255) * stringToHash.length())];
        hashValue = (hashValue * multiplier) % MOD;
    }
    return hashValue;
}

hashedString hashPost(std::string content, hashedString authorID, hashedString timeStamp) {
    return hash(content + std::to_string(authorID) + std::to_string(timeStamp));
} 

bool validUser(std::string username, std::string password){
    hashedString id = hash(username);
    if(users.find(id) == users.end()) {
        std::cout << "Did't find user" << std::endl;
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

unsigned long long timeSinceEpoch() {
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}