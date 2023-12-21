#pragma once
#include <string>
#include <vector>
#include <list>

struct User
{
    unsigned long long id;
    std::string username;
    unsigned long long passwordHashed;
    std::string pfpLink;

    unsigned long long likedPostsNum;
    std::list<unsigned long long> likedPosts;
    unsigned long long followingNum;
    std::list<unsigned long long> followingIDs;
    unsigned long long followerNum;
    std::list<unsigned long long> followerIDs;
};
