#pragma once
#include <db.hpp>

#define COMMAND_ARGS const int &argsCount, char const** &argv

void login(COMMAND_ARGS);
void add_user(COMMAND_ARGS);
void add_post(COMMAND_ARGS);
void delete_user(COMMAND_ARGS);
void delete_post(COMMAND_ARGS);
void get_post(COMMAND_ARGS);
void get_all_posts(COMMAND_ARGS);
void get_username(COMMAND_ARGS);
void toggle_like_post(COMMAND_ARGS);
void get_user(COMMAND_ARGS);
void get_user_posts(COMMAND_ARGS);