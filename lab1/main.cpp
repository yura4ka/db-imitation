#include "Restaurant.h"
#include "Dish.h"
#include <iostream>

using std::cin;
using std::cout;

int main() {
	Restaurant restaurant;
	Dish dish;
	int action, id, rid;

	do {
		cout << "Choose action:\n"
			"(1) Add restaurant\n"
			"(2) Get restaurant by id\n"
			"(3) Update restaurant\n"
			"(4) Delete restaurant\n"
			"(5) Add dish\n"
			"(6) Get dish by id\n"
			"(7) Update dish\n"
			"(8) Delete dish\n"
			"---------------\n"
			"(9) Display all restaurants\n"
			"(10) Display all dishes\n"
			"(11) Fill database\n"
			"(0) Exit\n";
		cin >> action;

		switch (action)
		{
		case 0:
			return 0;
		case 1:
			restaurant.input();
			restaurant.insert_in_db();
			cout << "\nSuccessfully added! id: " << restaurant.id << "\n\n";
			break;
		case 2:
			id = prompt_restaurant_id();
			if (id == -1) break;
			restaurant = Restaurant::get_by_id(id);
			cout << "\nId: " << restaurant.id 
				<< "\nName: " << restaurant.name 
				<< "\nDescription: " << restaurant.description 
				<< "\nDish count: " << restaurant.dish_count <<"\n\n";
			break;
		case 3:
			id = prompt_restaurant_id();
			if (id == -1) break;
			restaurant = Restaurant::get_by_id(id);
			restaurant.input(true);
			restaurant.update_db();
			cout << "\nSuccessfully updated!\n\n";
			break;
		case 4:
			id = prompt_restaurant_id();
			if (id == -1) break;
			restaurant = Restaurant::get_by_id(id);
			restaurant.remove();
			cout << "\nDone\n\n";
			break;
		case 5:
			dish.input();
			dish.insert_in_db();
			cout << "\nSuccessfully added! id: " << dish.id << "\n\n";
			break;
		case 6: 
			rid = prompt_restaurant_id();
			if (rid == -1) break;
			id = prompt_dish_id();
			if (id == -1) break;
			dish = Dish::get_by_id(rid, id);
			if (dish.id == -1)
				cout << "\nDish with id " << id << " doesn't exist!\n\n";
			else
				cout << "\nName: " << dish.name
				<< "\nPrice: " << dish.price << "\n\n";
			break;
		case 7: 
			rid = prompt_restaurant_id();
			if (rid == -1) break;
			id = prompt_dish_id();
			if (id == -1) break;
			dish = Dish::get_by_id(rid, id);
			if (dish.id == -1) {
				cout << "\nDish with id " << id << " doesn't exist!\n\n";
				break;
			}
			dish.input(true);
			dish.update_db();
			cout << "\nSuccessfully updated!\n\n";
			break;
		case 8:
			rid = prompt_restaurant_id();
			if (rid == -1) break;
			id = prompt_dish_id();
			if (id == -1) break;
			dish = Dish::get_by_id(rid, id);
			if (dish.id == -1) {
				cout << "\nDish with id " << id << " doesn't exist!\n\n";
				break;
			}
			dish.remove();
			cout << "\nSuccessfully deleted\n\n";
			break;
		case 9:
			Restaurant::dispay_all();
			break;
		case 10:
			Dish::display_all();
			break;
		case 11:
			fill_db();
			cout << "\nDone\n\n";
			break;
		default:
			break;
		}
	} while (action != 0);

	return 0;
}