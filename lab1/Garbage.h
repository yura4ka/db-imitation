#pragma once

void add_to_garbage(const int& address, const char* path);
int get_first_deleted(const char* path);
void delete_from_garbage(const int& address, const char* path);

void add_to_dish_garbage(const int& address);
int get_first_deleted_dish();
void delete_from_dish_garbage(const int& address);

void add_to_restaurant_garbage(const int& address);
int get_first_deleted_restaurant();
void delete_from_restaurant_garbage(const int& address);