#pragma once

#include "Restaurant.h"
#include "Index.h"
#include "Dish.h"
#include <iostream>
#include <string>
#include <iomanip>

using std::string, std::cout, std::cin;

template<typename T> inline void fread_one(FILE* file, T* buffer);
template<typename T> inline void fwrite_one(FILE* file, T* buffer);

void save_fopen(FILE** f, const char* path, const char* mode);
void fopen_db(FILE** f, const char* mode);
void fopen_index(FILE** f, const char* mode);
void fopen_dish(FILE** f, const char* mode);

string input_field(const char* field, const bool& is_update, const char* default_value);
int input_number(const char* field, const bool& is_update, const int& default_value);
string format_field(const char* field, const int& max_width);
template<typename T> inline void format_output(T t, const int& max_width, const bool& is_last = false);

int get_address_by_id(const int& id);
int prompt_restaurant_id();
int prompt_dish_id();
int get_max_restaurant_id();
int get_max_dish_id();
void fill_db();

static const char* INDEX_PATH = "restaurant.ind";
static const char* RESTAURANTS_PATH = "restaurant.fl";
static const char* DISH_PATH = "dish.fl";
static const size_t INDEX_SIZE = sizeof(Index);
static const size_t RESTAURANT_SIZE = sizeof(Restaurant);

template<typename T> 
inline void fread_one(FILE* file, T* buffer) { fread(buffer, sizeof(T), 1, file); }

template<typename T> 
inline void fwrite_one(FILE* file, T* buffer) { fwrite(buffer, sizeof(T), 1, file); }

template<typename T>
inline void format_output(T t, const int& max_width, const bool& is_last) {
	cout << std::left << std::setw(max_width) << std::setfill(' ') << t;
	if (!is_last) cout << " | ";
}



