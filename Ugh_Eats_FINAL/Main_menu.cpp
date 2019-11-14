#include "Main_menu.h"
#include "Base.h"
#include "utils.h"
#include "Exceptions.h"

void main_menu_client(Client* client, Base * base) { //j� volta atr�s e j� fecha
	while (true) {
		string input;
		utils::clear_screen();

		cout << "Welcome to Ugh Eats, " << client->get_name() << endl;
		bool retry = true;
		do {
			try
			{
				cout << "1. Make order" << endl;
				cout << "2. My info" << endl;
				cout << "3. Edit Info" << endl;
				cout << "0. Go Back" << endl;
				cout << "3. Close Program" << endl;
				cout << ">> ";

				getline(cin, input);
				retry = false;
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(3, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o << endl;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s << endl;
				cout << "Try Again!" << endl << endl;
			}
		} while (retry);


		if (input == "1") { 
			utils::clear_screen();
			client->make_order(base); 
		}
		if (input == "2") { 
			utils::clear_screen(); 
			client->print(); 
			
			cout << "\n>>"; 
			cin.ignore(); 
		}
		if (input == "3") client->edit(base);  //J� volta atr�s
		if (input == "0") {
			cin.clear();
			utils::clear_screen();
			return; //<------------------------- implement something to save and exit
		}
		if (input == "3")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheiros
		}

		else continue;
	}
}

void main_menu_admin_clients(Base * base) { // j� volta atr�s e j� fecha
	while (true) {
		string input;
		utils::clear_screen();
		bool retry = true;
		do {
			try {
				cout << "1. Show all clients" << endl;
				cout << "2. Search a client" << endl;
				cout << "3. Add a client" << endl;
				cout << "4. Edit a client" << endl;
				cout << "5. Remove a client" << endl;
				cout << "0. Go Back" << endl;
				cout << "6. Close Program" << endl;
				cout << ">> ";

				getline(cin, input);
				retry = false;
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(6, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o << endl;
				cout << "Try Again! " << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s << endl;
				cout << "Try Again!" << endl << endl;
			}

		} while (retry);
		

		if (input == "1") {
			utils::clear_screen();
			base->seeAllClients(); //J� volta atr�s

			cout << "\n>> ";
			cin.ignore();
		}

		if (input == "2") {
			utils::clear_screen();
			base->seeOneClient(); //J� volta atr�s
		}

		if (input == "3") {
			utils::clear_screen();
			base->addClient(); //J� volta atr�s

		}

		if (input == "4") {
			utils::clear_screen();
			base->changeClient(); //J� volta atr�s

		}

		if (input == "5") {
			utils::clear_screen();
			base->removeClient(); //J� volta atr�s
		}

		if (input == "0") {
			cin.clear();
			utils::clear_screen();
			break;
		}
		if (input == "6")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheirosS
		}
	}
}

void main_menu_admin_workers(Base * base) { // j� volta atr�s e j� fecha
	while (true) {
		string input;
		utils::clear_screen();

		bool retry = true;
		do
		{
			try {
				cout << "1. Show all workers" << endl;
				cout << "2. Search a worker" << endl;
				cout << "3. Add a worker" << endl;
				cout << "4. Edit a worker" << endl;
				cout << "5. Remove a worker" << endl;
				cout << "0. Go Back" << endl;
				cout << "6. Close Program" << endl;
				cout << ">> ";

				getline(cin, input);
				retry = false;
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(6, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}
		} while (retry);
		

		if (input == "1") {
			utils::clear_screen();
			base->seeAllWorkers(); //J� volta para tr�s
			
			cout << "\n>> ";
			cin.ignore();
		}

		if (input == "2") {
			utils::clear_screen();
			base->seeOneWorker(); //J� volta para tr�s
		}

		if (input == "3") {
			utils::clear_screen();
			base->addWorker(); //J� volta para tr�s
		}

		if (input == "4") {
			utils::clear_screen();
			base->changeWorker(); //J� volta para tr�s
		}

		if (input == "5") {
			utils::clear_screen();
			base->removeWorker(); //J� volta para tr�s
		}

		if (input == "0") {
			cin.clear();
			utils::clear_screen();
			break;
		}
		if (input == "6")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheiros
		}

		else continue;
	}
}

