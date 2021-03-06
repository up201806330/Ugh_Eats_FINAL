//#pragma comment(lib, "winmm.lib")
//
//#include <Windows.h>
//#include <mmsystem.h>

#include "Base.h"
#include "utils.h"


using namespace std;

#include <set>
#include <list>
#include <iomanip>
#include <algorithm>

Base::Base()
{

}

Base::~Base()
{

}

tuple<string, vector<string> > Base::parseAreaOfInfluence(string str) {

	// ':' separates the "name" of the base to the rest 
	vector<string> parts = utils::split(str, ':');
	for (auto & part : parts) utils::trim(part);

	vector<string> towns = utils::split(parts.at(1), ';');
	for (auto & town : towns) utils::trim(town);

	return make_tuple(parts.at(0), towns);
}

vector<Base*> Base::load(string path) {

	Base::load_blacklist();

	ifstream base_text(path);

	try {
		if (!base_text.is_open())
			throw FileOpenErrorException(path);
	}
	catch (FileOpenErrorException & f)
	{
		cout << f;

		exit(0);
	}


	vector<Base*> bases;
	string textline;

	vector<string> areaOfInfluence;

	while (!base_text.eof()) {
		Base base;
		getline(base_text, textline);

		tuple<string, vector<string> > distAndAreaOfInf = Base::parseAreaOfInfluence(textline);
		base.setDistrict(get<0>(distAndAreaOfInf));
		base.setAreaOfInfluence(get<1>(distAndAreaOfInf));
		getline(base_text, textline);
		Address addr;
		addr.parse(textline);
		base.setAddress(addr);

		getline(base_text, textline);
		base.setRestaurantsFileName(textline);
		Restaurant::load(textline, &base);

		getline(base_text, textline);
		base.setDeliveriesFileName(textline);
		Order::load(textline, &base);

		getline(base_text, textline);
		base.setWorkersFileName(textline);
		Worker::load(textline, &base);

		Admin* manager = *(base.getAdmins().begin());
		base.setAdmin(manager);
		// base.getAdmins().begin());
		

		getline(base_text, textline);
		base.setClientsFileName(textline);
		Client::load(textline, &base);

		getline(base_text, textline);
		base.setTechniciansFileName(textline);
		Technician::load(textline, &base);

		getline(base_text, textline);

		Base * ptr = new Base;
		*ptr = base;
		bases.push_back(ptr);
	}
	base_text.close();

	return bases;
}

vector<string> Base::blacklist = {};

void Base::load_blacklist()
{
	ifstream stream("blacklisted.txt");

	try
	{
		if (!stream.is_open())
			throw FileOpenErrorException("blacklisted.txt");
	}
	catch (FileOpenErrorException & f)
	{
		cout << f;
		exit(0);
	}


	string input;

	while (!stream.eof()) {
		getline(stream, input);
		blacklist.push_back(input);
	}

	stream.close();
}

Restaurant * Base::findRestaurant(string str) {
	for (auto & restaurant : restaurants)
		if (restaurant->get_name() == str)
			return restaurant;
	return nullptr;
}

map<int, Order*> Base::findOrders(string str) {

	map<int, Order*> result = {};

	// in case there are no orders, the textline is "-" and therefore the map should have 0 elements
	if (str == "-") {
		return result;
	}

	vector<string> ids = utils::split(str, ':');
	// cout << "Size: " << ids.size() << endl;
	for (auto i = 0; i < ids.size(); i++) {
		int x = stoi(ids.at(i));
		// cout << "x: " << x << endl;

		map<int, Order*>::iterator it = orders.find(x);

		if (it != orders.end()) {
			// cout << "x: " << x << " second: " << (*it).second << " ." << endl;
			result.insert(pair<int, Order*>(x, (*it).second));
		}
	}
	return result;
}


void Base::setRestaurantsFileName(string str) {
	restaurantsFileName = str;
}

void Base::setDeliveriesFileName(string str) {
	deliveriesFileName = str;
}

void Base::setWorkersFileName(string str) {
	workersFileName = str;
}

void Base::setClientsFileName(string str) {
	clientsFileName = str;
}

void Base::setTechniciansFileName(string str) {
	technicianFileName = str;
}

void Base::setDistrict(string d) {
	district = d;
}

void Base::setAreaOfInfluence(vector<string> areaOfInf) {
	areaOfInfluence = areaOfInf;
}

void Base::setAddress(Address add) {
	address = add;
}

void Base::setAdmins(unordered_set<Admin*, hashAdmin, eqAdmin> admins) {
	this->admins = admins;
}

void Base::setDeliveryPeople(unordered_set<Delivery*, hashDeliv, eqDeliv> deliverers) {
	this->deliveryPeople = deliverers;
}

void Base::setAdmin(Admin * administrador) {
	admin = administrador;
}

void Base::setClients(vector<Client*> clients) {
	this->clients = clients;
}

void Base::setOrders(map<int, Order*> orders)
{
	if (orders.size() == 0) this->orders = {};
	this->orders = orders;
}

void Base::setRestaurants(vector<Restaurant*> restaurants) {
	this->restaurants = restaurants;
}

void Base::setTechnicians(priority_queue<Technician> tec) {
	technicians = tec;
}

void Base::setVehicles(BST<Vehicle> veh) {
	vehicles = veh;
}

string Base::getRestaurantsFileName() const {
	return restaurantsFileName;
}

string Base::getDeliveriesFileName() const {
	return deliveriesFileName;
}

string Base::getWorkersFileName() const {
	return workersFileName;
}

string Base::getClientsFileName() const {
	return clientsFileName;
}

string Base::getTechnicianFileName() const {
	return technicianFileName;
}

string Base::getDistrict() const {
	return district;
}

vector<string> Base::getAreaOfInfluence() const {
	return areaOfInfluence;
}

Address Base::getAddress() const {
	return address;
}

const unordered_set<Admin*, hashAdmin, eqAdmin> Base::getAdmins() const {
	return admins;
}

const unordered_set<Delivery*, hashDeliv, eqDeliv> Base::getDeliveryPeople() const {
	return deliveryPeople;
}

Admin * Base::getAdmin() const {
	return admin;
}

const vector<Client*> & Base::getClients() const {
	return clients;
}

const vector<Restaurant*> & Base::getRestaurants() const {
	return restaurants;
}

const map<int, Order*> & Base::getOrders() const {
	return orders;
}

priority_queue<Technician> Base::getTechnicians() const {
	return technicians;
}

BST<Vehicle> Base::getVehicles() const
{
	return vehicles;
}

//Visualizacao de Informacao
void Base::seeAllClients()
{
	cout << "ALL CLIENTS" << endl << endl;
	vector<Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		(*it)->print();
		cout << endl << endl;
	}
}

