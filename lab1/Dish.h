#pragma once

#include "Utils.h"
#include "Restaurant.h"

struct Dish {
	int id = 0;
	int restaurant_id = -1;
	char name[MAX_SIZE];
	int price;
	int next = -1;
	bool exists = true;

	int input(const bool& is_update = false);
	void insert_in_db();
	bool update_db();
	void remove();

	static Dish get_by_id(const int& restaurant_id, const int& id);
	static void display_all();
};

