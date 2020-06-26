#include "Books.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include "VariadicTable.h"
#include <sstream> 

using namespace std;
using namespace sql;


//checks if the format of a string is yyyy-mm-tt, returns true if so and false otherwise
bool validateDateFormat(string date) {
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

/*========1========*/
int showStorage(string title) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"title, author, store_items.amount AS store_amount, storage_items.amount AS storage_amount "
			"FROM yad_two_store.storage_items "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = storage_items.book_id "
			"LEFT JOIN yad_two_store.store_items ON store_items.book_id = storage_items.book_id "
			"WHERE book_attributes.title = '" + title + "'; "
		);
		
		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "The book does not exist in our storage" << endl;
		else {
			rset->next();
			cout << endl;
			cout << "The book: " << rset->getString("title") << ", by " << rset->getString("author") << ", has " 
				<< rset->getUInt("store_amount") << " copies in store and " << rset->getUInt("storage_amount") << " copies in the storage" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}

/*========2========*/
int theOldestCustomer() {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"MIN(trans_date) AS 'oldest_date', customers.cust_fname, customers.cust_lname, book_attributes.title, book_attributes.author "
			"FROM yad_two_store.transactions "
			"LEFT JOIN yad_two_store.customers ON customers.cust_id = transactions.cust_id "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = transactions.book_id; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "The book does not exist in our storage" << endl;
		else {
			rset->next();
			cout << endl;
			cout << "The Oldest customer is: " << rset->getString("cust_fname") << " " << rset->getString("cust_lname") << ", his purchase was in "
				<< rset->getString("oldest_date") << ", and he bought the book: " << rset->getString("title") << " by " << rset->getString("author") << '\n';
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*========3========*/
// The book that has the most time in stock
int theOldestBookInStore() {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"trans_date, title, author "
			"FROM ( "
				"SELECT trans_date, book_attributes.book_id, title, author "
				"FROM ( "
					"SELECT * "
					"FROM yad_two_store.transactions "
					"GROUP BY trans_date "
					"ORDER BY trans_date DESC "
				") AS boughts_in_asc_order "
				"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = boughts_in_asc_order.book_id "
				"GROUP BY book_attributes.book_id "
			") AS bla "
			"ORDER BY trans_date ASC "
			"LIMIT 1; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "The book does not exist in our storage" << endl;
		else {
			rset->next();
			cout << endl;
			cout << "The book that has the most time in stock is: " << rset->getString("title") <<
				" by " << rset->getString("author") << ", and the last purchase was it was in "
				<< rset->getString("trans_date") << '\n';
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*========4========*/
// orders still not arrived yet by priority by dates
int orderList() {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"order_id, customers.cust_fname, customers.cust_lname, orders.book_id, title, order_date, payment_amount, method_options.meth_type, shipping_options.ship_type "
			"FROM yad_two_store.orders "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = orders.book_id "
			"LEFT JOIN yad_two_store.customers ON customers.cust_id = orders.cust_id "
			"LEFT JOIN yad_two_store.method_options ON method_options.meth_id = orders.meth_id "
			"LEFT JOIN yad_two_store.shipping_options ON shipping_options.ship_id = orders.ship_id "
			"WHERE arrive = 0 "
			"GROUP BY order_date "
			"ORDER BY order_date ASC; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "The book does not exist in our storage" << endl;
		else {
			VariadicTable<int, string, string, int, string, string, int, string, string> 
				vt({ "order ID", "cust Fname", "cust Lname", "book ID", "Title", "Order Date", "payment amount", "payed by", "shipp by" });
			
			while (rset->next()) {
				vt.addRow(rset->getInt("order_id"), rset->getString("cust_fname"), rset->getString("cust_lname"),
						  rset->getInt("book_id"), rset->getString("title"), rset->getString("order_date"),
						  rset->getInt("payment_amount"), rset->getString("meth_type"), rset->getString("ship_type") );
			}
			vt.print(cout);
			
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*========5========*/
// number of copies the book sold
int orderList(string title) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT title, count(title) AS sales_num, author, book_attributes.book_id "
			"FROM yad_two_store.book_attributes "
			"LEFT JOIN yad_two_store.transactions ON transactions.book_id = book_attributes.book_id "
			"WHERE title = '" + title + "'; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "The book does not exist in our storage" << endl;
		else {
			rset->next();
			cout << endl;
			cout << "The book " << title << " by " << rset->getString("author") 
				 << ", was salled " << rset->getString("sales_num") << " times in this store" << '\n';
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*========6========*/
// the most popular author between dates
int mostPopularAuthor(string sDate, string fDate) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT author, num_of_sales "
			"FROM ( "
				"SELECT book_id, COUNT(*) AS num_of_sales  "
				"FROM yad_two_store.transactions "
				"WHERE trans_date BETWEEN '" + sDate + "' AND '" + fDate + "' "
				"GROUP BY book_id "
				"ORDER BY num_of_sales DESC "
				"LIMIT 1 "
			") as sold_between "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = sold_between.book_id; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "The book does not exist in our storage" << endl;
		else {
			rset->next();
			cout << endl;
			cout << "The most reader author between " << sDate << " and " << fDate << " is: " << rset->getString("author")
				<< ", and " << rset->getDouble("num_of_sales") << " people read his books" << '\n';
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*========7========*/
// top 3 customers (bought the most)
int topThreeCustomers() {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"transactions.cust_id, customers.cust_fname, customers.cust_lname, COUNT(transactions.cust_id) AS buy_amount "
			"FROM yad_two_store.transactions "
			"LEFT JOIN yad_two_store.customers ON customers.cust_id = transactions.cust_id "
			"GROUP BY cust_id "
			"ORDER BY buy_amount DESC "
			"LIMIT 3; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "There are no open orders" << endl;
		else {
			VariadicTable<int, string, string, int> vt({ "cust ID", "first name", "last name", "amount of books bought" });
			while (rset->next()) {
				vt.addRow(rset->getInt("cust_id"), rset->getString("cust_fname"), rset->getString("cust_lname"), rset->getInt("buy_amount"));
			}
			vt.print(cout);
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*--------------------to complete 8!!-------------*/
/*--------------------to complete 8!!-------------*/
/*--------------------to complete 8!!-------------*/
/*--------------------to complete 8!!-------------*/
/*--------------------to complete 8!!-------------*/


/*========9========*/
// customer's history
int customerSalesHistory(int cust_id) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"trans_id, book_attributes.book_id, cust_id, trans_date, payment_amount, books_num, title, author "
			"FROM ( "
				"SELECT * "
				"FROM yad_two_store.transactions "
				"WHERE cust_id = '" + to_string(cust_id) + "' "
			") AS cust_bought "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = cust_bought.book_id "
			"ORDER BY trans_date DESC; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "There are no open orders" << endl;
		else {
			VariadicTable<int, int, int, string, int, int, string, string> 
				vt({ "Transaction ID", "Book ID", "Customer ID", "Transaction Date",
					 "payment amount", "Number of books", "Title", "Author" });
			while (rset->next()) {
				vt.addRow(rset->getInt("trans_id"), rset->getInt("book_id"), rset->getInt("cust_id"), rset->getString("trans_date"),
						  rset->getInt("payment_amount"), rset->getInt("books_num"), rset->getString("title"), rset->getString("author"));
			}
			vt.print(cout);
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*========10========*/
// customer's history
int customerOrdersHistory(int cust_id) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT order_id, book_attributes.book_id, order_date, title, author "
			"FROM( "
				"SELECT order_id, store_items.book_id, amount, order_date "
				"FROM( "
					"SELECT * "
					"FROM yad_two_store.orders " 
					"WHERE cust_id = '" + to_string(cust_id) + "' "
				") AS cust_bought "
				"LEFT JOIN yad_two_store.store_items ON store_items.book_id = cust_bought.book_id "
				"WHERE amount > 0 "
			")AS in_store "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = in_store.book_id "
			"ORDER BY order_date DESC "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "There are no open orders" << endl;
		else {
			VariadicTable<int, int, string, string, string>
				vt({ "Order ID", "Book ID", "Order Date",
					 "payment amount", "Title", "Author" });
			while (rset->next()) {
				vt.addRow(rset->getInt("order_id"), rset->getInt("book_id"), 
						  rset->getString("order_date"), rset->getString("title"), rset->getString("author"));
			}
			vt.print(cout);
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*========11========*/
// temp table
int shippingPrice(int order_id) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("CREATE TEMPORARY TABLE yad_two_store.table1 "
			"SELECT books_num, price, ship_cost, book_weight, SUM(books_num * price + ship_cost + book_weight) AS sum_price "
			"FROM( "
				"SELECT order_id, orders.book_id, cust_id, order_date, meth_id, ship_id, status_id, books_num, book_weight "
				"FROM yad_two_store.orders "
				"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = orders.book_id "
				"WHERE order_id = '" + to_string(order_id) + "' "
			")AS bla "
			"LEFT JOIN yad_two_store.book_prices ON book_prices.book_id = bla.book_id "
			"LEFT JOIN yad_two_store.shipping_options ON shipping_options.ship_id = bla.ship_id; "
		);
		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();


		pstmt = con->prepareStatement("SELECT * FROM yad_two_store.table1; ");
		rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "error on calculation ship price" << endl;
		else {
			VariadicTable<int, int, int, int, int> vt({ "book's number", "book's price", "ship cost", "book's weight", "total amount to pay" });
			while (rset->next()) {
				vt.addRow(rset->getInt("books_num"), rset->getInt("price"), rset->getInt("ship_cost"), rset->getInt("book_weight"), rset->getInt("sum_price"));
			}
			vt.print(cout);
		}

		pstmt = con->prepareStatement("DROP TEMPORARY TABLE yad_two_store.table1; ");
		rset = pstmt->executeQuery();

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}


/*--------------------to complete 12!!-------------*/
/*--------------------to complete 12!!-------------*/
/*--------------------to complete 12!!-------------*/
/*--------------------to complete 12!!-------------*/
/*--------------------to complete 12!!-------------*/


/*========13========*/
// spesific order status
int specificOrderStatus(int order_id) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"order_id, book_attributes.book_id, title, customers.cust_id, "
			"customers.cust_fname, order_date, method_options.meth_type, "
			"shipping_options.ship_type, status_type "
			"FROM yad_two_store.orders "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = orders.book_id "
			"LEFT JOIN yad_two_store.shipping_status ON shipping_status.status_id = orders.status_id "
			"LEFT JOIN yad_two_store.customers ON customers.cust_id = orders.cust_id "
			"LEFT JOIN yad_two_store.method_options ON method_options.meth_id = orders.meth_id "
			"LEFT JOIN yad_two_store.shipping_options ON shipping_options.ship_id = orders.ship_id "
			"WHERE order_id = '" + to_string(order_id) + "'; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "eror..." << endl;
		else {
			VariadicTable<int, int, string, int, string, string, string, string, string>
				vt({ "Order ID", "Book ID", "Title", "customer ID", "customer name", "Order Date", "method type", "ship by", "status" });
			while (rset->next()) {
				vt.addRow(rset->getInt("order_id"), rset->getInt("book_id"), rset->getString("title"), rset->getInt("cust_id"),
					rset->getString("cust_fname"), rset->getString("order_date"), rset->getString("meth_type"),
					rset->getString("ship_type"), rset->getString("status_type"));
			}
			vt.print(cout);
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}



/*========14========*/
// sum amount of xpress in specific month
int sumAmountInSpecificMonth(string year, string month) {
	Database& db = Database::getInstance();
	
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT SUM(books_num * price + 30 + book_weight) AS sum_price "
			"FROM yad_two_store.orders "
			"LEFT JOIN yad_two_store.book_prices ON book_prices.book_id = orders.book_id "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = orders.book_id "
			"WHERE (order_date BETWEEN '" + year + "-''" + month + "-01' AND '" + year + "-''" + month + "-30') AND ship_id = 5; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "xpress didn't sent anithing in this month" << '\n';
		else {
			rset->next();
			cout << endl;
			cout << "xspress sum amount in  " << year << "-" << month << " is: " << rset->getInt("sum_price") << '\n';
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}



/*========15========*/
// sum amount payd in bit in specific month
int sumAmountPayedInBit(string year, string month) {
	Database& db = Database::getInstance();

	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT sum(books_num * price) AS sum_price "
			"FROM yad_two_store.transactions "
			"LEFT JOIN yad_two_store.book_prices ON book_prices.book_id = transactions.book_id "
			"WHERE (trans_date BETWEEN '" + year + "-''" + month + "-01' AND '" + year + "-''" + month + "-30') AND meth_id = 2; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "non'e of the customers payed in Bit at this month" << '\n';
		else {
			rset->next();
			cout << endl;
			cout << "The store sum amount payed in Bit in  " << year << "-" << month << " is: " << rset->getInt("sum_price") << '\n';
		}

		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
}