void main_menu_admin_restaurant(Base * base) { // J� volta atr�s e j� fecha
	while (true) {
		string input;
		utils::clear_screen();

		bool retry = true;
		do
		{
			try
			{
				cout << "1. Show all restaurant" << endl;
				cout << "2. Search a restaurant" << endl;
				cout << "3. Add a restaurant" << endl;
				cout << "4. Edit a restaurant" << endl;
				cout << "5. Remove a restaurant" << endl;
				cout << "0. Go Back" << endl;
				cout << "6. Close Program" << endl;
				cout << ">> ";

				retry = false;
				getline(cin, input);
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(6, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

		} while (retry);
		

		if (input == "1") {
			utils::clear_screen();
			base->seeAllRestaurants(); //J� volta para tr�s

			cout << "\n>> ";
			cin.ignore();
		}

		if (input == "2") {
			utils::clear_screen();
			base->seeOneRestaurant(); //J� volta para tr�s
		}

		if (input == "3") {
			utils::clear_screen();
			base->addRestaurant(); //J� volta para tr�s
		}

		if (input == "4") {
			utils::clear_screen();
			base->changeRestaurant(); //J� volta para tr�s
		}

		if (input == "5") {
			utils::clear_screen();
			base->removeRestaurant(); //J� volta para tr�s
		}

		if (input == "0") {
			cin.clear();
			utils::clear_screen();
			break;
		}
		if (input == "6")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheiros
		}
		else continue;
	}
}

void main_menu_admin_orders(Base * base) // J� volta atr�s e j� fecha
{
	while (true) {
		string input;
		utils::clear_screen();

		bool retry = true;
		do
		{
			try
			{
				cout << "1. Show all orders" << endl;
				cout << "2. Search an order" << endl;
				cout << "0. Go Back" << endl;
				cout << "3. Close Program" << endl;
				cout << ">> ";

				retry = false;
				getline(cin, input);
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(3, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again" << endl << endl;
			}

		} while (retry);
		

		if (input == "1") {
			utils::clear_screen();
			base->seeAllOrders(); //J� volta atr�s
		}

		if (input == "2") {
			utils::clear_screen();
			base->seeOneOrder(); //J� volta atr�s
		}
		
		if (input == "0") {
			cin.clear();
			utils::clear_screen();
			break;
		}
		if (input == "3")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheiros
		}

		else continue;
	}
}

void main_menu_admin_profits(Base * base) { // J� volta atr�s e fecha
	while (true) {
		string input;
		utils::clear_screen();

		bool retry = true;
		do
		{
			try
			{
				cout << "1. From this base" << endl;
				cout << "2. Per restaurant" << endl;
				cout << "3. Per client" << endl;
				cout << "4. Per time period" << endl;
				cout << "0. Go Back" << endl;
				cout << "5. Close Program" << endl;
				cout << ">> ";

				retry = false;
				getline(cin, input);
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(5, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch(InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}
		} while (retry);


		if (input == "1") {
			utils::clear_screen();
			base->seeProfits(); //J� volta atr�s
		}
		if (input == "2") {
			utils::clear_screen();
			base->seeProfitsPerRestaurant(); //J� volta atr�s
		}
		if (input == "3") {
			utils::clear_screen();
			base->seeProfitsPerClient(); //J� volta atr�s
		}
		if (input == "4") {
			utils::clear_screen();
			base->seeProfitsPerTime(); //-> implementar as cenas qd a fun��o tiver pronta
		}

		if (input == "0") {
			cin.clear();
			utils::clear_screen();
			break;
		}
		if (input == "5")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheiros
		}
		
		else continue;

	}
}

void main_menu_admin(Base * base) { // j� volta atr�s e j� fecha
	while (true) {
		string input;
		utils::clear_screen();

		bool retry = true;
		do
		{
			try
			{
				cout << "1. Clients" << endl;
				cout << "2. Workers" << endl;
				cout << "3. Restaurants" << endl;
				cout << "4. Orders" << endl;
				cout << "5. Profits" << endl;
				cout << "0. Go Back" << endl;
				cout << "6. Close Program" << endl;
				cout << ">> ";

				retry = false;
				getline(cin, input);
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(6, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}
		} while (retry);


		if (input == "1") main_menu_admin_clients(base);
		if (input == "2") main_menu_admin_workers(base);
		if (input == "3") main_menu_admin_restaurant(base);
		if (input == "4") main_menu_admin_orders(base);
		if (input == "5") main_menu_admin_profits(base);

		if (input == "0") { // volta atr�s
			cin.clear();
			utils::clear_screen();
			return; //<-------------- Same shit
		}
		if (input == "6")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheiros
		}
		else continue;
	}
}

void main_menu_client_login(Base * base){ // j� volta atr�s e j� fecha
	while (true) {
		string input;
		utils::clear_screen();

		bool retry = true;
		do
		{
			try
			{
				cout << "1. Login" << endl;
				cout << "2. Sign up" << endl;
				cout << "0. Go Back" << endl;
				cout << "3. Close Program" << endl;
				cout << ">> ";
				
				retry = false;
				getline(cin, input);
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(3, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}
		} while (retry);


		if (input == "1") {
			vector<Client*>::iterator it;
			vector<Client*> clients = base->getClients();

			cout << "\n\nClient name: ";
			getline(cin, input);
			
			bool notFound = true;
			for (it = clients.begin(); it != clients.end(); it++) {
				if ((*it)->get_name() == input)
				{
					main_menu_client(*it, base);
					notFound = false;
				}

				if ((*it)->get_name() == input) {
					main_menu_client(*it, base);
					notFound = false;
				}
			}
			
			if (notFound) {
				cout << "\nClient not found; Try again (Enter to continue)" << endl;
				cout << ">> ";
				cin.ignore();
			}

		}

		if (input == "2") {
			utils::clear_screen();
			bool teste = base->addClient();//try para apanhar execao blacklisted ; sem isto nao funciona corretamente
			if (teste)
				main_menu_client(base->getClients().at(base->getClients().size() - 1), base);
			else
			{
				utils::clear_screen();
				continue;
			}
		}

		if (input == "0") {
			cin.clear();
			utils::clear_screen();
			break;
		}
		if (input == "3")
		{
			cin.clear();
			exit(0); //aplicar fun��o que guarde nos ficheiros
		}
		
		else continue;
	}
}

void main_menu(vector<Base*> bases) { 
	while (true) {
		string input;
		string temp;
		Base * selected_base;
		utils::clear_screen();

		bool retry = true;
		do
		{
			try {
				cout << "Pick a Base:" << endl;

				for (auto n = 0; n < bases.size(); n++) {
					cout << n + 1 << ". " << bases.at(n)->getDistrict() << endl;
				}

				cout << ">> ";

				getline(cin, temp);
				if (cin.eof())
				{
					cin.clear();
					return;
				}

				retry = false;
				if (!isNumber(temp))
					throw InvalidNumberException(temp);
				if (InvalidOptions(bases.size(), stoi(temp)))
					throw InvalidOptionException(stoi(temp));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

		} while (retry);


		selected_base = bases.at(stoi(temp) - 1);

		//cin.ignore();

		cout << "\n\n";
		cout << "Client or Admin ?" << endl; // j� volta atr�s e j� fecha
		retry = true;
		do
		{
			try
			{
				cout << "1. Client" << endl;
				cout << "2. Admin" << endl;
				cout << "0. Go Back" << endl;
				cout << "3. Close Program" << endl;
				cout << ">> ";

				retry = false;
				getline(cin, input);
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (InvalidOptions(3, stoi(input)))
					throw InvalidOptionException(stoi(input));
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}
			if (input == "1")
			{
				main_menu_client_login(selected_base);
				retry = true;
				cout << endl;
			}
			if (input == "2")
			{
				main_menu_admin(selected_base);
				retry = true;
				cout << endl;
			}
			if (input == "0") {
				cin.clear();
				utils::clear_screen();
				continue;
			}
			if (input == "3")
			{
				cin.clear();
				exit(0); //aplicar fun��o que guarde nos ficheiros
			}
		} while (retry);
	}
}