#pragma once
#include <iostream>
#include <User.hpp>
#include <Post.hpp>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <filesystem>
typedef unsigned long long hashedString;

extern std::unordered_map<hashedString,User> users;
extern std::unordered_map<hashedString,Post> posts;

hashedString parseHashedString(std::string str);
hashedString hash(std::string stringToHash);
hashedString hashPost(std::string content, hashedString authorID);
bool validUser(std::string username, std::string password);
bool endsWith(const std::string& fullString, const std::string& ending);