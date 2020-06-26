#include <iostream>
#include <mysql_connection.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>
#include <limits>
#include "Books.h"
#include "Database.h"

//#define NAMESPACES using namespace boost::posix_time;using namespace boost::gregorian;
using namespace std;
using namespace sql;

//NAMESPACES;

void printMenu() {
	cout << endl;
	cout << "Enter the number of the action you wish to take" << endl;
	cout << "1. Show all available books in inventory" << endl;
	cout << "2. Show all open orders" << endl;
	cout << "3. Show all customers who made a purchase" << endl;
	cout << "4. Show all suppliers" << endl;
	cout << "5. Show all purchases between given dates" << endl;
	cout << "6. Show all books available for global sale" << endl;
	cout << "7. Check if a given book is available in the inventory" << endl;
	cout << "8. Show all suppliers of a given book" << endl;
	cout << "9. How many books from a certain type where sold from since a given date" << endl;
	cout << "10. How many books and different book types were purchased by a certain customer since given date" << endl;
	cout << "11. Show the details of the customer who bought the most since a given date" << endl;
	cout << "12. Show the details of the supplier who sold us the most books since a given date" << endl;
	cout << "13. Amount of orders made and how many books ordered between given dates" << endl;
	cout << "14. Amount of orders made and how many books ordered between given dates that were made by customers and were sold" << endl;
	cout << "15. Show the total discount a customer received since a certain date" << endl;
	cout << "16. Sum revenue in Q1, Q2, Q3, Q4 in a given year" << endl;
	cout << "17. Show how many customers were added since a given date" << endl;
	cout << "18. Total amount paid to a given supplier between given dates" << endl;
	cout << "19. Total amount a given seller earned between given dates" << endl;
	cout << "20. Show the top 10 most sold books between given dates" << endl;
	cout << "99. Exit" << endl;
	cout << endl;
}

void waitToContinue() {
	cout << '\n' << "Press any key to continue" << '\n';
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
}

int main(int argc, const char* argv[]) {
	string sDate , fDate, title, author;
	unsigned int id;
	int userChoise, cust_id, order_id;

	while (1) {
		printMenu();
		cin >> userChoise;
		switch (userChoise) {
		case 1:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert the name of the book your'e looking for" << '\n';
			getline(cin, title);
			showStorage(title);
			break;

		case 2:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			theOldestCustomer();
			break;

		case 3:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			theOldestBookInStore();
			break;

		case 4:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			orderList();
			break;

		case 5:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert the name of the book" << '\n';
			getline(cin, title);
			orderList(title);
			break;

		case 6:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert start date" << '\n';
			getline(cin, sDate);
			cout << "please insert final date" << '\n';
			getline(cin, fDate);
			mostPopularAuthor(sDate, fDate);
			break;

		case 7:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			topThreeCustomers();
			break;

		/*case 8:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			to complete();
			break;*/

		case 9:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert customer's id" << '\n';
			cin >> cust_id;
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			customerSalesHistory(cust_id);
			break;

		case 10:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert customer's id" << '\n';
			cin >> cust_id;
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			customerOrdersHistory(cust_id);
			break;

		case 11:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert order's id" << '\n';
			cin >> order_id;
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			shippingPrice(order_id);
			break;

		/*case 12:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			to complete();
			break;*/

		case 13:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert order's id" << '\n';
			cin >> order_id;
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			specificOrderStatus(order_id);
			break;

		case 14:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert year" << '\n';
			getline(cin, sDate);
			cout << "please insert month" << '\n';
			getline(cin, fDate);
			sumAmountInSpecificMonth(sDate, fDate);
			break;

		case 15:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert year" << '\n';
			getline(cin, sDate);
			cout << "please insert month" << '\n';
			getline(cin, fDate);
			sumAmountPayedInBit(sDate, fDate);
			break;

		case 99:
			return 1;

		default:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Not a valid input" << endl;
			break;
		}
		waitToContinue();
	}
}
