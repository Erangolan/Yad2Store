#include <iostream>
#include <mysql_connection.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <sstream> 

#include <string>
#include <limits>
#include "Books.h"
#include "Database.h"

using namespace std;
using namespace sql;


void otionsBar() {
	cout << '\n' << "Please insert the number of the action you want" << '\n';
	cout << "1. Check if a book exist" << '\n';
	cout << "2. The oldest customer" << '\n';
	cout << "3. The oldest book" << '\n';
	cout << "4. Order list" << '\n';
	cout << "5. Number of books __ sold by yad2" << '\n';
	cout << "6. The most reader author between dates" << '\n';
	cout << "7. Top 3 customers who bought the most" << '\n';
	cout << "8. The book with the most translations" << '\n';
	cout << "9. Purch history of customer X" << '\n';
	cout << "10. Orders history of specific customer" << '\n';
	cout << "11. Shipping cost" << '\n';
	cout << "12. Splitting shipp" << '\n';
	cout << "13. Currently status of specific shipp" << '\n';
	cout << "14. Sum of shipps made by Xpress in specific month" << '\n';
	cout << "15. Sum of money transfered to yad2 by Bit App" << '\n';
	cout << "16. Last year's transactions with profit higher than average" << '\n';
	cout << "17. Number of shipps made by Israel post and Xpress at the last year" << '\n';
	cout << "18. Details about all ships that included 2 editions of the same book" << '\n';
	cout << "19. Details about customers that didn't buy at the last 24 months" << '\n';
	cout << "20. Customers orders that didn't arrive to take their books" << '\n';
	cout << "21. Last year's transactions with profit higher than avg" << '\n';
	cout << "22. Number of books yad2 bought between dates and their sum amount" << '\n';
	cout << "23. The profit of yad2 in specific month" << '\n';
	cout << "24. Average monthly cross-sectional transactions" << '\n';
	cout << "25. Bruto salary of empoleey" << '\n';
	cout << "26. Employee Z's gross salary for a particular month" << '\n';
	cout << "0. Exit" << '\n' << '\n';
}

bool dateValidator(string date) {
	if (date[4] != '-' || date[7] != '-' || date.size() != 10)
		return false;

	int year = 0, month = 0, day = 0;
	string delimiter = "-";
	string token1 = date.substr(0, 4), token2 = date.substr(date.find(delimiter) + 1, date.find(delimiter) - 2), token3 = date.substr(8, 9);
	stringstream geek1(token1), geek2(token2), geek3(token3);
	geek1 >> year, geek2 >> month, geek3 >> day;

	if (month < 0 || month > 12 || day < 0 || day > 30 || year < 0)
		return false;

	return true;
}


int main(int argc, const char* argv[]) {
	string sDate , fDate, title, author;
	int userChoise, cust_id, order_id;

	while (1) {
		otionsBar();
		cin >> userChoise;
		switch (userChoise) {
		case 1:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert the name of the book your'e looking for" << '\n';
			getline(cin, title);
			showStorage(title);
			break;

		case 2:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			theOldestCustomer();
			break;

		case 3:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			theOldestBookInStore();
			break;

		case 4:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			orderList();
			break;

		case 5:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert the name of the book" << '\n';
			getline(cin, title);
			orderList(title);
			break;

		case 6:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert start date. for example 2018-02-12" << '\n';
			getline(cin, sDate);
			cout << "please insert final date. for example 2020-02-13" << '\n';
			getline(cin, fDate);
			if (!dateValidator(sDate) || !dateValidator(fDate)) {
				cout << "invalid date or format. please insert date in format yyyy-mm-tt. for example 2020-02-12" << endl;
				break;
			}
			mostPopularAuthor(sDate, fDate);
			break;

		case 7:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			topThreeCustomers();
			break;

		case 8:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			mostTranslateBook();
			break;

		case 9:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert customer's id" << '\n';
			cin >> cust_id;
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			customerSalesHistory(cust_id);
			break;

		case 10:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert customer's id" << '\n';
			cin >> cust_id;
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			customerOrdersHistory(cust_id);
			break;

		case 11:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert order's id" << '\n';
			cin >> order_id;
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			shippingPrice(order_id);
			break;

		case 12:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert customer's id" << '\n';
			cin >> order_id;
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			splittingShippOrder(order_id);
			break;

		case 13:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert order's id" << '\n';
			cin >> order_id;
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			specificOrderStatus(order_id);
			break;

		case 14:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert year. for example 2020" << '\n';
			getline(cin, sDate);
			cout << "please insert month. for example 01" << '\n';
			getline(cin, fDate);
			sumAmountInSpecificMonth(sDate, fDate);
			break;

		case 15:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert year. for example 2020" << '\n';
			getline(cin, sDate);
			cout << "please insert month. for example 07" << '\n';
			getline(cin, fDate);
			sumAmountPayedInBit(sDate, fDate);
			break;

		case 16:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			transactionsProfitHigherThanAvg();
			break;

		case 17:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			sumAmountShipping();
			break;

			case 18:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			shippDetailsBook();
			break;

		case 19:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			oldCustomers();
			break;

		case 20:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			badCustomers();
			break;

		case 21:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "please insert year" << '\n';
			getline(cin, sDate);
			numOfBooksInStorage(sDate);
			break;

		case 22:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert start date. for example 2020-01-01" << '\n';
			getline(cin, sDate);
			cout << "please insert end date. for example 2020-07-19" << '\n';
			getline(cin, fDate);
			if (!dateValidator(sDate) || !dateValidator(fDate)) {
				cout << "invalid date or format. please insert date in format yyyy-mm-tt. for example 2020-02-12" << endl;
				break;
			}
			storeBought(sDate, fDate);
			break;

		case 23:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert year. for example 2020" << '\n';
			getline(cin, sDate);
			cout << "please insert month. for example 07" << '\n';
			getline(cin, fDate);
			storeProfit(sDate, fDate);
			break;

		case 24:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert year. for example 2018" << '\n';
			getline(cin, sDate);
			transactionsAmountEveryMonth(sDate);
			break;

		case 25:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert employ's id. for example 1" << '\n';
			cin >> cust_id;
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert year. for example 2018" << '\n';
			getline(cin, sDate);
			cout << "please insert month. for example 01" << '\n';
			getline(cin, fDate);
			employSalary(sDate, fDate, cust_id);
			break;

		case 26:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "please insert year. for example 2020" << '\n';
			getline(cin, sDate);
			cout << "please insert month. for example 07" << '\n';
			getline(cin, fDate);
			bestSellerEmploy(sDate, fDate);
			break;

		case 0:
			return 1;

		default:
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
			cout << "Invalid input" << '\n';
			break;
		}
		cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
	}
}
