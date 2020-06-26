#include "Database.h"

Database* Database::instance = 0;

Database::Database() : driver(get_driver_instance()) {
	connection_properties["hostName"] = DB_HOST;
	connection_properties["port"] = DB_PORT;
	connection_properties["userName"] = DB_USER;
	connection_properties["password"] = DB_PASS;
	connection_properties["OPT_RECONNECT"] = true;

	try {
		Connection* con = driver->connect(connection_properties);
		try {
			con->setSchema("yad_two_store");
		}
		catch (SQLException& e) {
			Statement* stmt = con->createStatement();
			stmt->execute("CREATE DATABASE IF NOT EXISTS yad_two_store;");
			con->setSchema("yad_two_store");
			stmt->execute("CREATE TABLE IF NOT EXISTS status_condition("
				"condition_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"condition_type VARCHAR(100)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS method_options("
				"meth_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"meth_type VARCHAR(100)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS shipping_options("
				"ship_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"ship_type VARCHAR(100),"
				"ship_cost INT"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS shipping_status("
				"status_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"status_type VARCHAR(100)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS store_employees("
				"emp_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"emp_fname VARCHAR(100),"
				"emp_lname VARCHAR(100),"
				"emp_phone VARCHAR(10)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS customers("
				"cust_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"cust_fname VARCHAR(100),"
				"cust_lname VARCHAR(100),"
				"phone VARCHAR(10)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS book_attributes("
				"book_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"title VARCHAR(100),"
				"author VARCHAR(100),"
				"translator VARCHAR(100),"
				"publisher VARCHAR(100),"
				"publishing_year VARCHAR(100),"
				"pages_number INT UNSIGNED NOT NULL,"
				"book_weight INT UNSIGNED NOT NULL,"
				"condition_id INT UNSIGNED,"
				"FOREIGN KEY(condition_id) REFERENCES status_condition(condition_id)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS store_items("
				"book_id INT UNSIGNED NOT NULL,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"amount INT UNSIGNED"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS storage_items("
				"book_id INT UNSIGNED NOT NULL,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"amount INT UNSIGNED"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS book_prices("
				"book_id INT UNSIGNED NOT NULL,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"price INT"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS transactions("
				"trans_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"book_id INT UNSIGNED,"
				"cust_id INT UNSIGNED,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"FOREIGN KEY(cust_id) REFERENCES customers(cust_id),"
				"trans_date DATE,"
				"meth_id INT UNSIGNED,"
				"FOREIGN KEY(meth_id) REFERENCES method_options(meth_id),"
				"books_num INT"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS orders("
				"order_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"book_id INT UNSIGNED,"
				"cust_id INT UNSIGNED,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"FOREIGN KEY(cust_id) REFERENCES customers(cust_id),"
				"order_date DATE,"
				"meth_id INT UNSIGNED,"
				"ship_id INT UNSIGNED,"
				"status_id INT UNSIGNED,"
				"books_num INT UNSIGNED,"
				"FOREIGN KEY(meth_id) REFERENCES method_options(meth_id),"
				"FOREIGN KEY(ship_id) REFERENCES shipping_options(ship_id),"
				"FOREIGN KEY(status_id) REFERENCES shipping_status(status_id)"
				");");

			stmt->execute("INSERT INTO status_condition(condition_id, condition_type) "
				"VALUES "
				"(1, 'As new'), "
				"(2, 'Fine'), "
				"(3, 'Very good'), "
				"(4, 'Good'), "
				"(5, 'Fair'), "
				"(6, 'Poor');");
			stmt->execute("INSERT INTO method_options(meth_id, meth_type) "
				"VALUES "
				"(1, 'Credit card'), "
				"(2, 'Bit'), "
				"(3, 'Bank transfer');");
			stmt->execute("INSERT INTO shipping_options(ship_id, ship_type, ship_cost) "
				"VALUES "
				"(1, 'Israel Post', 50), "
				"(2, 'Registered mail', 40), "
				"(3, 'Courier service', 60), "
				"(4, 'Fast courier mail', 20), "
				"(5, 'Xpress messenger company', 30);");
			stmt->execute("INSERT INTO shipping_status(status_id, status_type) "
				"VALUES "
				"(1, 'In preparation for delivery'), "
				"(2, 'Sent'), "
				"(3, 'Ready! wait for pickup'), "
				"(4, 'Delivered');");
			stmt->execute("INSERT INTO store_employees(emp_id, emp_fname, emp_lname, emp_phone) "
				"VALUES "
				"(1, 'Ran', 'Ben', '0523330101'), "
				"(2, 'Eran', 'Golani', '0528777122'), "
				"(3, 'Roy', 'Eshkol', '0542277010'), "
				"(4, 'Omer', 'Eshkol', '0502837000'), "
				"(5, 'Gad', 'Goldstein', '0502837777'), "
				"(6, 'Edna', 'Levi', '0502837776'), "
				"(7, 'Ruti', 'Meron', '0502837775'), "
				"(8, 'Tamir', 'Melamed', '0508887711'), "
				"(9, 'Miri', 'Sade', '0508889911'), "
				"(10, 'Moshe', 'Atzili', '0521234561');");
			stmt->execute("INSERT INTO customers(cust_id, cust_fname, cust_lname, phone) "
				"VALUES "
				"(1, 'Eli', 'Bloch', '0523339384'), "
				"(2, 'Adi', 'Guttman', '0528772222'), "
				"(3, 'Dana', 'Alon', '0542277374'), "
				"(4, 'Sharon', 'Idan', '0502837746'), "
				"(5, 'Sharona', 'Arnon', '0502837745'), "
				"(6, 'Shirel', 'Lowis', '0502837744'), "
				"(7, 'Shauli', 'Geva', '0502837743'), "
				"(8, 'Shay', 'Arbel', '0502837742'), "
				"(9, 'Almog', 'Shabi', '0508889900'), "
				"(10, 'Guy', 'Gadi', '0521234562');");
			stmt->execute("INSERT INTO book_attributes (book_id, title, author, translator, publisher, publishing_year, pages_number, book_weight, condition_id) "
				"VALUES "
				"(1, 'The Magician', 'Eyal Berkovic', NULL, 'Carmel Publishing House', '2006', 104, 1, 3), "
				"(2, 'Rich Dad Poor Dad', 'Robert Kiyosaki', 'Sharon Lechter', 'Warner Books Ed', '2000', 207, 2, 4), "
				"(3, 'Ulysses', 'James Joyce', NULL, 'Sylvia Beach', '1922', 730, 6, 6), "
				"(4, 'Parallel Worlds', 'Michio Kaku', NULL, 'Doubleday', '2004', 428, 2, 3), "
				"(5, 'retire young retire rich', 'Robert Kiyosaki', 'Sharon Lechter', 'Warner Books Ed', '2015', 400, 3, 2), "
				"(6, 'Bibi', 'Anshel Pfeffer', NULL, 'The Hurst Publishers', '2018', 432, 3, 1), "
				"(7, 'The Little Prince', 'Antoine de Saint Exupery', 'Katherine Woods', 'Reynal and Hitchcock', '1945', 128, 1, 6), "
				"(8, 'Room for Rent', 'Leah Goldberg', NULL, 'Publishing of a workers', '1959', 56, 1, 6), "
				"(9, 'The Great Gatsby', 'Scott Fitzgerald', NULL, 'Charles Scribners Sons', '1926', 218, 2, 5), "
				"(10, 'Harry Potter and the Goblet of Fire', 'J. K. Rowling', 'Nathan Granot', 'Bloomsbury Publishing plc', '2000', 636, 3, 4), "
				"(11, 'The Sellout', 'Paul Beatty', 'Matt Buck', '	Farrar, Straus and Giroux', '2015', 304, 2, 2), "
				"(12, 'The lover', 'Nimrod Zahavi', 'Shay Barlev', 'Carmel Publishing House', '2012', 209, 2, 4), "
				"(13, 'The Glass Hotel', 'Emily St. John Mandel', 'Eliyahu Shabat', 'HarperCollins', '2020', 320, 2, 1);");
			stmt->execute("INSERT INTO store_items (book_id, amount) "
				"VALUES "
				"(1, 20), "
				"(2, 30), "
				"(3, 40), "
				"(4, 0), "
				"(5, 15), "
				"(6, 0), "
				"(7, 70), "
				"(8, 100), "
				"(9, 0), "
				"(10, 5), "
				"(11, 80), "
				"(12, 1), "
				"(13, 90);");
			stmt->execute("INSERT INTO storage_items(book_id, amount) "
				"VALUES "
				"(1, 0), "
				"(2, 0), "
				"(3, 20), "
				"(4, 30), "
				"(5, 0), "
				"(6, 100), "
				"(7, 70), "
				"(8, 40), "
				"(9, 50), "
				"(10, 10), "
				"(11, 0), "
				"(12, 10), "
				"(13, 0);");
			stmt->execute("INSERT INTO book_prices(book_id, price) "
				"VALUES "
				"(1, 20), "
				"(2, 40), "
				"(3, 30), "
				"(4, 80), "
				"(5, 100), "
				"(6, 90), "
				"(7, 70), "
				"(8, 40), "
				"(9, 60), "
				"(10, 50), "
				"(11, 40), "
				"(12, 100);");
			stmt->execute("INSERT INTO transactions (trans_id, book_id, cust_id, trans_date, meth_id, books_num) "
				"VALUES "
				"(1, 2, 1, '2015-01-01', 1, 1), "
				"(2, 10, 2, '2015-01-12', 1, 1), "
				"(3, 2, 3, '2015-03-15', 1, 2), "
				"(4, 3, 4, '2015-05-15', 1, 2), "
				"(5, 2, 1, '2015-06-25', 3, 4), "
				"(6, 11, 2,  '2015-06-27', 3, 0), "
				"(7, 7, 3,  '2016-07-04', 1, 1), "
				"(8, 11, 4,  '2016-08-12', 3, 5), "
				"(9, 10, 4,  '2016-05-22', 1, 3), "
				"(10, 4, 4,  '2016-05-30', 3, 4), "
				"(11, 2, 1,  '2016-10-07', 3, 1), "
				"(12, 10, 2,  '2016-10-17', 3, 7), "
				"(13, 6, 3, '2017-10-19', 3, 6), "
				"(14, 5, 4,  '2017-10-24', 1, 0), "
				"(15, 10, 1,  '2017-11-11', 1, 8), "
				"(16, 2, 2, '2017-11-18', 1, 2), "
				"(17, 9, 3, '2017-11-24', 1, 4), "
				"(18, 8, 4, '2017-12-02', 1, 5), "
				"(19, 10, 4, '2018-04-23', 3, 5), "
				"(20, 4, 4, '2018-06-04', 1, 7), "
				"(21, 2, 1, '2018-07-14', 3, 2), "
				"(22, 6, 2, '2018-07-21', 3, 0), "
				"(23, 2, 3, '2018-08-28', 3, 5), "
				"(24, 3, 4, '2019-10-01', 3, 9), "
				"(25, 2, 1, '2019-11-05', 2, 1), "
				"(26, 11, 2, '2019-12-30', 2, 8), "
				"(27, 4, 3, '2020-03-10', 2, 9), "
				"(28, 1, 4, '2020-03-28', 2, 1), "
				"(29, 10, 4, '2020-04-14', 2, 1), "
				"(30, 12, 4, '2020-05-14', 2, 1);");
			stmt->execute("INSERT INTO orders (order_id, book_id, cust_id, order_date, meth_id, ship_id, status_id, books_num) "
				"VALUES "
				"(1, 1, 3, '2018-05-04', 3, 5, 4, 1), "
				"(2, 4, 3, '2018-05-13', 1, 5, 4, 2), "
				"(3, 5, 3, '2018-05-19', 3, 5, 4, 4), "
				"(4, 7, 3, '2018-05-26', 2, 5, 4, 3), "
				"(5, 2, 3, '2018-10-19', 3, 3, 4, 1), "
				"(6, 4, 4,  '2018-10-24', 1, 2, 4, 2), "
				"(7, 4, 1,  '2018-11-11', 1, 4, 4, 3), "
				"(8, 3, 2, '2018-11-18', 1, 5, 4, 2), "
				"(9, 1, 3, '2018-11-24', 1, 2, 4, 2), "
				"(10, 9, 4, '2018-12-02', 1, 2, 4, 1), "
				"(11, 6, 4, '2019-04-23', 3, 4, 3, 2), "
				"(12, 6, 4, '2019-06-04', 1, 1, 2, 2), "
				"(13, 2, 1, '2019-07-14', 3, 3, 1, 3), "
				"(14, 11, 9, '2019-07-21', 3, 1, 1, 4), "
				"(15, 1, 9, '2019-08-28', 3, 1, 2, 5), "
				"(16, 2, 1, '2020-08-01', 3, 1, 2, 5), "
				"(17, 12, 7, '2020-01-05', 2, 2, 1, 6), "
				"(18, 8, 5, '2020-08-30', 2, 5, 2, 3), "
				"(19, 7, 10, '2020-03-10', 2, 4, 3, 1), "
				"(20, 11, 8, '2020-02-28', 2, 4, 2, 2), "
				"(21, 10, 6, '2020-01-14', 2, 2, 2, 4), "
				"(22, 2, 6, '2020-09-14', 2, 5, 3, 2);");

			
			delete stmt;
		}
		delete con;
	}
	catch (SQLException& e) {
		cout << e.getErrorCode() << " " << e.what() << " " << e.getSQLState();
	}
}

Database& Database::getInstance() {
	if (Database::instance == 0) {
		instance = new Database();
	}
	return *instance;
}

Connection* Database::getConnection() {
	try {
		Connection* con = driver->connect(connection_properties);
		con->setSchema(DB_NAME);
		return con;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	return 0;
}



/*catch (SQLException& e) {
			Statement* stmt = con->createStatement();
			stmt->execute("CREATE DATABASE IF NOT EXISTS yad2_store;");
			con->setSchema("yad2_store");
			
			
			
			

			
			
			
			
			stmt->execute("INSERT INTO store_payments(date, rental, property_tax, water_tax, electric_bill, telephone_line_account, cellphone_line_account, shipping_account, employee_salaries) "
				"VALUES "
				"('2015-01-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-02-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-03-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-04-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-05-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-06-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-07-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-08-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-09-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-10-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-11-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2015-12-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-01-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-02-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-03-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-04-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-05-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-06-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-07-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-08-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-09-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-10-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-11-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2016-12-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-01-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-02-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-03-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-04-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-05-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-06-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-07-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-08-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-09-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-10-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-11-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2017-12-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-01-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-02-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-03-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-04-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-05-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-06-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-07-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-08-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-09-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-10-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-11-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2018-12-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-01-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-02-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-03-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-04-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-05-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-06-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-07-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-08-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-09-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-10-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-11-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2019-12-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-01-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-02-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-03-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-04-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-05-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-06-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-07-10', 200, 80, 20, 30, 50, 50, 0, 100), "
				"('2020-08-10', 200, 80, 20, 30, 50, 50, 0, 100);");
			
			
			

			
			
			delete stmt;
		}*/