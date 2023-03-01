#pragma once

#define MAX_SIZE 100

struct Restaurant {
	int id = 0;
	char name[MAX_SIZE];
	char description[MAX_SIZE];
	int dish_count = 0;
	int first_dish_address = -1;

	void input(bool isUpdate = false);
	void insert_in_db();
	void update_db();
	void remove();

	static Restaurant get_by_id(const int& id);
	static void dispay_all();
};


