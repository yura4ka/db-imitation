#include "Garbage.h"
#include "Utils.h"

const char* DISH_GARBAGE = "dish_garbage.bin";
const char* RESTAURANT_GARBAGE = "restaurant_garbage.bin";

void add_to_garbage(const int& address, const char* path) {
	FILE* garbage;
	save_fopen(&garbage, path, "r+b");

	int count = 1;

	fseek(garbage, 0, SEEK_END);
	bool is_empty = ftell(garbage) == 0;
	if (!is_empty) {
		fseek(garbage, 0, SEEK_SET);
		fread_one(garbage, &count);
		count++;
		fseek(garbage, 0, SEEK_SET);
		fwrite_one(garbage, &count);
	}
	fclose(garbage);
	save_fopen(&garbage, path, "ab");

	if (is_empty) fwrite_one(garbage, &count);
	fwrite_one(garbage, &address);

	fclose(garbage);
}

int get_first_deleted(const char* path) {
	FILE* garbage;
	save_fopen(&garbage, path, "rb");

	int count = -1;
	fread_one(garbage, &count);
	if (count <= 0) {
		fclose(garbage);
		return -1;
	}

	int address;
	fread_one(garbage, &address);
	fclose(garbage);
	return address;
}

void delete_from_garbage(const int& address, const char* path) {
	FILE* garbage;
	save_fopen(&garbage, path, "rb");
	fseek(garbage, 0, SEEK_END);
	int size = ftell(garbage);
	fseek(garbage, 0, SEEK_SET);
	int temp = -1, count = -1;
	fread_one(garbage, &count);
	if (count <= 0 || size == 0) throw "error";

	int n = count - 1;
	int* addresses = new int[n];

	for (int i = 0; i < n && ftell(garbage) != size; ) {
		fread_one(garbage, &temp);
		if (temp != address)
			addresses[i++] = temp;
	}

	fclose(garbage);
	save_fopen(&garbage, path, "wb");
	fwrite_one(garbage, &n);
	fwrite(addresses, sizeof(int), n, garbage);
	fclose(garbage);
	delete[] addresses;
}

void add_to_dish_garbage(const int& address) {
	add_to_garbage(address, DISH_GARBAGE);
}

int get_first_deleted_dish() {
	return get_first_deleted(DISH_GARBAGE);
}

void delete_from_dish_garbage(const int& address) {
	delete_from_garbage(address, DISH_GARBAGE);
}

void add_to_restaurant_garbage(const int& address) {
	add_to_garbage(address, RESTAURANT_GARBAGE);
}

int get_first_deleted_restaurant() {
	return get_first_deleted(RESTAURANT_GARBAGE);
}

void delete_from_restaurant_garbage(const int& address) {
	delete_from_garbage(address, RESTAURANT_GARBAGE);
}