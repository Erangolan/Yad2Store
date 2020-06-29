#include "Books.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include "VariadicTable.h"
#include <sstream> 

using namespace std;
using namespace sql;


bool DateValidator(string date) {
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
			"order_id, customers.cust_fname, customers.cust_lname, orders.book_id, title, order_date, method_options.meth_type, shipping_options.ship_type "
			"FROM yad_two_store.orders "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = orders.book_id "
			"LEFT JOIN yad_two_store.customers ON customers.cust_id = orders.cust_id "
			"LEFT JOIN yad_two_store.method_options ON method_options.meth_id = orders.meth_id "
			"LEFT JOIN yad_two_store.shipping_options ON shipping_options.ship_id = orders.ship_id "
			"WHERE status_id != 4 "
			"GROUP BY order_date "
			"ORDER BY order_date ASC; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "All orders already arrived!" << endl;
		else {
			VariadicTable<int, string, string, int, string, string, string, string> 
				vt({ "order ID", "cust Fname", "cust Lname", "book ID", "Title", "Order Date", "payed by", "shipp by" });
			
			while (rset->next()) {
				vt.addRow(rset->getInt("order_id"), rset->getString("cust_fname"), rset->getString("cust_lname"),
						  rset->getInt("book_id"), rset->getString("title"), rset->getString("order_date"),
						  rset->getString("meth_type"), rset->getString("ship_type"));
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


/*========8========*/
// the book with the biggest amount of translations
int mostTranslateBook() {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"title, author, translate_number "
			"FROM yad_two_store.book_attributes "
			"ORDER BY translate_number DESC "
			"LIMIT 1 "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "error..." << endl;
		else {
			rset->next();
			cout << endl;
			cout << "The book with the biggest amount of translations is: " << rset->getString("title") << ", by: " << rset->getString("author")
				<< ". and was translated to " << rset->getString("translate_number") << " languages." << '\n';
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


/*========9========*/
// customer's history
int customerSalesHistory(int cust_id) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"trans_id, book_attributes.book_id, cust_id, trans_date, books_num, title, author "
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
			VariadicTable<int, int, int, string, int, string, string> 
				vt({ "Transaction ID", "Book ID", "Customer ID", "Transaction Date",
					 "Number of books", "Title", "Author" });
			while (rset->next()) {
				vt.addRow(rset->getInt("trans_id"), rset->getInt("book_id"), rset->getInt("cust_id"), rset->getString("trans_date"),
						  rset->getInt("books_num"), rset->getString("title"), rset->getString("author"));
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
			cout << endl << "This customer never invite something" << endl;
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
			"SELECT books_num, cust_price, ship_cost, book_weight, SUM(books_num * cust_price + ship_cost + book_weight) AS sum_price "
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
				vt.addRow(rset->getInt("books_num"), rset->getInt("cust_price"), rset->getInt("ship_cost"), rset->getInt("book_weight"), rset->getInt("sum_price"));
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



/*========12========*/
// split shipp
int splittingShippOrder(int cust_id) {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"order_id, book_id, cust_fname, order_date, meth_type, books_num, cust_price, dest_type, "
			"price_without_ship, status_type, ship_cost, (price_without_ship + ship_cost)as total_amount_topay "
			"FROM ( "
				"SELECT "
				"book_prices.book_id, order_id, cust_fname, order_date, meth_id, ship_id, "
				"status_id, books_num, dest_id1, book_prices.cust_price, "
				"(books_num * book_prices.cust_price) as price_without_ship "
				"FROM( "
					"SELECT order_id, book_id, cust_fname, order_date, meth_id, ship_id, status_id, books_num, dest_id1 "
					"FROM( "
						"SELECT * "
						"FROM yad_two_store.orders "
						"WHERE dest_id2 IS NOT NULL AND cust_id = '" + to_string(cust_id) + "' "
					")AS bla "
					"LEFT JOIN yad_two_store.customers ON customers.cust_id = bla.cust_id "
				")AS blala "
				"LEFT JOIN yad_two_store.book_prices ON book_prices.book_id = blala.book_id "
			")AS bli "
			"LEFT JOIN yad_two_store.shipping_status ON shipping_status.status_id = bli.status_id "
			"LEFT JOIN yad_two_store.shipping_destination ON shipping_destination.dest_id = bli.dest_id1 "
			"LEFT JOIN yad_two_store.method_options ON method_options.meth_id = bli.meth_id "
			"LEFT JOIN yad_two_store.shipping_options ON shipping_options.ship_id = bli.ship_id "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "eror..." << endl;
		else {
			VariadicTable<int, int, string, string, string, int, int, string, int, string, int, int>
				vt({ "Order ID", "Book ID", "customer name", "Order Date", "method type", "num of books",
					"price", "destination", "price without shipping", "type", "ship cost", "total amount to pay" });
			while (rset->next()) {
				vt.addRow(rset->getInt("order_id"), rset->getInt("book_id"), rset->getString("cust_fname"), rset->getString("order_date"),
					rset->getString("meth_type"), rset->getInt("books_num"), rset->getInt("cust_price"),
					rset->getString("dest_type"), rset->getInt("price_without_ship"),
					rset->getString("status_type"), rset->getInt("ship_cost"), rset->getInt("total_amount_topay"));
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
		PreparedStatement* pstmt = con->prepareStatement("SELECT SUM(books_num * cust_price + 30 + book_weight) AS sum_price "
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
		PreparedStatement* pstmt = con->prepareStatement("SELECT sum(books_num * cust_price) AS sum_price "
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



/*--------------------to complete 16!!-------------*/
/*--------------------to complete 16!!-------------*/
/*--------------------to complete 16!!-------------*/
/*--------------------to complete 16!!-------------*/
/*--------------------to complete 16!!-------------*/


/*========17========*/
// amount of shipping by Israel Post and amount of shipping by xpress at the last 12 months 
int sumAmountShipping() {
	Database& db = Database::getInstance();

	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT SUM(ship_id = 5) AS xprss, SUM(ship_id = 1) AS isrl "
			"FROM yad_two_store.orders "
			"WHERE (order_date BETWEEN '2019-09-09' AND '2020-09-09') AND (ship_id = 5 OR ship_id = 1); "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "Israel Post and xpress didn't shipp at the last year" << '\n';
		else {
			rset->next();
			cout << '\n' << "The number of ships made by Xpress at the last year is: " << rset->getInt("xprss") << '\n'
				 << "The number of ships made by Israel post at the last year is: " << rset->getInt("isrl") <<  '\n';
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


/*========18========*/
// same shipp different editions of the same book
int shippDetailsBook() {
	Database& db = Database::getInstance();
	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("CREATE TEMPORARY TABLE yad_two_store.tablee "
			"SELECT order_id, group_concat(Distinct c.book_id SEPARATOR ',') AS books_ids, order_date, "
			"GROUP_CONCAT(Distinct cust_fname SEPARATOR ',') AS cust_fname, "
			"GROUP_CONCAT(Distinct cust_lname SEPARATOR ',') AS cust_lname, "
			"CONCAT(cust_fname, ' ', cust_lname) AS full_name, "
			"GROUP_CONCAT( books_num SEPARATOR ',') AS books_numbers, "
			"GROUP_CONCAT(Distinct e.status_type SEPARATOR ',') AS status_type, "
			"GROUP_CONCAT(Distinct f.ship_cost SEPARATOR ',') AS ship_cost, "
			"GROUP_CONCAT(Distinct dest_id1 SEPARATOR ',') AS dest_id1, "
			"GROUP_CONCAT(Distinct title SEPARATOR ',') AS titless, "
			"GROUP_CONCAT(book_weight SEPARATOR ',') AS weightss, "
			"GROUP_CONCAT(cust_price SEPARATOR ',') AS prices "
			"FROM ( "
				"SELECT distinct a.order_id, a.book_id, a.order_date, a.books_num, a.status_id, a.ship_id, a.dest_id1, a.cust_id "
				"from yad_two_store.orders a, yad_two_store.orders b "
				"where a.order_id = b.order_id AND a.book_id != b.book_id "
				"GROUP BY a.book_id "
			")AS bla "
			"INNER JOIN yad_two_store.book_attributes c ON bla.book_id = c.book_id "
			"INNER JOIN yad_two_store.book_prices d ON bla.book_id = d.book_id "
			"INNER JOIN yad_two_store.shipping_status e ON bla.status_id = e.status_id "
			"INNER JOIN yad_two_store.shipping_options f ON bla.ship_id = f.ship_id "
			"INNER JOIN yad_two_store.customers g ON bla.cust_id = g.cust_id "
			"group by order_id "
		);
		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();


		pstmt = con->prepareStatement("SELECT * "
			"FROM yad_two_store.tablee "
			"where (CHAR_LENGTH(books_ids) - CHAR_LENGTH(REPLACE(books_ids, ',', '')) + 1) != (CHAR_LENGTH(titless) - CHAR_LENGTH(REPLACE(titless, ',', '')) + 1); "
		);
		rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "there wasn't shipps like this" << endl;
		else {
			VariadicTable<int, string, string, string, string, string, string, string, string, string, string>
				vt({ "order_id", "books_ids", "order_date", "full_name", "books_numbers", "status_type", "ship_cost", "dest_id1", "titless", "weightss", "prices" });
			while (rset->next()) {
				vt.addRow(rset->getInt("order_id"), rset->getString("books_ids"), rset->getString("order_date"), rset->getString("full_name"),
						  rset->getString("books_numbers"), rset->getString("status_type"),
						  rset->getString("ship_cost"), rset->getString("dest_id1"), rset->getString("titless"),
						  rset->getString("weightss"), rset->getString("prices"));
			}
			vt.print(cout);
		}

		pstmt = con->prepareStatement("DROP TEMPORARY TABLE yad_two_store.tablee; ");
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




/*========19========*/
// customers didn't buy at the last 24 months 
int oldCustomers() {
	Database& db = Database::getInstance();

	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"customers.cust_id, customers.cust_fname, customers.cust_lname, customers.phone, trans_date, books_num, title, author "
			"FROM( "
				"SELECT * "
				"FROM( "
					"SELECT * "
					"FROM yad_two_store.transactions "
					"ORDER BY trans_date DESC "
					"LIMIT 100 "
				")as bla "
				"GROUP BY cust_id "
			")as lala "
			"left join yad_two_store.customers on customers.cust_id = lala.cust_id "
			"left join yad_two_store.book_attributes on book_attributes.book_id = lala.book_id "
			"WHERE trans_date < (NOW() - INTERVAL 2 year); "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "There are no open orders" << endl;
		else {
			VariadicTable<int, string, string, string, string, int, string, string> 
				vt({ "cust ID", "cust name", "last name", "phone", "last purchase", "book's number", "book's name", "written by" });
			while (rset->next()) {
				vt.addRow(rset->getInt("cust_id"), rset->getString("cust_fname"), rset->getString("cust_lname"), rset->getString("phone"),
						  rset->getString("trans_date"), rset->getInt("books_num"), rset->getString("title"), rset->getString("author"));
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




/*========20========*/
// customers didn't arrive to store to get the book they invited 
int badCustomers() {
	Database& db = Database::getInstance();

	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"customers.cust_id, customers.cust_fname, customers.cust_lname, customers.phone "
			"FROM ( "
				"SELECT * "
				"FROM yad_two_store.orders "
				"WHERE dest_id = 1 AND status_id = 3 AND order_date < (NOW() - INTERVAL 2 WEEK) "
			")AS bought "
			"LEFT JOIN yad_two_store.customers ON customers.cust_id = bought.cust_id; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "There are no open orders" << endl;
		else {
			VariadicTable<int, string, string, string> vt({ "cust ID", "first name", "last name", "phone" });
			while (rset->next()) {
				vt.addRow(rset->getInt("cust_id"), rset->getString("cust_fname"), rset->getString("cust_lname"), rset->getString("phone"));
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



/*========22========*/
// how many books the store bought between given dates
int storeBought(string sDate, string fDate) {
	Database& db = Database::getInstance();

	try {
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"SUM(books_num) AS books_num, SUM(books_num * store_price + book_weight + ship_cost) as total_price "
			"FROM yad_two_store.orders "
			"LEFT JOIN yad_two_store.book_prices ON book_prices.book_id = orders.book_id "
			"LEFT JOIN yad_two_store.book_attributes ON book_attributes.book_id = orders.book_id "
			"LEFT JOIN yad_two_store.shipping_options ON shipping_options.ship_id = orders.ship_id "
			"WHERE order_date BETWEEN '" + sDate + "' AND '" + fDate + "' AND cust_id = 100; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "yad2 didn't bought between the given dates" << '\n';
		else {
			rset->next();
			cout << '\n' << "The number of books yad2 store bought between the given dates is: " << rset->getInt("books_num") << '\n'
				<< "yad2 paid about the stuff: " << rset->getInt("total_price") << " shekels" << '\n';
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


/*========23========*/
// total profit the store made in specific month 
int storeProfit(string year, string month) {
	Database& db = Database::getInstance();

	try {	
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"SUM(cust_price - store_price) AS store_profit "
			"FROM yad_two_store.transactions "
			"LEFT JOIN yad_two_store.book_prices ON book_prices.book_id = transactions.book_id "
			"WHERE trans_date BETWEEN '" + year + "-''" + month + "-01' AND '" + year + "-''" + month + "-30'; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "the profit of yad-2 at this month is 0 shekels" << '\n';
		else {
			rset->next();
			cout << "the profit of yad2 in " << year << "-" << month << " is: " << rset->getInt("store_profit") << '\n';
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


/*========24========*/
//amount of transactions made every month
int transactionsAmountEveryMonth(string year) {
	Database& db = Database::getInstance();

	try {	
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT ( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-01-01' AND '" + year + "-01-30') "
			") AS january, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-02-01' AND '" + year + "-02-30') "
			") AS february, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-03-01' AND '" + year + "-03-30') "
			") AS march, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-04-01' AND '" + year + "-04-30') "
			") AS april, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-05-01' AND '" + year + "-05-30') "
			") AS may, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-06-01' AND '" + year + "-06-30') "
			") AS june, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-07-01' AND '" + year + "-07-30') "
			") AS jul, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-08-01' AND '" + year + "-08-30') "
			") AS aug, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-09-01' AND '" + year + "-09-30') "
			") AS sept, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-10-01' AND '" + year + "-10-30') "
			") AS auktober, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-11-01' AND '" + year + "-11-30') "
			") AS nov, "
			"( "
			"SELECT COUNT(*) "
			"FROM yad_two_store.transactions "
			"WHERE (trans_date BETWEEN '" + year + "-12-01' AND '" + year + "-12-30') "
			") AS dece "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "There are no open orders" << endl;
		else {
			VariadicTable<string, string, string, string, string, string, string, string, string, string, string, string>
				vt({ "January", "February", "March", "April", "May", "June", 
					 "July", "August", "September", "Auctober", "November", "December" });
			while (rset->next()) {
				vt.addRow(rset->getString("january"), rset->getString("february"), rset->getString("march"), rset->getString("april"),
						  rset->getString("may"), rset->getString("june"), rset->getString("jul"),
						  rset->getString("aug"), rset->getString("sept"), rset->getString("auktober"),
						  rset->getString("nov"), rset->getString("dece"));
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


/*========25========*/
// salary of employ in specific month
int employSalary(string year, string month, int emp_id) {
	Database& db = Database::getInstance();

	try {	//the sheilta is good! just to complete the rest.
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"store_employees.emp_fname, store_employees.emp_lname, sales, hours_per_month, SUM(sales * 0.1 + hours_per_month * 25) AS bruto_salary "
			"FROM ( "
				"SELECT emp_id, SUM(books_num * cust_price) as sales "
				"FROM yad_two_store.transactions "
				"LEFT JOIN yad_two_store.book_prices ON book_prices.book_id = transactions.book_id "
				"WHERE trans_date BETWEEN '" + year + "-''" + month + "-01' AND '" + year + "-''" + month + "-30' AND emp_id = '" + to_string(emp_id) + "' "
			") AS sales_bet "
			"LEFT JOIN yad_two_store.employee_salary ON employee_salary.emp_id = sales_bet.emp_id "
			"LEFT JOIN yad_two_store.store_employees ON store_employees.emp_id = sales_bet.emp_id "
			"WHERE sales_bet.emp_id = '" + to_string(emp_id) + "' AND year = '" + year + "' and months = '" + month + "'; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();

		if (rset->rowsCount() == 0)
			cout << endl << "the profit of yad-2 at this month is 0 shekels" << '\n';
		else {
			rset->next();
			cout << "The bruto salary of " << rset->getString("emp_fname") << " " << rset->getString("emp_lname")
				<< " is: " << rset->getInt("bruto_salary") << " and he sold in amount of: " << rset->getInt("sales")
				<< " shekels! and woked " << rset->getInt("hours_per_month") << " hours in this month!" << '\n';
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


/*========26========*/
// best employ in specific month (sold the most)
int bestSellerEmploy(string year, string month) {
	Database& db = Database::getInstance();

	try {	
		Connection* con = db.getConnection();
		PreparedStatement* pstmt = con->prepareStatement("SELECT "
			"store_employees.emp_id, emp_fname, emp_lname, emp_phone, transactions_number "
			"FROM ( "
				"SELECT emp_id, COUNT(*) AS transactions_number "
				"FROM yad_two_store.transactions "
				"WHERE trans_date BETWEEN '" + year + "-''" + month + "-01' AND '" + year + "-''" + month + "-30' "
				"GROUP BY emp_id "
				"ORDER BY transactions_number DESC "
				"LIMIT 2 "
			")AS best "
			"LEFT JOIN yad_two_store.store_employees ON store_employees.emp_id = best.emp_id; "
		);

		ResultSet* rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0)
			cout << endl << "There are no open orders" << endl;
		else {
			VariadicTable<int, string, string, string, int> vt({ "Employ ID", "Employ name", "Employ last name", "Employ phone", "transactions number" });
			while (rset->next()) {
				vt.addRow(rset->getInt("emp_id"), rset->getString("emp_fname"), rset->getString("emp_lname"), 
						  rset->getString("emp_phone"), rset->getInt("transactions_number"));
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
