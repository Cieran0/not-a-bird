#pragma once
#include <string>
#include <vector>

struct User
{
    unsigned long long id;
    std::string username;
    unsigned long long passwordHashed;
    std::vector<unsigned long long> likedPosts;
};
