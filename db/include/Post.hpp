#pragma once
#include <string>
#include <User.hpp>

struct Post
{
    unsigned long long id;
    unsigned long long authorID;
    unsigned long long timeSinceEpoc;
    int likeCount;
    std::string content;
};

