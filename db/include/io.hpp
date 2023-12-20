#pragma once
#include <db.hpp>

User readUserFromFile(const std::string& filename);
Post readPostFromFile(const std::string& filename);
std::vector<std::string> getAllFiles(const std::string& directoryPath);
void loadUsers();
void loadPosts();
void addUser(std::string username, std::string password);
void addPost(hashedString authorID, std::string content, hashedString timeStamp);
void deleteUser(hashedString id);
void deletePost(hashedString id);
std::string serializePost(Post post);
std::string serializeUser(User user);
int saveUser(User user);
int savePost(Post post);