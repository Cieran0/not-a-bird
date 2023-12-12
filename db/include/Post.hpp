#pragma once
#include <string>
#include <User.hpp>

struct Post
{
    unsigned long long id;
    unsigned long long authorID;
    int likeCount;
    std::string content;
};

