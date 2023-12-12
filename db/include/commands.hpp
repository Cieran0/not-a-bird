#pragma once
#include <db.hpp>

void login(int argsCount, char const *argv[]);
void add_user(int argsCount, char const *argv[]);
void add_post(int argsCount, char const *argv[]);
void delete_user(int argsCount, char const *argv[]);
void delete_post(int argsCount, char const *argv[]);
void get_post_content(int argsCount, char const *argv[]);
void get_all_posts(int argsCount, char const *argv[]);