void Base::seeOneClient()
{
	vector<Client*>::iterator it;
	int id = 1;
	string answer;
	bool retry = true;
	do
	{
		try
		{
			utils::clear_screen();
			retry = false;
			cout << "Pick the Client you want to see: " << endl << endl;
			for (it = clients.begin(); it != clients.end(); it++)
			{
				cout << id << ". " << (*it)->get_name() << endl;
				id++;
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, answer);
			if (answer == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(answer))
				throw InvalidNumberException(answer);
			if (answer != "")
			{
				if (InvalidOptions(clients.size(), stoi(answer)))
					throw InvalidOptionException(stoi(answer));
				int input = stoi(answer);
			}
			else
			{
				id = 1;
				retry = true;
				utils::clear_screen();
			}
		}
		catch (InvalidOptionException & o)
		{
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}
		catch (InvalidNumberException & s)
		{
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}
	} while (retry);


	cout << endl << "INFO" << endl;
	clients.at(stoi(answer) - 1)->print();

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

void Base::seeAllRestaurants()
{
	cout << "ALL RESTAURANTS" << endl << endl;
	vector<Restaurant*>::iterator it;
	for (it = restaurants.begin(); it != restaurants.end(); it++)
	{
		cout << *(*it);
	}
}

void Base::seeOneRestaurant()
{

	vector<Restaurant*>::iterator it;
	int id = 1;
	string answer;
	bool retry = true;
	do
	{
		try
		{
			utils::clear_screen();
			retry = false;
			cout << "Pick the Restaurant you want to see" << endl << endl;
			for (it = restaurants.begin(); it != restaurants.end(); it++)
			{
				cout << id << ". " << (*it)->get_name() << endl;
				id++;
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";

			getline(cin, answer);
			if (answer == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(answer))
				throw InvalidNumberException(answer);
			if (answer != "")
			{
				if (InvalidOptions(restaurants.size(), stoi(answer)))
					throw InvalidOptionException(stoi(answer));
				int input = stoi(answer);
			}
			else
			{
				id = 1;
				retry = true;
				utils::clear_screen();
			}

		}
		catch (InvalidOptionException & o)
		{
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}
		catch (InvalidNumberException & s)
		{
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}

	} while (retry);



	int input = stoi(answer);
	cout << endl << "INFO" << endl;
	cout << (*restaurants.at(input - 1));
	if ((*restaurants.at(input - 1)).get_products().size() == 0)
		cout << endl;
	else
	{
		cout << "Products:" << endl;
		vector<Product*>::iterator ite;

		vector<Product*> vec = restaurants.at(input - 1)->get_products();
		for (ite = vec.begin(); ite != vec.end(); ite++)
			// for (ite = (*restaurants.at(answer - 1)).get_products().begin(); ite != (*restaurants.at(answer - 1)).get_products().end(); ite++)
		{
			cout << *(*ite) << endl;
		}

	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

int Base::seeFormerEmployees() {

	bool retry = true;
	int answer;
    string input;
	do {
		try {
			utils::clear_screen();
			retry = false;
			cout << "See Only Former Employees? " << endl;

			cout << "1. Yes" << endl;
			cout << "2. No. Only Current Employees" << endl;
			cout << "3. No. Both Former and Current." << endl;
			cout << "0. Go Back" << endl << endl;

			cout << ">> ";
			getline(cin, input);

			if (input == "0") {
				cin.clear();
				utils::clear_screen();
				return 0;
			}
			if (!isNumber(input)) throw InvalidNumberException(input);

			if (input != "") {
				if (InvalidOptions(3, stoi(input))) throw InvalidOptionException(stoi(input));
				answer = stoi(input);
			}

			else {
				retry = true;
				utils::clear_screen();
			}
		}

		catch (InvalidOptionException & o) {
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}

		catch (InvalidNumberException & s) {
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

	return answer;
}


void Base::seeAllWorkers(bool vectorData, vector<Admin*> adminVector, vector<Delivery*> delivPeopleVector) {

	// 1 -> former; 2 -> current; 3 -> both; 0 -> go back
	int seeFormerWorkers = seeFormerEmployees();

	if (seeFormerWorkers == 0) return;

	vector<int> displaySubset = {};
	int provSize = 0;
	vector<int> adminSubsetVector;
	vector<int> delivSubsetVector;

	displaySubset.clear();
	displaySubset.push_back(seeFormerWorkers);

	utils::clear_screen();
	cout << "ALL WORKERS" << endl << endl;

	cout << "Administrators" << endl << endl;
	if (vectorData) seeAllAdmins(displaySubset, 1, true, adminVector);
	else seeAllAdmins(displaySubset);

	cout << endl << endl;

	provSize = (int) displaySubset.size();

	displaySubset.clear();
	displaySubset.push_back(seeFormerWorkers);

	cout << "Delivery People" << endl << endl;
	// if (vectorData) seeAllDeliveryPeople( (int) admins.size() + 1, true, delivPeopleVector);
	// else seeAllDeliveryPeople( (int) admins.size() + 1);
	if (vectorData) seeAllDeliveryPeople(displaySubset, provSize + 1, true, delivPeopleVector);
	else seeAllDeliveryPeople(displaySubset, provSize + 1);
}

void Base::seeOneWorker() {

	bool retry = true;
	int answer;
	string input;

	// 1 -> former; 2 -> current; 3 -> both; 0 -> go back
	int seeFormerWorkers = seeFormerEmployees();
	
	if (seeFormerWorkers == 0) return;

	vector<int> displaySubset = {};
	int provSize = 0;
	vector<int> adminSubsetVector;
	vector<int> delivSubsetVector;

	do {
		
		displaySubset.clear();
		displaySubset.push_back(seeFormerWorkers);
		try {
			utils::clear_screen();
			retry = false;
			cout << "Pick the worker you want to see" << endl << endl;

			cout << "Administrators" << endl << endl;
			seeAllAdminsNames(displaySubset, 1, false, {});
			cout << endl << endl;
			cout << "Delivery People" << endl << endl;

			provSize = (int) displaySubset.size();
			adminSubsetVector = displaySubset;

			displaySubset.clear();
			displaySubset.push_back(seeFormerWorkers);

			seeAllDeliveryPeopleNames(displaySubset, provSize + 1, false, {});

			delivSubsetVector = displaySubset;

			// cout << "SIZE: " << displaySubset.size() << endl;
			/*
			cout << endl << endl;
			for (auto & x : displaySubset) cout << x << endl;
			cout << endl << endl;
			*/

			cout << endl;
			cout << "0. Go Back" << endl << endl;
			cout << ">> ";
			getline(cin, input);

			if (input == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(input)) throw InvalidNumberException(input);

			if (input != "") {
				// if (InvalidOptions(admins.size() + displaySubset.size(), stoi(input))) throw InvalidOptionException(stoi(input));
				if (InvalidOptions(provSize + displaySubset.size(), stoi(input))) throw InvalidOptionException(stoi(input));
				// answer = stoi(input);
				answer = (stoi(input) > provSize) ? (delivSubsetVector[stoi(input) - provSize - 1 ] - provSize + ( (int) admins.size() ) ) : (adminSubsetVector[stoi(input) - 1] - 1);
			}

			else {
				retry = true;
				utils::clear_screen();
			}
		}

		catch (InvalidOptionException & o) {
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}

		catch (InvalidNumberException & s) {
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

	cout << endl;

	int counter;

	// worker chosen was not an admin
	if (answer > admins.size()) {
		unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator delIt = deliveryPeople.begin();
		int offset = answer - ( (int) admins.size() );
		counter = 0;
		
		while (counter < offset - 1) {
			delIt++;
			counter++;
		}
		(*delIt)->print();
	}
	// worker chosen was an admin
	else {
		unordered_set<Admin*, hashAdmin, eqAdmin>::iterator admIt = admins.begin();
		counter = 0;

		while (counter < answer) {
			admIt++;
			counter++;
		}
		(*admIt)->print();
	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

void Base::seeAllDeliveryPeopleNames(vector<int> &displaySubset, int i, bool vectorData, vector<Delivery*> delivPeopleVector) {

	int condition = displaySubset[0];
	int realPos = i;

	displaySubset.clear();

	unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator it = deliveryPeople.begin();

	vector<Delivery*>::iterator itVector;

	if (vectorData) {
		itVector = delivPeopleVector.begin();

		// 1 -> former; 2 -> current; 3 -> both
		switch (condition) {

			case 1:
				while (itVector != delivPeopleVector.end()) {

					if ((*itVector)->get_state() == false) {
						cout << i << ". ";
						cout << (*itVector)->get_name() << " [FORMER EMPLOYEE]" << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					itVector++;
					realPos++;
					
				}
				break;

			case 2:
				while (itVector != delivPeopleVector.end()) {
					
					if ((*itVector)->get_state()) {
						cout << i << ". ";
						cout << (*itVector)->get_name() << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					itVector++;
					realPos++;

				}
				break;

			case 3:
				while (itVector != delivPeopleVector.end()) {
					cout << i << ". ";
					cout << (*itVector)->get_name();

					if ((*itVector)->get_state()) cout << endl;
					else cout << " [FORMER EMPLOYEE]" << endl;

					displaySubset.push_back(realPos);

					i++;
					realPos++;
					itVector++;
				}
				break;
		}
	}

	else {

		// 1 -> former; 2 -> current; 3 -> both
		switch (condition) {

			case 1:
				while (it != deliveryPeople.end()) {
					
					if ((*it)->get_state() == false) {
						cout << i << ". ";
						cout << (*it)->get_name() << " [FORMER EMPLOYEE]" << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					it++;
					realPos++;
				}
				break;

			case 2:
				while (it != deliveryPeople.end()) {
					
					if ((*it)->get_state()) {
						cout << i << ". ";
						cout << (*it)->get_name() << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					it++;
					realPos++;
				}
				break;

			case 3:
				while (it != deliveryPeople.end()) {
					cout << i << ". ";
					cout << (*it)->get_name();

					if ((*it)->get_state()) cout << endl;
					else cout << " [FORMER EMPLOYEE]" << endl;

					displaySubset.push_back(realPos);

					i++;
					realPos++;
					it++;
				}
				break;
		}

		/*
		while (it != deliveryPeople.end()) {
			cout << i++ << ". ";
			cout << (*it)->get_name();
			
			if ((*it)->get_state()) cout << endl;
			else cout << " [FORMER EMPLOYEE]" << endl;

			it++;
		}
		*/
	}
}

void Base::seeAllAdminsNames(vector<int> &displaySubset, int i, bool vectorData, vector<Admin*> adminVector) {
	
	int condition = displaySubset[0];
	int realPos = i;

	displaySubset.clear();

	unordered_set<Admin*, hashAdmin, eqAdmin>::iterator it = admins.begin();

	vector<Admin*>::iterator itVector;

	if (vectorData) {
		itVector = adminVector.begin();

		// 1 -> former; 2 -> current; 3 -> both
		switch (condition) {


			case 1:
				while (itVector != adminVector.end()) {





					if ((*itVector)->get_state() == false) {
						cout << i << ". ";
						cout << (*itVector)->get_name() << " [FORMER EMPLOYEE]" << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					itVector++;
					realPos++;

				}
				break;

			case 2:
				while (itVector != adminVector.end()) {

					if ((*itVector)->get_state()) {
						cout << i << ". ";
						cout << (*itVector)->get_name() << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					itVector++;
					realPos++;

				}
				break;
			
			case 3:
				while (itVector != adminVector.end()) {
					cout << i << ". ";
					cout << (*itVector)->get_name();

					if ((*itVector)->get_state()) cout << endl;
					else cout << " [FORMER EMPLOYEE]" << endl;

					displaySubset.push_back(realPos);

					i++;
					realPos++;
					itVector++;
				}
				break;
				
				
			/*
			while (itVector != adminVector.end()) {
				cout << i++ << ". ";
				cout << (*itVector)->get_name();

				if ((*itVector)->get_state()) cout << endl;
				else cout << " [FORMER EMPLOYEE]" << endl;

				itVector++;
			}
			*/
		}
	}

	else {

		// 1 -> former; 2 -> current; 3 -> both
		switch (condition) {
			case 1:
				while (it != admins.end()) {

					if ((*it)->get_state() == false) {
						cout << i << ". ";
						cout << (*it)->get_name() << " [FORMER EMPLOYEE]" << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					it++;
					realPos++;
				}
				break;

			case 2:
				while (it != admins.end()) {

					if ((*it)->get_state()) {
						cout << i << ". ";
						cout << (*it)->get_name() << endl;

						displaySubset.push_back(realPos);

						i++;
					}
					it++;
					realPos++;
				}
				break;

			case 3:
				while (it != admins.end()) {
					cout << i << ". ";
					cout << (*it)->get_name();

					if ((*it)->get_state()) cout << endl;
					else cout << " [FORMER EMPLOYEE]" << endl;

					displaySubset.push_back(realPos);

					i++;
					realPos++;
					it++;
				}
				break;
		}

		/*
		while (it != admins.end()) {
			cout << i++ << ". ";
			cout << (*it)->get_name();

			if ((*it)->get_state()) cout << endl;
			else cout << " [FORMER EMPLOYEE]" << endl;

			it++;
		}
		*/
	}
}

void Base::seeAllDeliveryPeople(vector<int> &displaySubset, int i, bool vectorData, vector<Delivery*> delivPeopleVector) {

	bool noSubsetDisplay = false;
	int condition = 0;

	if (displaySubset.size() == 0) {
		noSubsetDisplay = true;
	}

	else {
		condition = displaySubset[0];
	}

	int realPos = i;

	displaySubset.clear();


	// cout << "ALL OF THE DELIVERIES' PEOPLE" << endl << endl;
	unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator it = deliveryPeople.begin();

	vector<Delivery*>::iterator itVector;

	if (noSubsetDisplay) {

		if (vectorData) {
			itVector = delivPeopleVector.begin();

			while (itVector != delivPeopleVector.end()) {
				cout << i++ << ".";
				if ((*itVector)->get_state()) cout << endl;
				else cout << " [FORMER EMPLOYEE]" << endl;
				(*itVector)->print();

				itVector++;

				cout << endl;
			}
		}

		else {
			while (it != deliveryPeople.end()) {
				cout << i++ << ".";
				if ((*it)->get_state()) cout << endl;
				else cout << " [FORMER EMPLOYEE]" << endl;
				(*it)->print();

				it++;

				cout << endl;
			}
		}
	}

	else {
		if (vectorData) {
			itVector = delivPeopleVector.begin();

			// 1 -> former; 2 -> current; 3 -> both
			switch (condition) {

				case 1:
					while (itVector != delivPeopleVector.end()) {

						if ((*itVector)->get_state() == false) {
							cout << i << ". " << " [FORMER EMPLOYEE]" << endl;
							(*itVector)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						itVector++;
						realPos++;

					}
					break;

				case 2:
					while (itVector != delivPeopleVector.end()) {

						if ((*itVector)->get_state()) {
							cout << i << ". " << endl;
							(*itVector)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						itVector++;
						realPos++;


					}
					break;

				case 3:
					while (itVector != delivPeopleVector.end()) {
						cout << i << ". ";

						if ((*itVector)->get_state()) cout << endl;
						else cout << " [FORMER EMPLOYEE]" << endl;

						(*itVector)->print();

						displaySubset.push_back(realPos);

						i++;
						realPos++;
						itVector++;

						cout << endl;
					}
					break;
			}

		}

		else {
			// 1 -> former; 2 -> current; 3 -> both
			switch (condition) {
				case 1:
					while (it != deliveryPeople.end()) {

						if ((*it)->get_state() == false) {
							cout << i << ". " << " [FORMER EMPLOYEE]" << endl;
							(*it)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						it++;
						realPos++;
					}
					break;

				case 2:
					while (it != deliveryPeople.end()) {

						if ((*it)->get_state()) {
							cout << i << ". ";
							(*it)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						it++;
						realPos++;
					}
					break;

				case 3:
					while (it != deliveryPeople.end()) {
						cout << i << ". ";

						if ((*it)->get_state()) cout << endl;
						else cout << " [FORMER EMPLOYEE]" << endl;

						(*it)->print();

						displaySubset.push_back(realPos);

						cout << endl;

						i++;
						realPos++;
						it++;
					}
					break;
			}
		}
	}
}

void Base::seeAllAdmins(vector<int> &displaySubset, int i, bool vectorData, vector<Admin*> adminVector) {

	bool noSubsetDisplay = false;
	int condition = 0;

	if (displaySubset.size() == 0) {
		noSubsetDisplay = true;
	}
	
	else {
		condition = displaySubset[0];
	}
	
	int realPos = i;

	displaySubset.clear();


	// cout << "ALL ADMINISTRATORS" << endl << endl;
	unordered_set<Admin*, hashAdmin, eqAdmin>::iterator it = admins.begin();

	vector<Admin*>::iterator itVector;
	
	if (noSubsetDisplay) {

		if (vectorData) {
			itVector = adminVector.begin();

			while (itVector != adminVector.end()) {
				cout << i++ << ".";
				if ((*itVector)->get_state()) cout << endl;
				else cout << " [FORMER EMPLOYEE]" << endl;
				(*itVector)->print();
				
				itVector++;

				cout << endl;
			}
		}

		else {
			while (it != admins.end()) {
				cout << i++ << ".";
				if ((*it)->get_state()) cout << endl;
				else cout << " [FORMER EMPLOYEE]" << endl;
				(*it)->print();
				
				it++;

				cout << endl;
			}
		}
	}

	else {
		if (vectorData) {
			itVector = adminVector.begin();

			// 1 -> former; 2 -> current; 3 -> both
			switch (condition) {

				case 1:
					while (itVector != adminVector.end()) {

						if ((*itVector)->get_state() == false) {
							cout << i << ". " << " [FORMER EMPLOYEE]" << endl;
							(*itVector)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						itVector++;
						realPos++;
						
					}
					break;

				case 2:
					while (itVector != adminVector.end()) {

						if ((*itVector)->get_state()) {
							cout << i << ". " << endl;
							(*itVector)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						itVector++;
						realPos++;

						
					}
					break;

				case 3:
					while (itVector != adminVector.end()) {
						cout << i << ". ";

						if ((*itVector)->get_state()) cout << endl;
						else cout << " [FORMER EMPLOYEE]" << endl;

						(*itVector)->print();

						displaySubset.push_back(realPos);

						i++;
						realPos++;
						itVector++;

						cout << endl;
					}
					break;
			}
			/*
			while (itVector != adminVector.end()) {
				cout << "#" << i++ << ".";
				if ((*itVector)->get_state()) cout << endl;
				else cout << " [FORMER EMPLOYEE]" << endl;
				(*itVector)->print();
				cout << endl;
				itVector++;
			}
			*/
		}

		else {
			// 1 -> former; 2 -> current; 3 -> both
			switch (condition) {
				case 1:
					while (it != admins.end()) {

						if ((*it)->get_state() == false) {
							cout << i << ". " << " [FORMER EMPLOYEE]" << endl;
							(*it)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						it++;
						realPos++;
					}
					break;

				case 2:
					while (it != admins.end()) {

						if ((*it)->get_state()) {
							cout << i << ". ";
							(*it)->print();

							displaySubset.push_back(realPos);

							cout << endl;

							i++;
						}
						it++;
						realPos++;
					}
					break;

				case 3:
					while (it != admins.end()) {
						cout << i << ". ";

						if ((*it)->get_state()) cout << endl;
						else cout << " [FORMER EMPLOYEE]" << endl;

						(*it)->print();

						displaySubset.push_back(realPos);

						cout << endl;

						i++;
						realPos++;
						it++;
					}
					break;
			}
		}

	}
}


void Base::seeAllOrders()
{
	cout << "ALL ORDERS" << endl << endl;
	map<int, Order*>::iterator it;
	for (it = orders.begin(); it != orders.end(); it++)
	{
		cout << *((*it).second) << endl << "-------------------------------" << endl;
	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

void Base::seeOneOrder()
{
	bool retry = true;
	do
	{
		try
		{
			utils::clear_screen();
			retry = false;
			cout << "Pick the Order you want to see" << endl;
			int op = 1, answer;
			string input;
			map<int, Order*>::iterator it;
			vector<int> temporary_ids;

			for (it = orders.begin(); it != orders.end(); it++)
			{
				cout << op++ << ". ID: " << (*it).second->getID();
				cout << endl;
				temporary_ids.push_back((*it).second->getID());
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, input);
			if (input == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(input))
				throw InvalidNumberException(input);
			if (input != "")
			{
				if (InvalidOptions(orders.size(), stoi(input)))
					throw InvalidOptionException(stoi(input));
				answer = stoi(input);
			}
			else
			{
				retry = true;
				utils::clear_screen();
				continue;
			}

			cout << "INFO" << endl;
			cout << *(orders.at(temporary_ids.at(answer - 1)));

			cout << "\n>> ";
			cin.clear();
			cin.ignore(INT_MAX,'\n');
		}
		catch (InvalidNumberException & n)
		{
			retry = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidOptionException & o)
		{
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
	} while (retry);

}

void Base::seeProfits()
{
	cout << "Profit for this Base: ";
	map<int, Order*>::iterator it;
	double total = 0;
	for (it = orders.begin(); it != orders.end(); it++)
	{
		total += (*it).second->getDeliveryFee();
	}
	cout << total << endl;

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

void Base::seeProfitsPerRestaurant()
{
	cout << "Profits per Restaurant" << endl << endl;
	vector< Restaurant *>::iterator it;
	for (it = restaurants.begin(); it != restaurants.end(); it++)
	{
		double total = 0;
		map<int, Order*>::iterator ite;
		for (ite = orders.begin(); ite != orders.end(); ite++)
		{
			if ((*it)->get_name() == (*ite).second->getRestaurant()->get_name())
			{
				total += (*ite).second->getDeliveryFee();
			}
		}
		cout << (*it)->get_name() << ": " << total << endl;
	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

void Base::seeProfitsPerClient()
{
	cout << "Profits per Client" << endl << endl;
	vector<Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		double total = 0;
		map<int, Order*> vec = (*it)->get_orders();
		map<int, Order*>::iterator ite;
		for (ite = vec.begin(); ite != vec.end(); ite++)
		{
			total += (*ite).second->getDeliveryFee();
		}
		cout << (*it)->get_name() << ": " << total << endl;
	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

void Base::seeProfitsPerTime()
{

	Date_time left, right, null;
	string left_string, right_string;
	bool retry = true;
	bool teste;
	bool re = true;
	do {
		re = false;
		try {
			do
			{
				try
				{
					retry = false;
					cout << "Please input starting date (day:month:year): "; getline(cin, left_string);
					teste = left.parse(left_string);
					if (!teste || !isDateValid(left)) {
						throw InvalidDateException(left_string);
					}
				}
				catch (InvalidDateException & d)
				{
					retry = true;
					cout << d;
					cout << "Try Again!" << endl << endl;
				}

			} while (retry);


			retry = true;
			do
			{
				try
				{
					retry = false;
					cout << endl << "Please input ending date (day:month:year): "; getline(cin, right_string);
					teste = right.parse(right_string);
					if (!teste || !isDateValid(right)) {
						throw InvalidDateException(right_string);
					}
				}
				catch (InvalidDateException & d)
				{
					retry = true;
					cout << d;
					cout << "Try Again!" << endl << endl;
				}
			} while (retry);

			if (right < left)
				throw InvalidDatesException(left_string, right_string);
		}
		catch (InvalidDatesException & d)
		{
			re = true;
			cout << d;
			cout << "Try Again!" << endl << endl;
		}

	} while (re);



	double total = 0;
	map<int, Order*>::iterator it;
	for (it = orders.begin(); it != orders.end(); it++) {
		if (left < (*it).second->getDateTime() && (*it).second->getDeliver()->getDateTime() < right)
			total += (*it).second->getDeliveryFee();
	}

	cout << "Profits in set period: " << total << endl;

	cout << "\n>>";
	cin.clear();
	cin.ignore(INT_MAX,'\n');

	
}

void Base::seeAllVehicles() {
	cout << "ALL VEHICLES" << endl << endl;
	BST<Vehicle> vehicles = this->getVehicles();
	for (auto it = vehicles.begin(); it != vehicles.end(); it++) {
		(*it).print();
		cout << endl;
	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

void Base::seeOneVehicle() {

	bool invalidOption;
	string opt;
	do
	{
		try
		{
			int i = 0;
			utils::clear_screen();
			invalidOption = false;
			cout << "Pick the Vehicle you want to see: " << endl << endl;
			BST<Vehicle> vehicles = this->getVehicles();
			for (auto it = vehicles.begin() ; it != vehicles.end(); it++, i++)
			{
				cout << i + 1 << ". " << (*it).get_brand() << "; " << (*it).get_license() << endl;
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, opt);
			if (opt == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(opt))
				throw InvalidNumberException(opt);
			if (opt != "")
			{
				if (InvalidOptions(i, stoi(opt)))
					throw InvalidOptionException(stoi(opt));

			}
			else
			{
				invalidOption = true;
				utils::clear_screen();
			}

		}
		catch (InvalidNumberException & n)
		{
			invalidOption = true;
			cout << n;
			cout << "Try Again!" << endl << endl;

		}
		catch (InvalidOptionException & o)
		{
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}



	} while (invalidOption);

	cout << endl << "INFO" << endl;
	auto result = vehicles.begin();
	for (int j = stoi(opt) - 1; j > 0; j--) result++;
	(*result).print();
	
	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

bool Base::seeAllVehiclesWihMileage(Base * base, string input)
{
	bool retry;

	try
	{
		retry = false;
		if (!isNumber(input))
			throw InvalidNumberException(input);
		if (input != "")
		{
			if (InvalidOptions(INT_MAX, stoi(input)))
				throw InvalidOptionException(stoi(input));
		}
		else
		{
			retry = true;
		}
	}
	catch (InvalidNumberException & n)
	{
		retry = true;
		cout << n;
		cout << "Try Again!" << endl << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return false;
	}
	catch (InvalidOptionException & o)
	{
		retry = true;
		cout << o;
		cout << "Try Again!" << endl << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return false;
	}

	utils::clear_screen();

	BST<Vehicle> vehicles = this->getVehicles();
	for (auto it = vehicles.begin(); it != vehicles.end(); it++) {
		if ((*it).get_mileage() == stoi(input)) {
			(*it).print();
			cout << endl;
		}
	}
	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');

	return true;

}

bool Base::seeAllVehiclesWihTripsUnder(Base * base, string input)
{
	bool retry;

	try
	{
		retry = false;
		if (!isNumber(input))
			throw InvalidNumberException(input);
		if (input != "")
		{
			if (InvalidOptions(INT_MAX, stoi(input)))
				throw InvalidOptionException(stoi(input));
		}
		else
		{
			retry = true;
		}
	}
	catch (InvalidNumberException & n)
	{
		retry = true;
		cout << n;
		cout << "Try Again!" << endl << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return false;
	}
	catch (InvalidOptionException & o)
	{
		retry = true;
		cout << o;
		cout << "Try Again!" << endl << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return false;
	}

	utils::clear_screen();

	BST<Vehicle> vehicles = this->getVehicles();
	for (auto it = vehicles.begin(); it != vehicles.end(); it++) {
		if ((*it).get_trips() > stoi(input)) break;
		(*it).print();
		cout << endl;
	}
	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');

	return true;

}

void Base::filterVehicles(Base * base)
{
	while (true)
	{
		string input;
		bool retry;
		do
		{
			try
			{
				retry = false;
				utils::clear_screen();
				cout << "1. Filter by trips made" << endl;
				cout << "2. Filter by mileage" << endl;
				cout << "7. Close program" << endl;
				cout << "0. Go back" << endl;
				cout << ">> ";

				getline(cin, input);
				if (!isNumber(input))
					throw InvalidNumberException(input);
				if (input != "")
				{
					if (InvalidOptions(7, stoi(input)))
						throw InvalidOptionException(stoi(input));
				}
				else
				{
					retry = true;
				}
			}
			catch (InvalidNumberException & n)
			{
				retry = true;
				cout << n;
				cout << "Try Again!" << endl << endl;
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			catch (InvalidOptionException & o)
			{
				retry = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}

		} while (retry);

		if (input == "0")
			break;

		if (input == "1") {
			cout << "Show all vehicles with trips made under: ";
			getline(cin, input);
			if (!base->seeAllVehiclesWihTripsUnder(base, input)) continue;
			else break;
		}

		if (input == "2") {
			cout << "Show all vehicles with mileage equal to: ";
			getline(cin, input);
			if (!base->seeAllVehiclesWihMileage(base, input)) continue;
			else break;

		}

		if (input == "7")
		{
			cin.clear();
			base->writeAll();
		}
		else
			continue;
	}
}

void Base::seeAllTechnicians()
{
	cout << "ALL TECHNICIANS" << endl << endl;
	vector<Technician> temp;
	while (!technicians.empty())
	{
		temp.push_back(technicians.top());
		technicians.pop();
	}
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i].print();
		cout << endl;
	}
	for (int i = 0; i < temp.size(); i++)
	{
		technicians.push(temp[i]);
	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

void Base::seeOneTechnician()
{
	vector<Technician> temp;
	while (!technicians.empty())
	{
		temp.push_back(technicians.top());
		technicians.pop();
	}
	bool invalidOption;
	string opt;
	do
	{
		try
		{
			utils::clear_screen();
			invalidOption = false;
			cout << "Pick the Technician you want to see: " << endl << endl;
			for (int i = 0; i < temp.size(); i++)
			{
				cout << i + 1 << ". " << temp[i].get_name() << endl;
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, opt);
			if (opt == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(opt))
				throw InvalidNumberException(opt);
			if (opt != "")
			{
				if (InvalidOptions(temp.size(), stoi(opt)))
					throw InvalidOptionException(stoi(opt));

			}
			else
			{
				invalidOption = true;
				utils::clear_screen();
			}

		}
		catch (InvalidNumberException & n)
		{
			invalidOption = true;
			cout << n;
			cout << "Try Again!" << endl << endl;

		}
		catch (InvalidOptionException & o)
		{
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}



	} while (invalidOption);

	cout << endl << "INFO" << endl;
	int index = stoi(opt) - 1;
	temp[index].print();
	for (int i = 0; i < temp.size(); i++)
	{
		technicians.push(temp[i]);
	}

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}


void Base::seeBase()
{
	cout << this->getDistrict() << endl;
	cout << this->getAddress() << endl;
	cout << "Admin: " << this->getAdmin()->get_name();
}



void Base::changeBase() {
	list<string> options = { "address" };

	string baseName = this->getDistrict();
	bool invalidOption;
	string strChoice;
	unsigned index = 0;

	cout << "Pick the field you want to change information of:" << endl;
	list<string>::iterator optionsIt;
	int attributeChoice;
	do {
		index = 0;
		invalidOption = false;

		for (optionsIt = options.begin(); optionsIt != options.end(); ++optionsIt, ++index) {
			cout << index + 1 << ". " << (*optionsIt) << endl;
		}

		try {
			cout << ">> ";
			getline(cin, strChoice);

			if (cin.eof()) {
				cin.clear();
				return;
			}

			attributeChoice = stoi(strChoice);

			if (attributeChoice < 1 || attributeChoice > options.size()) {
				invalidOption = true;
			}
		}
		catch (...) {
			invalidOption = true;
		}

		cout << endl;

	} while (invalidOption);


	Address newAddress;
	bool invalidAddress = false;
	string fullAddress;
	vector<string> areaOfInf = this->getAreaOfInfluence();

	switch (attributeChoice) {
		// Address
	case 1:
		do {
			invalidAddress = false;

			cout << "Current Address: " << this->getAddress() << endl;
			cout << "Updated Address (Town / Street / No / Floor / Latitude / Longitude): " << endl;
			getline(cin, fullAddress);

			if (cin.eof()) {
				cin.clear();
				return;
			}

			try { //esta exceção não está testada
				bool teste = address.parseInsideBase(fullAddress, baseName);
				if (!teste)
					throw InvalidAddressException(fullAddress);

				// if it doesnt belong to the are of influence it is considered invalid
				if (find(areaOfInf.begin(), areaOfInf.end(), address.get_town()) == areaOfInf.end()) {
					throw InvalidCityException(address.get_town());
				}
			}

			catch (InvalidAddressException & a) {
				invalidAddress = true;
				cout << a;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidCityException & c)
			{
				invalidAddress = true;
				cout << c;
				cout << "Try Again!" << endl << endl;
			}

			cout << endl;
		} while (invalidAddress);
		this->setAddress(address);

		break;

	}

	cout << "Based successfully edited! (Enter to continue)" << endl;
	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

bool Base::addClient() {

	try
	{
		Client c;

		string baseName = this->getDistrict();
		c.set_base(this);

		// name input
		bool invalidName;
		string name;
		do {
			try
			{
				invalidName = false;

				cout << "Name: ";
				getline(cin, name);
				if (!isString(name))
					throw InvalidStringException(name);
			}
			catch (InvalidStringException & s)
			{
				invalidName = true;
				cout << s;
				cout << "Try Again!" << endl << endl;

			}
		} while (invalidName);

		c.set_name(name);

		if (find(Base::blacklist.begin(), Base::blacklist.end(), c.get_name()) != Base::blacklist.end()) {
			throw BlackListedException(c.get_name());
		}

		// nif input
		bool invalidNif;
		string strNif;
		do {
			invalidNif = false;

			cout << "NIF: ";
			getline(cin, strNif);

			try {
				if (!isNumber(strNif) || strNif.size() != 9)
					throw InvalidNIFException(strNif);
			}

			catch (InvalidNIFException & n) {
				invalidNif = true;
				cout << n;
				cout << "Try Again!" << endl << endl;
			}

		} while (invalidNif);

		c.set_NIF(stoi(strNif));

		// address input
		bool invalidAddress;
		Address address;
		vector<string> areaOfInf = this->getAreaOfInfluence();

		do {
			invalidAddress = false;

			string fullAddress;
			cout << "Address (Town / Street / No / Floor / Latitude / Longitude):" << endl;
			getline(cin, fullAddress);

			try {
				bool teste = address.parseInsideBase(fullAddress, baseName);
				if (!teste) {
					throw InvalidAddressException(fullAddress);
				}
				// if it doesnt belong to the area of influence it is considered invalid
				if (find(areaOfInf.begin(), areaOfInf.end(), address.get_town()) == areaOfInf.end()) {
					throw InvalidCityException(address.get_town());
				}
			}

			catch (InvalidAddressException & a) {
				invalidAddress = true;
				cout << a;
				cout << "Try Again!" << endl << endl;

			}
			catch (InvalidCityException & c)
			{
				cout << c;
				cout << ">> ";
				cin.clear();
				cin.ignore(INT_MAX,'\n');
				return false;
			}

		} while (invalidAddress);

		c.set_address(address);

		// orders vector that starts empty
		map<int, Order*> clientOrders = {};

		c.set_orders(clientOrders);


		Client * ptr = new Client;
		*ptr = c;

		clients.push_back(ptr);

		cout << endl;
		cout << "Client added successfully" << endl;
		cout << ">> ";
		cin.clear();
		cin.ignore(INT_MAX,'\n');
	}
	catch (BlackListedException & b)
	{
		cout << b;
		cout << ">> ";
		cin.clear();
		cin.ignore(INT_MAX,'\n');
		return false;

	}
	return true;
}

void Base::changeClient() {



	vector<Client*>::iterator it;
	bool invalidOption;
	string strChoice;
	int clientChoice;
	unsigned index = 0;


	do {
		index = 0;
		invalidOption = false;
		cout << "Pick the client you want to change information about:" << endl;
		for (it = clients.begin(); it != clients.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);
			if (strChoice == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);


			if (strChoice != "")
			{
				clientChoice = stoi(strChoice);
				if (InvalidOptions(clients.size(), clientChoice)) {
					throw InvalidOptionException(clientChoice);
				}
			}
			else
			{
				invalidOption = true;
				utils::clear_screen();


			}

		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
		

	} while (invalidOption);

	clientChoice--; // not to excede the max index available

	clients.at(clientChoice)->edit(this);
}

void Base::removeClient() {



	vector<Client*>::iterator it;
	bool invalidOption;
	string strChoice;
	int clientChoice;
	unsigned index = 0;


	do {
		index = 0;
		invalidOption = false;
		cout << "Pick the client you want to remove:" << endl;
		for (it = clients.begin(); it != clients.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);

			if (strChoice == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);

			if (strChoice != "")
			{
				clientChoice = stoi(strChoice);

				if (InvalidOptions(clients.size(), clientChoice)) {
					throw InvalidOptionException(clientChoice);
				}
			}
			else
			{
				invalidOption = true;
				utils::clear_screen();
			}

		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;

	} while (invalidOption);

	clientChoice--; // not to excede the max index available

	// To free the memory
	vector<Client*>::iterator eraseIt = clients.begin() + clientChoice;
	delete * eraseIt;

	// To erase from the vector
	clients.erase(clients.begin() + clientChoice);

	cout << "Client removed successfully" << endl;
	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}


void Base::addWorker() {
	
	bool invalidWorkerType;
	string strWorkerType;
	int workerType;

	do {
		try {
			invalidWorkerType = false;
			cout << "Adding an Administrator or a Delivery person?" << endl;
			cout << "1. Administrator" << endl;
			cout << "2. Delivery" << endl;
			cout << "0. Go Back" << endl;
			cout << ">> ";

			getline(cin, strWorkerType);

			if (strWorkerType == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strWorkerType))	throw InvalidNumberException(strWorkerType);
			if (strWorkerType != "") {
				if (InvalidOptions(2, stoi(strWorkerType))) throw InvalidOptionException(stoi(strWorkerType));
			}
			else {
				invalidWorkerType = true;
				utils::clear_screen();
			}
		}
		catch (InvalidOptionException & o)
		{
			invalidWorkerType = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidWorkerType = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
	} while (invalidWorkerType);

	workerType = stoi(strWorkerType);

	switch (workerType) {
		case 1:
			addAdmin();
			break;
		
		case 2:
			addDeliverer();
			break;
	}

}

void Base::addAdmin() {

	Admin a;

	utils::clear_screen();

	cout << "Administrator Sign In" << endl << endl;

	bool managerExists = checkForManager();
	bool invalidManagerInput = false;
	string strManagerInput;
	int managerInput = -1;

	bool invalidRoleInput = false;
	string roleInput;

	// Role Input
	if (!managerExists) {
		do {
			invalidManagerInput = false;

			cout << "Is this Administrator the Manager? " << endl;
			cout << "1. Yes" << endl;
			cout << "2. No" << endl;
			cout << ">> ";
			getline(cin, strManagerInput);

			if (cin.eof()) {
				cin.clear();
				return;
			}

			try {
				if (!isNumber(strManagerInput)) throw InvalidNumberException(strManagerInput);
				managerInput = stoi(strManagerInput);

				if (strManagerInput != "") {
					if (InvalidOptions(2, managerExists)) throw InvalidOptionException(managerExists);
				}
				else {
					invalidManagerInput = true;
					utils::clear_screen();
				}
			}
			catch (InvalidOptionException & o) {
				invalidManagerInput = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s) {
				invalidManagerInput = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

			cout << endl;
		} while (invalidManagerInput);
	}

	if (managerInput == 1) a.set_role("manager");

	else {
		do {
			try {
				invalidRoleInput = false;
				cout << "Role: ";
				getline(cin, roleInput);
				if (!isString(roleInput)) throw InvalidStringException(roleInput);

				// if it is not manager then it can't be manager
				if (roleInput == "manager") {
					invalidRoleInput = true;
				}

				cout << endl;
			}

			catch (InvalidStringException & s) {
				invalidRoleInput = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

		} while (invalidRoleInput);

		a.set_role(roleInput);
	}
	
	// Name Input
	bool invalidName;
	string name;
	do {
		try{
			invalidName = false;
			cout << "Name: ";
			getline(cin, name);
			if (!isString(name)) throw InvalidStringException(name);
			cout << endl;
		}

		catch (InvalidStringException & s) {
			invalidName = true;
			cout << s;
			cout << "Try Again" << endl << endl;
		}

	} while (invalidName);
	a.set_name(name);

	// Nif Input
	bool invalidNif;
	string strNif;
	do {
		invalidNif = false;

		cout << "NIF: ";
		getline(cin, strNif);

		try {
			if (!isNumber(strNif) || strNif.size() != 9) throw InvalidNIFException(strNif);
		}

		catch (InvalidNIFException & n) {
			invalidNif = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;
	} while (invalidNif);

	a.set_NIF(stoi(strNif));

	// Birth day Input
	bool invalidBirthday;
	Date_time birthday;
	bool teste;
	do {
		invalidBirthday = false;

		string fullBirthday;
		cout << "Birthday: ";
		getline(cin, fullBirthday);

		try {
			teste = birthday.parse(fullBirthday);
			if (!teste || !isDateValid(birthday)) throw InvalidDateException(fullBirthday);
		}
		catch (InvalidDateException & d) {
			invalidBirthday = true;
			cout << d;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;
	} while (invalidBirthday);
	
	a.set_birthday(birthday);

	// Wage Input
	bool invalidWage;
	string strWage;

	do {
		invalidWage = false;

		cout << "Wage: ";
		getline(cin, strWage);

		try {
			if (!isNumber(strWage)) throw InvalidNumberException(strWage);

			if (stod(strWage) < 0) {
				invalidWage = true;
				cout << "Wage must be a positive value." << endl;
				cout << "Try Again!" << endl << endl;
			}
		}

		catch (InvalidNumberException & s) {
			invalidWage = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalidWage);

	a.set_wage(stod(strWage));


	a.set_state(true);


	Admin * adminPtr = new Admin;
	*adminPtr = a;

	if (checkInactiveAdminInDatabase(a)) {
		cout << "\nWelcome Back! (Enter to continue)" << endl;
	}

	else {
		auto pair_ = admins.insert(adminPtr);

		// If Error in Insertion, the admin was already "in the system"
		if (pair_.second == false) {
			cout << "Error In Sign In" << endl;
		}
		else {
			cout << "\nWorker successfully added! (Enter to continue)" << endl;
		}
	}

	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

bool Base::checkForManager() {
	unordered_set<Admin*, hashAdmin, eqAdmin>::iterator it = admins.begin();

	while (it != admins.end()) {
		if ( ((*it)->get_role() == "manager") && ( (*it)->get_state() == true)) return true;
		it++;
	}

	return false;
}

bool Base::checkInactiveAdminInDatabase(Admin a) {

	unordered_set<Admin*, hashAdmin, eqAdmin>::iterator adminIt = admins.begin();

	while (adminIt != admins.end()) {

		if ((*(*adminIt) == a) && ((*adminIt)->get_state() == false)) {
			(*adminIt)->set_state(true);
			return true;
		}

		adminIt++;
	}
	return false;
}

void Base::addDeliverer() {

	Delivery del;
	Vehicle veh;

	utils::clear_screen();

	cout << "Delivery Person Sign In" << endl << endl;
	
	// Name Input
	bool invalidName;
	string name;
	do {
		try {
			invalidName = false;
			cout << "Name: ";
			getline(cin, name);
			if (!isString(name)) throw InvalidStringException(name);
			cout << endl;
		}

		catch (InvalidStringException & s) {
			invalidName = true;
			cout << s;
			cout << "Try Again" << endl << endl;
		}

	} while (invalidName);
	del.set_name(name);

	// Nif Input
	bool invalidNif;
	string strNif;
	do {
		invalidNif = false;

		cout << "NIF: ";
		getline(cin, strNif);

		try {
			if (!isNumber(strNif) || strNif.size() != 9) throw InvalidNIFException(strNif);
		}

		catch (InvalidNIFException & n) {
			invalidNif = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;
	} while (invalidNif);

	del.set_NIF(stoi(strNif));

	// Birth day Input
	bool invalidBirthday;
	Date_time birthday;
	bool teste;
	do {
		invalidBirthday = false;

		string fullBirthday;
		cout << "Birthday: ";
		getline(cin, fullBirthday);

		try {
			teste = birthday.parse(fullBirthday);
			if (!teste || !isDateValid(birthday)) throw InvalidDateException(fullBirthday);
		}
		catch (InvalidDateException & d) {
			invalidBirthday = true;
			cout << d;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;
	} while (invalidBirthday);

	del.set_birthday(birthday);


	veh = newVehicle();

	Vehicle * vehiclePtr = new Vehicle;
	*vehiclePtr = veh;
	del.set_vehicle(veh);

	del.set_state(true);
	

	Delivery * delivPtr = new Delivery;
	*delivPtr = del;
	
	if (checkInactiveDelivPersonInDatabase(del)) {
		cout << "\nWelcome Back! (Enter to continue)" << endl;
	}

	else {
		auto pair_ = deliveryPeople.insert(delivPtr);

		// If Error in Insertion, the admin was already "in the system"
		if (pair_.second == false) {
			cout << "\nError In Sign In" << endl;
		}
		else {
			cout << "\nWorker successfully added! (Enter to continue)" << endl;
		}
	}

	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

bool Base::checkInactiveDelivPersonInDatabase(Delivery d) {

	unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator delivIt = deliveryPeople.begin();

	while (delivIt != deliveryPeople.end()) {

		if ((*(*delivIt) == d) && ((*delivIt)->get_state() == false)) {
			(*delivIt)->set_state(true);
			return true;
		}

		delivIt++;
	}
	return false;
}

Vehicle Base::newVehicle() {

	Vehicle v;

	// Vehicle Brand
	bool retry = true;
	string vehicleBrand;
	
	do {
		try {
			retry = false;
			cout << "Vehicle Brand: ";
			getline(cin, vehicleBrand);
			if (!isString(vehicleBrand)) throw InvalidStringException(vehicleBrand);
		}
		catch (InvalidStringException & s) {
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

	v.set_brand(vehicleBrand);

	cout << endl;

	// Vehicle Type
	string vehicleType;

	do {
		try {
			retry = false;
			cout << "Vehicle Type: ";
			getline(cin, vehicleType);
			if (!isString(vehicleType)) throw InvalidStringException(vehicleType);
		}

		catch (InvalidStringException & s) {
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

	v.set_type(vehicleType);

	cout << endl;
	
	// Vehicle Registration Date
	bool invalidRegistrationDate = false;
	bool checkValid;
	string strRegistrationDate;
	Date_time registrationDate;

	do {
		invalidRegistrationDate = false;
		cout << "Registration Date: ";
		getline(cin, strRegistrationDate);

		try {
			checkValid = registrationDate.parse(strRegistrationDate);
			if (!checkValid || !isDateValid(registrationDate))
				throw InvalidDateException(strRegistrationDate);
		}

		catch (InvalidDateException & d) {
			invalidRegistrationDate = true;
			cout << d;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;
	} while (invalidRegistrationDate);

	v.set_registrationDate(registrationDate);

	return v;
}


void Base::changeWorker() {

	bool retry = true;
	int answer;
	string input;

	vector<int> displaySubset = {};

	do {
		try {
			utils::clear_screen();
			retry = false;
			cout << "Pick the worker you want to change" << endl << endl;

			cout << "Administrators" << endl << endl;
			seeAllAdmins(displaySubset);
			cout << endl << endl;
			cout << "Delivery People" << endl << endl;
			seeAllDeliveryPeople(displaySubset, (int) admins.size() + 1);

			cout << "0. Go Back" << endl << endl;
			cout << ">> ";
			getline(cin, input);

			if (input == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(input)) throw InvalidNumberException(input);

			if (input != "") {
				if (InvalidOptions(admins.size() + deliveryPeople.size(), stoi(input))) throw InvalidOptionException(stoi(input));
				answer = stoi(input);
			}

			else {
				retry = true;
				utils::clear_screen();
			}
		}

		catch (InvalidOptionException & o) {
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}

		catch (InvalidNumberException & s) {
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

	cout << endl;

	int counter;

	// worker chosen was not an admin
	if (answer > admins.size()) {
		unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator delIt = deliveryPeople.begin();
		int offset = answer % admins.size();
		counter = 0;

		while (counter < offset - 1) {
			delIt++;
			counter++;
		}

		Delivery delivPerson = **delIt;

		deliveryPeople.erase(delIt);

		Delivery d = changeDeliveryPerson(delivPerson);

		Delivery * dPtr = new Delivery;
		*dPtr = d;

		auto pair_1 = deliveryPeople.insert(dPtr);

		// If an Error Occured
		if (pair_1.second == false) {
			cout << "\nError In Changes" << endl;
		}

		else {
			cout << "\nWorker successfully changed! (Enter to continue)" << endl;
		}


	}
	else { // worker chosen was an admin

		unordered_set<Admin*, hashAdmin, eqAdmin>::iterator admIt = admins.begin();
		counter = 0;

		while (counter < answer - 1) {
			admIt++;
			counter++;
		}

		Admin administrator = **admIt;

		admins.erase(admIt);

		Admin a = changeAdmin(administrator);

		Admin * aPtr = new Admin;
		*aPtr = a;

		auto pair_2 = admins.insert(aPtr);

		// If an Error Occured
		if (pair_2.second == false) {
			cout << "\nError In Changes" << endl;
		}
		else {
			cout << "\nWorker successfully changed! (Enter to continue)" << endl;
		}
	}
	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');

}

string Base::changeName(string currentName) {

	bool invalid;
	string newName;

	do {
		invalid = false;

		try {
			cout << "Current Name: " << currentName << endl;
			cout << "Updated Name: ";
			getline(cin, newName);
			if (!isString(newName)) throw InvalidStringException(newName);
		}

		catch (InvalidStringException & s) {
			invalid = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalid);
	
	cout << endl; 

	return newName;
}

int Base::changeNif(int currentNif) {

	bool invalid;
	string strNewNif;

	do {
		invalid = false;

		cout << "Current Nif: " << currentNif << endl;
		cout << "Updated Nif: ";
		getline(cin, strNewNif);

		try {
			if (!isNumber(strNewNif) || strNewNif.size() != 9)
				throw InvalidNIFException(strNewNif);
		}

		catch (InvalidNIFException & n) {
			invalid = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
	
	} while (invalid);

	cout << endl;

	return stoi(strNewNif);
}

Date_time Base::changeBirthday(Date_time currentBday) {

	bool invalid, noParseError;
	string fullBirthday;
	Date_time newBday;

	do {
		invalid = false;

		cout << "Current Birthday: " << currentBday << endl;
		cout << "Updated Birthday: ";
		getline(cin, fullBirthday);
		
		try {
			noParseError = newBday.parse(fullBirthday);
			if (!noParseError || !isDateValid(newBday)) throw InvalidDateException(fullBirthday);
		}

		catch (InvalidDateException & d) {
			invalid = true;
			cout << d;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalid);

	cout << endl;

	return newBday;
}

double Base::changeWage(double currentWage) {

	bool invalid;
	string strNewWage;

	do {
		invalid = false;

		cout << "Current Wage: " << currentWage << endl;
		cout << "Updated Wage: ";
		getline(cin, strNewWage);

		try {
			if (!isNumber(strNewWage)) throw InvalidNumberException(strNewWage);
		}

		catch (InvalidNumberException & n) {
			invalid = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalid);

	cout << endl;

	return stod(strNewWage);
}

string Base::changeRole(string currentRole, bool managerExists) {

	bool invalid;
	string newRole;

	do {
		try {
			invalid = false;
			cout << "Current role: " << currentRole << endl;
			cout << "Updated role: ";
			getline(cin, newRole);

			if (!isString(newRole)) throw InvalidStringException(newRole);

			if (newRole == "manager" && managerExists) {
				invalid = true;
				cout << "There is already a manager in this base" << endl;
			}

		}

		catch (InvalidStringException & s) {
			invalid = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalid);

	return newRole;
}

Vehicle Base::changeVehicle(Vehicle currentVehicle) {

	cout << "Current Vehicle" << endl;

	cout << "Vehicle Brand: " << currentVehicle.get_brand() << endl;
	cout << "Vehicle Type: " << currentVehicle.get_type() << endl;
	cout << "Vehicle Registration Date: " << currentVehicle.get_registration_date() << endl;

	cout << endl << endl;
	Vehicle newVehicle = Base::newVehicle();

	return newVehicle;
}

map<int, Order*> Base::changeHistory(map<int, Order*> currentHistory) {
	
	map<int, Order*> newHistory;
	

	return newHistory;
}


Delivery Base::changeDeliveryPerson(Delivery d) {
	
	list<string> delivOptions = { "Name", "Nif", "Birthday", "Vehicle", "Delivery History" };

	bool invalid;
	int index;
	list<string>::iterator optionsIt;
	string strDelivAttributeChoice;
	int delivAttributeChoice;

	do {
		index = 0;
		invalid = false;

		cout << "Pick the field you want to change information of:" << endl;

		for (optionsIt = delivOptions.begin(); optionsIt != delivOptions.end(); ++optionsIt, ++index) {
			cout << index + 1 << ". " << (*optionsIt) << endl;
		}

		try {
			cout << ">> ";
			getline(cin, strDelivAttributeChoice);

			if (!isNumber(strDelivAttributeChoice)) throw InvalidNumberException(strDelivAttributeChoice);

			if (strDelivAttributeChoice != "") {
				delivAttributeChoice = stoi(strDelivAttributeChoice);
				if (InvalidOptions(delivOptions.size(), delivAttributeChoice)) throw InvalidOptionException(delivAttributeChoice);
			}

			else {
				invalid = true;
				utils::clear_screen();
			}
		}

		catch (InvalidOptionException & o) {
			invalid = true;
			cout << o;
			cout << "Try Again!" << endl;
		}

		catch (InvalidNumberException & s) {
			invalid = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalid);


	switch (delivAttributeChoice) {

		// Name
		case 1:
			d.set_name(changeName(d.get_name()));
			break;

		// Nif
		case 2:
			d.set_NIF(changeNif(d.get_NIF()));
			break;

		// Birthday
		case 3:
			d.set_birthday(changeBirthday(d.get_birthday()));
			break;

		// Vehicle
		case 4:
			d.set_vehicle(changeVehicle(d.get_vehicle()));
			break;

		// Delivery History
		case 5:
			d.set_history(changeHistory(d.get_history()));
			d.set_wage(d.calculate_wage());
			break;
	}

	return d;
}

Admin Base::changeAdmin(Admin a) {

	list<string> adminOptions = { "Name", "Nif", "Birthday", "Wage", "Role" };
	
	bool invalid;
	int index;
	list<string>::iterator optionsIt;
	string strAdminAttributeChoice;
	int adminAttributeChoice;

	bool managerExists = checkForManager();

	do {
		index = 0;
		invalid = false;

		cout << "Pick the field you want to change information of:" << endl;

		for (optionsIt = adminOptions.begin(); optionsIt != adminOptions.end(); ++optionsIt, ++index) {
			cout << index + 1 << ". " << (*optionsIt) << endl;
		}

		try {
			cout << ">> ";
			getline(cin, strAdminAttributeChoice);

			if (!isNumber(strAdminAttributeChoice)) throw InvalidNumberException(strAdminAttributeChoice);

			if (strAdminAttributeChoice != "") {
				adminAttributeChoice = stoi(strAdminAttributeChoice);
				if (InvalidOptions(adminOptions.size(), adminAttributeChoice)) throw InvalidOptionException(adminAttributeChoice);
			}

			else {
				invalid = true;
				utils::clear_screen();
			}
		}

		catch (InvalidOptionException & o) {
			invalid = true;
			cout << o;
			cout << "Try Again!" << endl;
		}

		catch (InvalidNumberException & s) {
			invalid = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
	
	} while (invalid);


	switch (adminAttributeChoice) {

		// Name
		case 1:
			a.set_name( changeName(a.get_name()) );
			break;

		// Nif
		case 2:
			a.set_NIF( changeNif(a.get_NIF()) );
			break;

		// Birthday
		case 3:
			a.set_birthday( changeBirthday(a.get_birthday()) );
			break;

		// Wage
		case 4:
			a.set_wage( changeWage(a.get_wage()) );
			break;

		// Role
		case 5:
			a.set_role( changeRole(a.get_role(), managerExists) );
			break;
	}

	return a;
}

void Base::removeWorker() {

	bool retry = true;
	int answer;
	string input;

	vector<int> displaySubset = {};

	do {
		try {
			utils::clear_screen();
			retry = false;
			cout << "Pick the worker you want to remove" << endl << endl;

			cout << "Administrators" << endl << endl;
			seeAllAdmins(displaySubset);
			cout << endl << endl;
			cout << "Delivery People" << endl << endl;
			seeAllDeliveryPeople(displaySubset, (int) admins.size() + 1);

			cout << "0. Go Back" << endl << endl;
			cout << ">> ";
			getline(cin, input);

			if (input == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(input)) throw InvalidNumberException(input);

			if (input != "") {
				if (InvalidOptions(admins.size() + deliveryPeople.size(), stoi(input))) throw InvalidOptionException(stoi(input));
				answer = stoi(input);
			}

			else {
				retry = true;
				utils::clear_screen();
			}
		}

		catch (InvalidOptionException & o) {
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}

		catch (InvalidNumberException & s) {
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

	cout << endl;

	int counter;

	// worker chosen was not an admin
	if (answer > admins.size()) {

		unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator delIt = deliveryPeople.begin();
		int offset = answer % admins.size();
		counter = 0;

		while (counter < offset - 1) {
			delIt++;
			counter++;
		}
		
		// Set to Inactive instead of deleting it (deliveryPeople.erase(delIt) )
		(*delIt)->set_state(false);

	}

	else {

		unordered_set<Admin*, hashAdmin, eqAdmin>::iterator admIt = admins.begin();
		counter = 0;

		while (counter < answer - 1) {
			admIt++;
			counter++;
		}

		// Set to Inactive instead of deleting it (admins.erase(admIt) )
		(*admIt)->set_state(false);

	}

	cout << "\nWorker successfully removed! (Enter to continue)" << endl;

	cout << "\n>> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');

}


void Base::addRestaurant() {

	Restaurant r;

	// HARD CODED FOR BASE PORTO
	// vector<string> areaOfInfluence = { "Porto", "Matosinhos", "Vila Nova de Gaia", "Gondomar", "Maia" };
	string baseName = this->getDistrict();
	r.setBase(this);

	// name input
	bool invalidName = true;
	string name;
	do {
		try
		{
			invalidName = false;

			cout << "Name: ";
			getline(cin, name);
			if (!isString(name))
				throw InvalidStringException(name);
		}
		catch (InvalidStringException & s)
		{
			invalidName = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

	} while (invalidName);

	r.setName(name);

	// address input
	bool invalidAddress;
	Address address;
	string fullAddress;
	vector<string> areaOfInf = this->getAreaOfInfluence();

	do {
		invalidAddress = false;

		cout << "Address (Town / Street / No / Floor / Latitude / Longitude): ";
		getline(cin, fullAddress);

		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

		try {
			bool teste = address.parseInsideBase(fullAddress, baseName);
			if (!teste)
				throw InvalidAddressException(fullAddress);

			// if it doesnt belong to the are of influence it is considered invalid
			if (find(areaOfInf.begin(), areaOfInf.end(), address.get_town()) == areaOfInf.end()) {
				throw InvalidCityException(address.get_town());
			}
		}

		catch (InvalidAddressException & a) {
			invalidAddress = true;
			cout << a;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidCityException & c)
		{
			cout << c;
			cout << "\n>>";
			cin.clear();
			cin.ignore(INT_MAX,'\n');
			return;
		}

	} while (invalidAddress);

	r.setAddress(address);

	// products input
	bool invalidProduct;
	string strProduct;
	Product product;
	vector<Product*> productsVec;

	bool notEnded = true;

	cout << "List of Products (Name : Type : Price), type 'done' to stop input" << endl;

	do {

		do {
			invalidProduct = false; //Implementar uma exceção invalid product

			getline(cin, strProduct);

			//if (cin.eof()) {
			//	cin.clear();
			//	return;
			//}

			if (strProduct == "done") {
				notEnded = false;
				break;
			}

			try {
				bool teste = product.parse(strProduct);
				if (!teste)
					throw InvalidProductException(strProduct);
				Product * productPtr = new Product;
				*productPtr = product;
				productsVec.push_back(productPtr);
			}
			catch (InvalidProductException & p) {
				invalidProduct = true;
				cout << p;
				cout << "Try Again!" << endl << endl;
			}
		} while (invalidProduct);

	} while (notEnded);

	r.setProducts(productsVec);


	// cuisine types
	set<string> cuisineTypes;
	for (auto & prod : r.get_products()) {
		cuisineTypes.insert(prod->get_cuisine_type());
	}


	r.setCuisineTypes(cuisineTypes);



	// price average calculated form the vector of products
	r.setPriceAverage();

	Restaurant * restaurantPtr = new Restaurant;
	*restaurantPtr = r;
	restaurants.push_back(restaurantPtr);

	cout << endl;
	cout << "Restaurant successfully added! (Enter to continue)" << endl;
	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

void Base::changeRestaurant() {

	vector<Restaurant*>::iterator it;
	bool invalidOption;
	string strChoice;
	int restaurantChoice;
	unsigned index = 0;



	do {
		index = 0;
		invalidOption = false;
		cout << "Pick the restaurant you want to change information about:" << endl;
		for (it = restaurants.begin(); it != restaurants.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);
			if (strChoice == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);
			if (strChoice != "") {
				restaurantChoice = stoi(strChoice);

				if (InvalidOptions(restaurants.size(), restaurantChoice)) {
					throw InvalidOptionException(restaurantChoice);
				}
			}
			else {
				invalidOption = true;
				utils::clear_screen();
			}

		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
		//cout << endl;

	} while (invalidOption);

	restaurantChoice--;	// not to excede the max index available

	list<string> options = { "name", "address", "products" };

	list<string>::iterator optionsIt;
	int attributeChoice;
	do {
		index = 0;
		invalidOption = false;
		cout << "Pick the field you want to change information of:" << endl;
		for (optionsIt = options.begin(); optionsIt != options.end(); ++optionsIt, ++index) {
			cout << index + 1 << ". " << (*optionsIt) << endl;
		}

		try {
			cout << ">> ";
			getline(cin, strChoice);

			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);
			if (strChoice != "")
			{
				attributeChoice = stoi(strChoice);

				if (InvalidOptions(options.size(), attributeChoice)) {
					throw InvalidOptionException(attributeChoice);
				}
			}
			else
			{
				invalidOption = true;
				utils::clear_screen();

			}

		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;

	} while (invalidOption);


	string newName;
	bool invalidName;

	string baseName = this->getDistrict();

	Address newAddress;
	bool invalidAddress = false;
	string fullAddress;
	vector<string> areaOfInf = this->getAreaOfInfluence();

	bool invalidProduct;
	string strProduct;
	Product product;
	vector<Product*> productsVec;
	bool notEnded = true;

	set<string> cuisineTypes;

	switch (attributeChoice) {
		// Name
	case 1:
		do {
			try
			{
				invalidName = false;
				cout << "Current Name: " << restaurants.at(restaurantChoice)->get_name() << endl;
				cout << "Updated Name: ";
				getline(cin, newName);
				if (!isString(newName))
					throw InvalidStringException(newName);
				//if (cin.eof()) {
				//	cin.clear();
				//	return;
				//}

				cout << endl;
				restaurants.at(restaurantChoice)->setName(newName);
			}
			catch (InvalidStringException & s)
			{
				invalidName = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

		} while (invalidName);
		break;

		// Address
	case 2:
		do {
			invalidAddress = false;

			cout << "Current Address:\n" << restaurants.at(restaurantChoice)->get_address() << endl;
			cout << "Updated Address (Town / Street / No / Floor / Latitude / Longitude): " << endl;
			getline(cin, fullAddress);

			if (cin.eof()) {
				cin.clear();
				return;
			}

			try {
				bool teste = newAddress.parseInsideBase(fullAddress, baseName);
				if (!teste)
					throw InvalidAddressException(fullAddress);

				// if it doesnt belong to the are of influence it is considered invalid
				if (find(areaOfInf.begin(), areaOfInf.end(), newAddress.get_town()) == areaOfInf.end()) {
					throw InvalidCityException(newAddress.get_town());
				}
			}

			catch (InvalidAddressException & a) {
				invalidAddress = true;
				cout << a;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidCityException & c)
			{
				cout << c;
				cin.clear();
				cin.ignore(INT_MAX,'\n');
				cout << ">> ";
				return;
			}

			cout << endl;
		} while (invalidAddress);

		cout << endl;
		restaurants.at(restaurantChoice)->setAddress(newAddress);
		break;

		// Products
	case 3:
		cout << "Current List of Products:" << endl; //Implementar exceção para produtos

		for (auto & prod : restaurants.at(restaurantChoice)->get_products()) {
			cout << prod->get_name() << " : " << prod->get_cuisine_type() << " : " << prod->get_price() << endl;
		}

		cout << endl;
		cout << "Updated List of Products (Name : Type : Price), type 'done' to stop input" << endl;

		do {
			do {
				invalidProduct = false;

				getline(cin, strProduct);

				//if (cin.eof()) {
				//	cin.clear();
				//	return;
				//}

				if (strProduct == "done") {
					notEnded = false;
					break;
				}

				try {
					bool teste = product.parse(strProduct);
					if (!teste)
						throw InvalidProductException(strProduct);
					Product * productPtr = new Product;
					*productPtr = product;
					productsVec.push_back(productPtr);
				}
				catch (InvalidProductException & p) {
					invalidProduct = true;
					cout << p;
					cout << "Try Again!" << endl << endl;

				}
			} while (invalidProduct);

		} while (notEnded);
		restaurants.at(restaurantChoice)->setProducts(productsVec);

		// updating cuisine types

		for (auto & prod : restaurants.at(restaurantChoice)->get_products()) {
			cuisineTypes.insert(prod->get_cuisine_type());
		}


		restaurants.at(restaurantChoice)->setCuisineTypes(cuisineTypes);

		// updating price average
		restaurants.at(restaurantChoice)->setPriceAverage();

		break;
	}

	cout << endl;
	cout << "Restaurant successfully edited! (Enter to continue)" << endl;
	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}

void Base::removeRestaurant() {

	vector<Restaurant*>::iterator it;
	bool invalidOption;
	string strChoice;
	int restaurantChoice;
	unsigned index = 0;


	do {
		index = 0;
		invalidOption = false;
		cout << "Pick the restaurant you want to delete:" << endl;
		for (it = restaurants.begin(); it != restaurants.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);

			if (strChoice == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);
			if (strChoice != "")
			{
				restaurantChoice = stoi(strChoice);

				if (InvalidOptions(restaurants.size(), restaurantChoice)) {
					throw InvalidOptionException(restaurantChoice);
				}
			}
			else
			{
				invalidOption = true;
				utils::clear_screen();
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}


		//cout << endl;

	} while (invalidOption);

	restaurantChoice--;	// not to excede the max index available

	// To free the memory
	vector<Restaurant*>::iterator eraseIt = restaurants.begin() + restaurantChoice;
	delete * eraseIt;

	// To erase from the vector
	restaurants.erase(restaurants.begin() + restaurantChoice);

	cout << "Restaurant removed successfully" << endl;
	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX,'\n');
}


void Base::addTechnician()
{
	
	Technician t;
	bool invalidName;
	string name;
	do 
	{
		try 
		{
			invalidName = false;
			cout << "Name: ";
			getline(cin, name);
			if (!isString(name))
				throw InvalidStringException(name);
		}
		catch (InvalidStringException & s)
		{
			invalidName = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalidName);

	t.set_name(name);

	bool invalidNIF;
	string strNIF;
	do
	{
		try
		{
			invalidNIF = false;
			cout << "NIF: ";
			getline(cin, strNIF);
			if (!isNumber(strNIF) || strNIF.size() != 9)
				throw InvalidNIFException(strNIF);
		}
		catch (InvalidNIFException & n)
		{
			invalidNIF = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
	} while (invalidNIF);

	t.set_NIF(stoi(strNIF));

	bool invalidAvailability;
	string strAvailability;
	do
	{
		try
		{
			invalidAvailability = false;
			cout << "Availability: ";
			getline(cin, strAvailability);
			if (!isNumber(strAvailability))
				throw InvalidNumberException(strAvailability);
		}
		catch (InvalidNumberException & n)
		{
			invalidAvailability = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}


	} while (invalidAvailability);

	t.set_availability(stoi(strAvailability));

	bool invalidMaintenance;
	string strMaintenance;
	do
	{
		try
		{
			invalidMaintenance = false;
			cout << "Number of Maintenance: ";
			getline(cin, strMaintenance);
			if (!isNumber(strMaintenance))
				throw InvalidNumberException(strMaintenance);
		}
		catch (InvalidNumberException & n)
		{
			invalidMaintenance = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
	} while (invalidMaintenance);

	t.set_maintenance(stoi(strMaintenance));

	technicians.push(t);
	
}


void Base::changeTechnician()
{
	
	vector<Technician> temp;
	while (!technicians.empty())
	{
		temp.push_back(technicians.top());
		technicians.pop();
	}
	bool invalidOption;
	string opt;
	int index;
	do
	{
		invalidOption = false;
		cout << "Pick the technician you want to change information about: " << endl;
		for (int i = 0; i < temp.size(); i++)
		{
			cout << i + 1 << ". " << temp[i].get_name() << endl;
		}
		try
		{
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, opt);
			if (opt == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(opt))
				throw InvalidNumberException(opt);
			if (opt != "")
			{
				index = stoi(opt);
				if (InvalidOptions(temp.size(), index))
					throw InvalidOptionException(index);
			}
			else
			{
				invalidOption = true;
				utils::clear_screen();
			}

		}
		catch (InvalidOptionException & o)
		{
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & n)
		{
			invalidOption = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
	} while (invalidOption);

	index--;
	int option;
	do
	{
		try
		{
			cout << "Pick the field you want to change information of:" << endl;
			cout << "1. Name" << endl;
			cout << "2. NIF" << endl;
			cout << "3. Availability" << endl;
			cout << "4. Maintenance" << endl;
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, opt);
			invalidOption = false;
			if (opt == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(opt))
				throw InvalidNumberException(opt);
			if (opt != "")
			{
				option = stoi(opt);
				if (InvalidOptions(4, option))
					throw InvalidOptionException(option);
			}
			else
			{
				invalidOption = true;
				utils::clear_screen();
			}


		}
		catch (InvalidNumberException & n)
		{
			invalidOption = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidOptionException & o)
		{
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalidOption);

	utils::clear_screen();

	if (option == 1)
	{
		bool invalidName;
		string name;
		do
		{
			try
			{
				invalidName = false;
				cout << "Current Name: " << temp[index].get_name() << endl;
				cout << "Update Name: "<< endl;
				getline(cin, name);
				if (!isString(name))
					throw InvalidStringException(name);

			}
			catch (InvalidStringException & s)
			{
				invalidName = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

		} while (invalidName);

		temp[index].set_name(name);
	}
	else if (option == 2)
	{
		bool invalidNIF;
		string nif;
		do
		{
			try
			{
				invalidNIF = false;
				cout << "Current NIF: " << to_string(temp[index].get_NIF()) << endl;
				cout << "Update NIF: " << endl;
				getline(cin, nif);
				if (!isNumber(nif) || nif.size() != 9)
					throw InvalidNIFException(nif);
			}
			catch (InvalidNIFException & n)
			{
				invalidNIF = true;
				cout << n;
				cout << "Try Again!" << endl << endl;
			}


		} while (invalidNIF);
		temp[index].set_NIF(stoi(nif));
	}
	else if (option == 3)
	{
		bool invalidAvailability;
		string availability;
		do
		{
			try
			{
				invalidAvailability = false;
				cout << "Current Availability: " << to_string(temp[index].get_availability()) << endl;
				cout << "Update Availability: " << endl;
				getline(cin, availability);
				if (!isNumber(availability))
					throw InvalidNumberException(availability);
			}
			catch (InvalidNumberException & n)
			{
				invalidAvailability = true;
				cout << n;
				cout << "Try Again!" << endl << endl;
			}

		} while (invalidAvailability);
		
		temp[index].set_availability(stoi(availability));
	}
	else if (option == 4)
	{
		bool invalidMaintenance;
		string maintenance;
		do
		{
			try
			{
				invalidMaintenance = false;
				cout << "Current Maintenance: " << to_string(temp[index].get_maintenance()) << endl;
				cout << "Update Maintenance: " << endl;
				getline(cin, maintenance);
				if (!isNumber(maintenance))
					throw InvalidNumberException(maintenance);
			}
			catch (InvalidNumberException & n)
			{
				invalidMaintenance = true;
				cout << n;
				cout << "Try Again!" << endl << endl;
			}
		} while (invalidMaintenance);

		temp[index].set_maintenance(stoi(maintenance));
	}

	for (int i = 0; i < temp.size(); i++)
	{
		technicians.push(temp[i]);
	}
}


void Base::removeTechnician()
{
	vector<Technician> temp;
	while (!technicians.empty())
	{
		temp.push_back(technicians.top());
		technicians.pop();
	}
	bool invalidOption;
	string opt;
	int index;
	do
	{
		invalidOption = false;
		cout << "Pick the technician you want to delete: " << endl;
		for (int i = 0; i < temp.size(); i++)
		{
			cout << i + 1 << ". " << temp[i].get_name() << endl;
		}
		try
		{
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, opt);
			if (opt == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(opt))
				throw InvalidNumberException(opt);
			if (opt != "")
			{
				index = stoi(opt);
				if (InvalidOptions(temp.size(), index))
					throw InvalidOptionException(index);
			}
			else
			{
				invalidOption = true;
				utils::clear_screen();
			}

		}
		catch (InvalidOptionException & o)
		{
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & n)
		{
			invalidOption = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
	} while (invalidOption);

	index--;
	for (int i = 0; i < temp.size(); i++)
	{
		if (i != index)
			technicians.push(temp[i]);
	}

}


bool clientByName(const Client * left, const Client * right) {
	return left->get_name() < right->get_name();
}

bool clientByNIF(const Client * left, const Client * right) {
	return left->get_NIF() < right->get_NIF();
}

bool Base::orderClients()
{
	string input;

	cout << "What parameter do you want the clients to be sorted by? " << endl;
	cout << "1. Name" << endl;
	cout << "2. NIF" << endl;
	cout << "0. Go Back" << endl;
	cout << ">> ";

	getline(cin, input);

	if (input == "1") sort(clients.begin(), clients.end(), clientByName);
	else if (input == "2") sort(clients.begin(), clients.end(), clientByNIF);
	else return false;

	return true;
}

bool restaurantByName(const Restaurant * left, const Restaurant * right) {
	return left->get_name() < right->get_name();
}

bool restaurantByPrice(const Restaurant * left, const Restaurant * right) {
	return left->get_price_average() < right->get_price_average();
}

bool Base::orderRestaurants()
{
	string input;

	cout << "What parameter do you want the restaurants to be sorted by? " << endl;
	cout << "1. Name" << endl;
	cout << "2. Price" << endl;
	cout << ">> ";

	getline(cin, input);

	if (input == "1") sort(restaurants.begin(), restaurants.end(), restaurantByName);
	else if (input == "2") sort(restaurants.begin(), restaurants.end(), restaurantByPrice);
	else return false;

	return true;
}

bool workerByName(const Worker * left, const Worker * right) {
	return left->get_name() < right->get_name();
}

bool workerByNIF(const Worker * left, const Worker * right) {
	return left->get_NIF() < right->get_NIF();
}

void Base::sortDisplayWorkers() {

	// Save "Original" Unordered Sets
	unordered_set<Admin*, hashAdmin, eqAdmin> savedAdmins = admins;
	unordered_set<Delivery*, hashDeliv, eqDeliv> savedDeliveryPeople = deliveryPeople;

	bool retry = true;
	int answer;
	string input;

	// Sorting Options
	do {
		try {
			utils::clear_screen();
			retry = false;

			cout << "What parameter do you want the workers to be sorted by? " << endl;
			cout << "1. Name" << endl;
			cout << "2. NIF" << endl;
			cout << "3. None" << endl;
			cout << ">> ";

			getline(cin, input);

			if (input == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(input)) throw InvalidNumberException(input);

			if (input != "") {
				if (InvalidOptions(3, stoi(input))) throw InvalidOptionException(stoi(input));
				answer = stoi(input);
			}

			else {
				retry = true;
				utils::clear_screen();
			}

		}

		catch (InvalidOptionException & o) {
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}

		catch (InvalidNumberException & s) {
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

	cout << endl;

	vector<Admin*> adminsVector;
	copy(admins.begin(), admins.end(), back_inserter(adminsVector));

	vector<Delivery*> delivPeopleVector;
	copy(deliveryPeople.begin(), deliveryPeople.end(), back_inserter(delivPeopleVector));

	// Sorting
	switch (answer) {
		// Name
		case 1:
			sort(adminsVector.begin(), adminsVector.end(), workerByName);
			sort(delivPeopleVector.begin(), delivPeopleVector.end(), workerByName);
			break;

		// Nif
		case 2:
			sort(adminsVector.begin(), adminsVector.end(), workerByNIF);
			sort(delivPeopleVector.begin(), delivPeopleVector.end(), workerByNIF);
			break;
		
		// None
		case 3:
			break;

	}
	

	// Convert admins' Unordered Set to Vector
	/*
	unordered_set<Admin*, hashAdmin, eqAdmin> sortedAdmins;

	for (auto & element1 : adminsVector) {
		sortedAdmins.insert(element1);
	}
	*/

	// Convert delivery People's Unordered Set to Vector
	/*
	unordered_set<Delivery*, hashDeliv, eqDeliv> sortedDeliveryPeople;

	for (auto & element2 : delivPeopleVector) {
		sortedDeliveryPeople.insert(element2);
	}
	*/

	// Set the Unordered Sets to the Sorted Ones
	/*
	admins = sortedAdmins;
	deliveryPeople = sortedDeliveryPeople;
	this->setAdmins(sortedAdmins);
	this->setDeliveryPeople(sortedDeliveryPeople);
	*/

	// Display Information
	seeAllWorkers(true, adminsVector, delivPeopleVector);

	// Set the Unordered Sets to the "Original" Ones (Probably Unnecessary)
	/*
	admins = savedAdmins;
	deliveryPeople = savedDeliveryPeople;
	this->setAdmins(savedAdmins);
	this->setDeliveryPeople(savedDeliveryPeople);
	*/

	cout << ">> ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');

}

/*
bool orderByPrice(const pair<int,Order*> & left, const pair<int, Order*> & right) {
	double priceLeft, priceRight;

	for (auto p : left.second->getProducts()) priceLeft += p->get_price();
	priceLeft += left.second->getDeliveryFee();

	for (auto q : right.second->getProducts()) priceRight += q->get_price();
	priceRight += right.second->getDeliveryFee();

	return priceLeft < priceRight;
}
*/

/*
bool orderByFee(const pair<int, Order*> & left, const pair<int, Order*> & right) {
	return left.second->getDeliveryFee() < right.second->getDeliveryFee();
}
*/

bool Base::orderOrders()
{
	string input;

	cout << "What parameter do you want the orders to be sorted by? " << endl;
	cout << "1. ID" << endl;
	cout << "2. Total price" << endl;
	cout << "3. Delivery fee" << endl;
	cout << ">> ";

	getline(cin, input);

	//if (input == "1");
	// if (input == "2") sort(orders.begin(), orders.end(), orderByPrice);
	// if (input == "3") sort(orders.begin(), orders.end(), orderByFee);
	//else false;

	return true;
}


void Base::searchForRestaurant()
{
	string name;
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			cout << "Which Restaurant do you want?" << endl;
			getline(cin, name);
			bool notFound = true;
			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}
			//sort(restaurants.begin(), restaurants.end(), sortRule<Restaurant *>);
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++) // Sequencial Search
			{
				if ((*it)->get_name() == name)
				{
					notFound = false;
					cout << "Products Available" << endl;
					vector<Product*> vec = (*it)->get_products();
					vector<Product*>::iterator ite;
					for (ite = vec.begin(); ite != vec.end(); ite++)
					{
						cout << *(*ite);
						cout << endl;
					}
					cout << endl;
				}
			}
			if (notFound)
				throw RestaurantNotFoundException(name);
		}
		catch (RestaurantNotFoundException & r)
		{
			retry = true;
			cout << r;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

}

void Base::searchForGeographicArea()
{
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			string city;
			cout << "Which City do you want?" << endl;
			getline(cin, city);
			bool notFound = true;
			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++) // Sequencial Search
			{
				if ((*it)->get_address().get_town() == city)
				{
					notFound = false;
					cout << "Products Available" << endl << endl;
					cout << (*it)->get_name() << ": " << endl;
					vector<Product*> vec = (*it)->get_products();
					vector<Product*>::iterator ite;
					for (ite = vec.begin(); ite != vec.end(); ite++)
					{
						cout << *(*ite);
						cout << endl;
					}
					cout << endl;
				}
			}
			if (notFound)
				throw GeographicAreaNotFoundException(city);
		}
		catch (GeographicAreaNotFoundException & g)
		{
			retry = true;
			cout << g;
			cout << "Try Again!" << endl << endl;
		}
	} while (retry);

}

void Base::searchForPriceRange() // o price range não tá a ter casas decimais
{
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			string min, max;
			double min_num, max_num;
			cout << "Which Price Range do you want?" << endl;
			cout << "Min: ";
			getline(cin, min);
			if (!isNumber(min))
				throw InvalidNumberException(min);
			cout << "Max: ";
			getline(cin, max);
			if (!isNumber(max))
				throw InvalidNumberException(max);
			min_num = stod(min);
			max_num = stod(max);
			if (max_num < min_num)
				throw InvalidPriceRangeException(min_num, max_num);
			cout << endl << "Products" << endl;
			bool none = true;
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++)
			{
				double price = (*it)->get_price_average();
				if (price >= min_num && price <= max_num)
				{
					none = false;
					cout << (*it)->get_name() << endl;
					vector<Product*> vec = (*it)->get_products();
					vector<Product*>::iterator ite;
					for (ite = vec.begin(); ite != vec.end(); ite++)
					{
						cout << *(*ite);
						cout << endl;
					}
				}
			}
			if (none)
				cout << "None" << endl;
		}
		catch (InvalidNumberException & n)
		{
			retry = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidPriceRangeException & p)
		{
			retry = true;
			cout << p;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

}

void Base::searchForCuisineTypes()
{
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			string type;
			cout << "Which Cuisine Type do you want?" << endl;
			getline(cin, type);
			bool notFound = true;
			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++)
			{
				vector<Product*> vec = (*it)->get_products();
				vector<Product*>::iterator ite;
				int count = 0;
				for (ite = vec.begin(); ite != vec.end(); ite++)
				{
					if ((*ite)->get_cuisine_type() == type)
					{
						notFound = false;
						if (!count)
						{
							cout << (*it)->get_name() << ": " << endl;
							count++;
						}
						cout << *(*ite);
						cout << endl;
					}

				}

			}
			if (notFound)
				throw CuisineTypeNotFoundException(type);
		}
		catch (CuisineTypeNotFoundException & c)
		{
			retry = true;
			cout << c;
			cout << "Try Again!" << endl << endl;
		}

	} while (retry);

}

Delivery * Base::getDeliveryMan() {

	if (deliveryPeople.size() == 0) return nullptr;

	unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator delivIt = deliveryPeople.begin();

	Delivery * minDeliv = (*delivIt);

	while (delivIt != deliveryPeople.end()) {
		
		if ((*delivIt)->get_history().size() < ((*minDeliv).get_history().size())) {
			minDeliv = (*delivIt);
		}

		delivIt++;

	}

	return minDeliv;
}

void Base::writeRestaurantsFile(string fileName) {

	ofstream restFileInput(fileName);

	try
	{
		if (restFileInput.fail())
			throw FileOpenErrorException(fileName);
	}
	catch (FileOpenErrorException & f)
	{
		cout << f;
		exit(0);
	}




	bool first = true;

	for (auto & restaurant : restaurants) {

		if (first) {
			first = false;
		}

		else {
			restFileInput << ";;;" << endl;
		}

		restFileInput << restaurant->get_name() << endl;
		restFileInput << restaurant->get_address() << endl;

		for (auto & product : restaurant->get_products()) {
			restFileInput << product->get_name();
			restFileInput << " : ";
			restFileInput << product->get_cuisine_type();
			restFileInput << " : ";
			restFileInput << product->get_price() << endl;
		}
	}
	restFileInput.close();
}

void Base::writeDeliveriesFile(string fileName) {

	ofstream deliveriesFileInput(fileName);



	bool firstOrder = true;
	bool firstProd = true;

	for (auto & order : orders) {
		if (firstOrder) {
			firstOrder = false;
		}
		else {
			deliveriesFileInput << "\n;;;" << endl;
		}
		deliveriesFileInput << order.second->getID() << endl;
		deliveriesFileInput << order.second->getRestaurant()->get_name() << endl;
		deliveriesFileInput << order.second->getDeliveryFee() << endl;
		deliveriesFileInput << order.second->getDeliver()->getInsuccessMessage() << endl; // missing reason 
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getDay() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getMonth() << ":";
		deliveriesFileInput << order.second->getDateTime().getYear() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getHours() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getMinutes() << endl;

		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getDay() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getMonth() << ":";
		deliveriesFileInput << order.second->getDeliver()->getDateTime().getYear() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getHours() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getMinutes() << endl;

		firstProd = true;

		for (auto & prod : order.second->getProducts()) {
			if (firstProd) {
				firstProd = false;
			}
			else {
				deliveriesFileInput << endl;
			}
			deliveriesFileInput << prod->get_name();
			deliveriesFileInput << " : ";
			deliveriesFileInput << prod->get_cuisine_type();
			deliveriesFileInput << " : ";
			deliveriesFileInput << prod->get_price();
		}
	}
	deliveriesFileInput.close();
}

void Base::writeWorkersFile(string fileName) {

	ofstream workersFileInput(fileName);

	try {
		if (workersFileInput.fail()) throw FileOpenErrorException(fileName);
	}

	catch (FileOpenErrorException & f) {
		cout << f;
		exit(0);
	}

	bool firstAdmin = true;

	unordered_set<Admin*, hashAdmin, eqAdmin>::iterator adminIt = admins.begin();

	while (adminIt != admins.end()) {

		if (firstAdmin) firstAdmin = false;
		else workersFileInput << ";" << endl;

		workersFileInput << (*adminIt)->get_name() << endl;
		workersFileInput << (*adminIt)->get_NIF() << endl;

		workersFileInput << (*adminIt)->get_birthday().getDay();
		workersFileInput << ":";
		workersFileInput << (*adminIt)->get_birthday().getMonth();
		workersFileInput << ":";
		workersFileInput << (*adminIt)->get_birthday().getYear();

		workersFileInput << endl;

		workersFileInput << (*adminIt)->get_wage() << endl;
		workersFileInput << (*adminIt)->get_role() << endl;
		workersFileInput << (*adminIt)->get_state() << endl;
		adminIt++;
	}

	workersFileInput << ";;;" << endl;

	bool firstDeliv = true;
	bool firstOrder = true;

	unordered_set<Delivery*, hashDeliv, eqDeliv>::iterator delivIt = deliveryPeople.begin();

	while (delivIt != deliveryPeople.end()) {
		
		if (firstDeliv) firstDeliv = false;
		else workersFileInput << ";" << endl;

		workersFileInput << (*delivIt)->get_name() << endl;
		workersFileInput << (*delivIt)->get_NIF() << endl;

		workersFileInput << (*delivIt)->get_birthday().getDay();
		workersFileInput << ":";
		workersFileInput << (*delivIt)->get_birthday().getMonth();
		workersFileInput << ":";
		workersFileInput << (*delivIt)->get_birthday().getYear();

		workersFileInput << endl;

		workersFileInput << (*delivIt)->get_vehicle().get_brand();
		workersFileInput << " ; ";
		workersFileInput << (*delivIt)->get_vehicle().get_type();
		workersFileInput << " ; ";
		workersFileInput << (*delivIt)->get_vehicle().get_registration_date().getDay();
		workersFileInput << ":";
		workersFileInput << (*delivIt)->get_vehicle().get_registration_date().getMonth();
		workersFileInput << ":";
		workersFileInput << (*delivIt)->get_vehicle().get_registration_date().getYear();
		workersFileInput << " ; ";
		workersFileInput << (*delivIt)->get_vehicle().get_license();
		workersFileInput << " ; ";
		workersFileInput << (*delivIt)->get_vehicle().get_trips();
		workersFileInput << " ; ";
		workersFileInput << (*delivIt)->get_vehicle().get_mileage();
		
		workersFileInput << endl;

		if ((*delivIt)->get_history().size() == 0) workersFileInput << "-";

		else {

			firstOrder = true;

			for (auto & order : (*delivIt)->get_history()) {
				if (firstOrder) firstOrder = false;
				
				else workersFileInput << " : ";

				workersFileInput << order.second->getID();
			}
		}

		workersFileInput << endl;

		workersFileInput << (*delivIt)->get_state() << endl;

		delivIt++;
	}

	workersFileInput.close();
}

void Base::writeClientsFile(string fileName) {

	ofstream clientsFileInput(fileName);

	try
	{
		if (clientsFileInput.fail())
			throw FileOpenErrorException(fileName);
	}
	catch (FileOpenErrorException & f)
	{
		cout << f;
		exit(0);
	}


	bool firstClient = true;
	bool firstOrder = true;

	for (auto & client : clients) {

		if (firstClient) {
			firstClient = false;
		}

		else {
			clientsFileInput << ";" << endl;
		}

		clientsFileInput << client->get_name() << endl;
		clientsFileInput << client->get_address() << endl;
		clientsFileInput << client->get_NIF() << endl;

		if (client->get_orders().size() == 0) {
			clientsFileInput << "-";
		}
		else {
			for (auto & order : client->get_orders()) {

				if (firstOrder) {
					firstOrder = false;
				}
				else {
					clientsFileInput << " : ";
				}

				clientsFileInput << order.second->getID();
			}
		}
		clientsFileInput << endl;
	}
	clientsFileInput.close();
}

void Base::writeTechniciansFile(string filename)
{
	ofstream techniciansFileInput(filename);
	try
	{
		if (techniciansFileInput.fail())
			throw FileOpenErrorException(filename);
	}
	catch (FileOpenErrorException & f)
	{
		cout << f;
		exit(0);
	}
	
	vector<Technician> temp;

	while (!technicians.empty())
	{
		temp.push_back(technicians.top());
		technicians.pop();
	}
	for (int i = 0; i < temp.size(); i++)
	{
		techniciansFileInput << temp[i].get_name() << endl;
		techniciansFileInput << temp[i].get_NIF() << endl;
		techniciansFileInput << temp[i].get_availability() << endl;
		techniciansFileInput << temp[i].get_maintenance() << endl;
		if (i != temp.size() - 1)
			techniciansFileInput << ";" << endl;
	}

	techniciansFileInput.close();
	for (int i = 0; i < temp.size(); i++)
	{
		technicians.push(temp[i]);
	}
}


void Base::writeAll()
{
	writeRestaurantsFile(this->getRestaurantsFileName());
	writeDeliveriesFile(this->deliveriesFileName);
	writeWorkersFile(this->workersFileName);
	writeClientsFile(this->clientsFileName);
	writeTechniciansFile(this->technicianFileName);

	// PlaySound(TEXT("MicrosoftWindowsXPShutdownSound.wav"), NULL, SND_FILENAME | SND_ASYNC);
	// Sleep(3000);
	exit(0);
}


void Base::vehicle_maintenance()
{
	bool invalidNumber;
	string input;
	do
	{
		try
		{
			invalidNumber = false;
			cout << "Insert the minimum maintenances required: " << endl;
			cout << ">> ";
			getline(cin, input);
			if (!isNumber(input))
				throw InvalidNumberException(input);

		}
		catch (InvalidNumberException & n)
		{
			invalidNumber = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}

	} while (invalidNumber);

	vector<Technician> temp;
	while (!technicians.empty())
	{
		temp.push_back(technicians.top());
		technicians.pop();
		
	}

	bool found = false;

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i].get_maintenance() > stoi(input))
		{
			found = true;
			string name = temp[i].get_name();
			int hours = temp[i].get_availability();
			temp[i].set_availability(hours + 4);
			cout << name << " will maintain your vehicle." << endl;
			cout << name<< " will be available in " << hours << " hours." << endl;
			break;

		}
	}
	if (!found)
	{
		cout << "There's no technician with more than " << input << " maintenances." << endl;

	}
	for (int i = 0; i < temp.size(); i++)
	{
		technicians.push(temp[i]);
	}

}