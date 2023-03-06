#include "Dish.h"
#include "Restaurant.h"
#include "Garbage.h"
#include <string>

const size_t DISH_SIZE = sizeof(Dish);

int Dish::input(const bool& is_update) {
	if (!is_update) {
		int max_id = get_max_restaurant_id();
		if (max_id == -1) {
			cout << "\nThere are no restaurants in the database!\n\n";
			return -1;
		}

		bool is_correct = false;
		while (!is_correct) {
			restaurant_id = input_number("restaurant's id", is_update, restaurant_id);
			if (restaurant_id > max_id || restaurant_id < 0) {
				cout << "Restaurant with such id doesn't exists!\n";
				continue;
			}
			is_correct = true;
		}
	}

	cin.ignore();
	strcpy_s(name, input_field("name", is_update, name).c_str());
	price = input_number("price", is_update, price);
	return 1;
}

void Dish::insert_in_db() {
	FILE* dish_db;
	Dish temp;
	fopen_dish(&dish_db, "a+b");

	auto r = Restaurant::get_by_id(restaurant_id);
	int address;

	int deleted = get_first_deleted_dish();
	if (deleted != -1) {
		address = deleted;
		fseek(dish_db, address, SEEK_SET);
		fread_one(dish_db, &temp);
		id = temp.id;
		fclose(dish_db);
		fopen_dish(&dish_db, "r+b");
		fseek(dish_db, address, SEEK_SET);
		delete_from_dish_garbage(address);
	}
	else {
		fseek(dish_db, 0, SEEK_END);
		if (ftell(dish_db) != 0) {
			fseek(dish_db, -(int)DISH_SIZE, SEEK_END);
			fread_one(dish_db, &temp);
			id = temp.id + 1;
		}
		address = ftell(dish_db);
	}

	next = r.first_dish_address;
	exists = true;
	fwrite_one(dish_db, this);
	r.first_dish_address = address;
	r.dish_count++;
	r.update_db();
	fclose(dish_db);
}

Dish Dish::get_by_id(const int& restaurant_id, const int& id) {
	FILE* dish_db;
	auto r = Restaurant::get_by_id(restaurant_id);
	Dish dish;
	fopen_dish(&dish_db, "rb");
	fseek(dish_db, r.first_dish_address, SEEK_SET);
	for (int i = 0; i < r.dish_count; i++) {
		fread_one(dish_db, &dish);
		if (dish.id == id) {
			fclose(dish_db);
			return dish;
		}
		fseek(dish_db, dish.next, SEEK_SET);
	}
	dish = Dish();
	dish.id = -1;
	fclose(dish_db);
	return dish;
}

bool Dish::update_db() {
	FILE* dish_db;
	Dish dish;
	fopen_dish(&dish_db, "r+b");
	auto r = Restaurant::get_by_id(restaurant_id);

	int prev;
	fseek(dish_db, r.first_dish_address, SEEK_SET);
	for (int i = 0; i < r.dish_count; i++) {
		prev = ftell(dish_db);
		fread_one(dish_db, &dish);
		if (dish.id == id) {
			fseek(dish_db, prev, SEEK_SET);
			fwrite_one(dish_db, this);
			fclose(dish_db);
			return true;
		}
		fseek(dish_db, dish.next, SEEK_SET);
	}
	fclose(dish_db);
	return false;
}

void Dish::remove() {
	exists = false;
	update_db();

	auto r = Restaurant::get_by_id(restaurant_id);
	int address = r.first_dish_address;

	FILE* dish_db;
	Dish temp, prev;
	fopen_dish(&dish_db, "r+b");

	fseek(dish_db, r.first_dish_address, SEEK_SET);
	fread_one(dish_db, &prev);

	if (prev.id != id) {
		for (int i = 1; i < r.dish_count; i++) {
			fseek(dish_db, prev.next, SEEK_SET);
			fread_one(dish_db, &temp);
			if (temp.id == id)
				break;
			prev = temp;
		}
		fclose(dish_db);
		address = prev.next;
		prev.next = next;
		prev.update_db();
	}
	else {
		r.first_dish_address = next;
		fclose(dish_db);
	}

	r.dish_count--;
	r.update_db();
	add_to_dish_garbage(address);
}

void Dish::display_all() {
	FILE* dish_db;
	Dish temp;
	fopen_dish(&dish_db, "rb");

	fseek(dish_db, 0, SEEK_END);
	int size = ftell(dish_db);
	fseek(dish_db, 0, SEEK_SET);

	const int num_width = 5;
	const int name_width = 13;

	cout << "\n\n";
	format_output("ID", num_width);
	format_output("RESTAURANT_ID", name_width);
	format_output("NAME", name_width);
	format_output("PRICE", num_width, true);
	cout << "\n------+---------------+---------------+-------\n";
	while (ftell(dish_db) != size) {
		fread_one(dish_db, &temp);
		if (!temp.exists) continue;
		format_output(temp.id, num_width);
		format_output(temp.restaurant_id, name_width);
		format_output(format_field(temp.name, name_width), name_width);
		format_output(temp.price, num_width, true);
		cout << "\n";
	}
	cout << "\n";
	fclose(dish_db);
}
