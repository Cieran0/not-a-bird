#pragma once
#include <string>
#include <vector>
#include <list>

struct User
{
    unsigned long long id;
    std::string username;
    unsigned long long passwordHashed;
    std::list<unsigned long long> likedPosts;
};
