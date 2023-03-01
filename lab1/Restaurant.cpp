#include <iostream>
#include <string>
#include "Restaurant.h"
#include "Index.h"
#include "Utils.h"
#include "Dish.h"
#include "Garbage.h"

using std::cout;
using std::cin;
using std::string;

void Restaurant::input(bool is_update) {
	cin.ignore();
	strcpy_s(this->name, input_field("name", is_update, this->name).c_str());
	strcpy_s(this->description, input_field("description", is_update, this->description).c_str());
}

void Restaurant::insert_in_db() {
	FILE* index_table, * db;
	Index index;

	fopen_index(&index_table, "a+b");
	fopen_db(&db, "a+b");

	int deleted = get_first_deleted_restaurant();
	if (deleted != -1) {
		fseek(index_table, deleted, SEEK_SET);
		fread_one(index_table, &index);
		id = index.id;
		fclose(db);
		fclose(index_table);
		fopen_index(&index_table, "r+b");
		fopen_db(&db, "r+b");
		fseek(index_table, deleted, SEEK_SET);
		fseek(db, index.address, SEEK_SET);
		delete_from_restaurant_garbage(deleted);
	}
	else {
		fseek(index_table, 0, SEEK_END);
		if (ftell(index_table) != 0) {
			fseek(index_table, -(int)INDEX_SIZE, SEEK_END);
			fread_one(index_table, &index);
			id = index.id + 1;
		}
		index.id = id;
		index.address = id * RESTAURANT_SIZE;
	}

	fwrite_one(db, this);

	index.exist = true;
	fwrite_one(index_table, &index);

	fclose(index_table);
	fclose(db);
}

void Restaurant::update_db() {
	FILE* db;
	fopen_db(&db, "r+b");
	fseek(db, get_address_by_id(id), SEEK_SET);
	fwrite_one(db, this);
	fclose(db);
}

Restaurant Restaurant::get_by_id(const int& id) {
	FILE* db;
	Restaurant r;

	fopen_db(&db, "rb");
	fseek(db, get_address_by_id(id), SEEK_SET);
	fread_one(db, &r);

	fclose(db);
	return r;
}

void Restaurant::dispay_all() {
	FILE* db, * index_table;
	Restaurant temp;
	Index index;
	fopen_db(&db, "rb");
	fopen_index(&index_table, "rb");

	fseek(index_table, 0, SEEK_END);
	int size = ftell(index_table);
	fseek(index_table, 0, SEEK_SET);

	const int num_width = 5;
	const int name_width = 13;

	cout << "\n\n";
	format_output("ID", num_width);
	format_output("NAME", name_width);
	format_output("DESCRIPTION", name_width);
	format_output("DISHES", num_width, true);
	cout << "\n------+---------------+---------------+-------\n";
	while (ftell(index_table) != size) {
		fread_one(index_table, &index);
		if (!index.exist) continue;
		fseek(db, index.address, SEEK_SET);
		fread_one(db, &temp);
		format_output(temp.id, num_width);
		format_output(format_field(temp.name, name_width), name_width);
		format_output(format_field(temp.description, name_width), name_width);
		format_output(temp.dish_count, num_width, true);
		cout << "\n";
	}
	cout << "\n";
	fclose(db);
	fclose(index_table);
}

void Restaurant::remove() {
	if (dish_count != 0) {
		Dish temp;
		FILE* dish_db;
		fopen_dish(&dish_db, "r+b");
		fseek(dish_db, first_dish_address, SEEK_SET);
		for (int i = 0; i < dish_count; i++) {
			fread_one(dish_db, &temp);
			fclose(dish_db);
			temp.remove();
			fopen_dish(&dish_db, "r+b");
			fseek(dish_db, temp.next, SEEK_SET);
		}
		first_dish_address = -1;
		dish_count = 0;
		fclose(dish_db);
	}

	FILE* index_table;
	Index index;

	fopen_index(&index_table, "r+b");
	fseek(index_table, id * INDEX_SIZE, SEEK_SET);
	fread_one(index_table, &index);
	index.exist = false;
	fseek(index_table, id * INDEX_SIZE, SEEK_SET);
	fwrite_one(index_table, &index);
	fclose(index_table);

	add_to_restaurant_garbage(id * INDEX_SIZE);
}