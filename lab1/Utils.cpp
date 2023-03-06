#include "Utils.h"

void save_fopen(FILE** f, const char* path, const char* mode) {
	if (fopen_s(f, path, mode) != 0)
		throw "error";
}

void fopen_db(FILE** f, const char* mode) { save_fopen(f, RESTAURANTS_PATH, mode); }
void fopen_index(FILE** f, const char* mode) { save_fopen(f, INDEX_PATH, mode); }
void fopen_dish(FILE** f, const char* mode) { save_fopen(f, DISH_PATH, mode); }

string input_field(const char* field, const bool& is_update, const char* default_value) {
	string prompt = is_update ? " (Blank line to skip): " : ": ";
	string input;

	do {
		cout << "Enter " << field << prompt;
		std::getline(cin, input);
		if (!input.empty())
			return input.substr(0, MAX_SIZE);
		if (!is_update) cout << field << " can't be empty!\n";
	} while (!is_update);
	return string(default_value);
}

int input_number(const char* field, const bool& is_update, const int& default_value) {
	string prompt = is_update ? " ('-' to skip): " : ": ";
	int input;
	do {
		cout << "Enter " << field << prompt;
		if (cin >> input && input >= 0)
			return input;
		if (input >= 0) {
			cin.clear();
			string discard;
			getline(cin, discard);
		}
		if (!is_update || input < 0) cout << "Wrong number!\n";
	} while (!is_update);

	return default_value;
}

string format_field(const char* field, const int& max_width) {
	string result = string(field);
	result = result.size() > max_width ? result.substr(0, max_width - 3) + "..." : result;
	return result;
}

int get_address_by_id(const int& id) {
	FILE* index_table;
	Index index;

	fopen_index(&index_table, "rb");
	fseek(index_table, id * INDEX_SIZE, SEEK_SET);
	fread_one(index_table, &index);

	fclose(index_table);
	return index.address;
}

int get_max_restaurant_id() {
	FILE* index_table;
	Index index;
	fopen_index(&index_table, "rb");
	fseek(index_table, 0, SEEK_END);
	if (ftell(index_table) == 0)
		return -1;

	fseek(index_table, -(int)INDEX_SIZE, SEEK_END);
	fread_one(index_table, &index);
	fclose(index_table);
	return index.id;
}

int prompt_restaurant_id() {
	int max_id = get_max_restaurant_id();
	if (max_id == -1) {
		cout << "\nThere are no restaurants in the database!\n\n";
		return -1;
	}

	FILE* index_table;
	Index index;
	fopen_index(&index_table, "rb");

	int id;
	bool is_correct = false;
	while (!is_correct) {
		cout << "Enter restaurant's id: ";
		if (!(cin >> id) || id < 0) continue;
		if (id > max_id) {
			cout << "Restaurant with such id doesn't exists!\n";
			continue;
		}

		fseek(index_table, id * INDEX_SIZE, SEEK_SET);
		fread_one(index_table, &index);
		if (!index.exist)
			cout << "Restaurant with such id doesn't exists!\n";
		else
			is_correct = true;
	}
	fclose(index_table);
	return id;
}

int get_max_dish_id() {
	FILE* dish_db;
	Dish dish;
	fopen_dish(&dish_db, "rb");
	fseek(dish_db, 0, SEEK_END);
	if (ftell(dish_db) == 0)
		return -1;

	fseek(dish_db, -(int)sizeof(Dish), SEEK_END);
	fread_one(dish_db, &dish);
	fclose(dish_db);
	return dish.id;
}

int prompt_dish_id() {
	int max_id = get_max_dish_id();
	if (max_id == -1) {
		cout << "\nThere are no dishes in the database!\n\n";
		return -1;
	}

	FILE* dish_db;
	Dish dish;
	fopen_dish(&dish_db, "rb");

	int id;
	bool is_correct = false;
	while (!is_correct) {
		cout << "Enter dish's id: ";
		if (!(cin >> id) || id < 0) continue;
		if (id > max_id) {
			cout << "Dish with such id doesn't exists!\n";
			continue;
		}

		fseek(dish_db, id * sizeof(Dish), SEEK_SET);
		fread_one(dish_db, &dish);
		if (!dish.exists)
			cout << "Dish with such id doesn't exists!\n";
		else
			is_correct = true;
	}
	fclose(dish_db);
	return id;
}

void fill_db() {
	Restaurant r1 = { 0, "r1", "r1 desc" },
		r2 = { 1, "r2", "r2 desc" },
		r3 = { 2, "r3", "r3 desc" };
	r1.insert_in_db();
	r2.insert_in_db();
	r3.insert_in_db();

	Dish d1 = { 0, 0, "r1 d11", 11 },
		d2 = { 1, 1, "r2 d1", 21 },
		d3 = { 2, 2, "r3 d1", 31 },
		d4 = { 3, 0, "r1 d2", 12 },
		d5 = { 4, 1, "r2 d2", 22 },
		d6 = { 5, 2, "r3 d2", 32 },
		d7 = { 6, 0, "r1 d3", 13 },
		d8 = { 7, 1, "r2 d3", 23 },
		d9 = { 8, 2, "r3 d3", 33 };
	d1.insert_in_db();
	d2.insert_in_db();
	d3.insert_in_db();
	d4.insert_in_db();
	d5.insert_in_db();
	d6.insert_in_db();
	d7.insert_in_db();
	d8.insert_in_db();
	d9.insert_in_db();
}