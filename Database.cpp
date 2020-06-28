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
			stmt->execute("CREATE TABLE IF NOT EXISTS shipping_destination("
				"dest_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"dest_type VARCHAR(100)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS store_employees("
				"emp_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"emp_fname VARCHAR(100),"
				"emp_lname VARCHAR(100),"
				"emp_phone VARCHAR(10),"
				"join_date DATE,"
				"departure_date DATE"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS employee_salary("
				"emp_id INT UNSIGNED NOT NULL,"
				"FOREIGN KEY(emp_id) REFERENCES store_employees(emp_id),"
				"year VARCHAR(100),"
				"months VARCHAR(100),"
				"hours_per_month INT UNSIGNED"
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
				"FOREIGN KEY(condition_id) REFERENCES status_condition(condition_id),"
				"translate_number INT"
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
				"cust_price INT,"
				"store_price INT"
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
				"books_num INT,"
				"emp_id INT UNSIGNED,"
				"FOREIGN KEY(emp_id) REFERENCES store_employees(emp_id)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS orders("
				"order_id INT UNSIGNED NOT NULL,"
				"book_id INT UNSIGNED,"
				"cust_id INT UNSIGNED,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"FOREIGN KEY(cust_id) REFERENCES customers(cust_id),"
				"order_date DATE,"
				"meth_id INT UNSIGNED,"
				"ship_id INT UNSIGNED,"
				"status_id INT UNSIGNED,"
				"books_num INT UNSIGNED,"
				"dest_id1 INT UNSIGNED,"
				"dest_id2 INT UNSIGNED,"
				"FOREIGN KEY(meth_id) REFERENCES method_options(meth_id),"
				"FOREIGN KEY(ship_id) REFERENCES shipping_options(ship_id),"
				"FOREIGN KEY(status_id) REFERENCES shipping_status(status_id),"
				"FOREIGN KEY(dest_id1) REFERENCES shipping_destination(dest_id),"
				"FOREIGN KEY(dest_id2) REFERENCES shipping_destination(dest_id)"
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
				"(5, 'Xpress messenger company', 30), "
				"(51, 'Xpress messenger company', 0);");
			stmt->execute("INSERT INTO shipping_destination(dest_id, dest_type) "
				"VALUES "
				"(1, 'Yad2 store'), "
				"(2, 'direct for cutomer');");
			stmt->execute("INSERT INTO shipping_status(status_id, status_type) "
				"VALUES "
				"(1, 'In preparation for delivery'), "
				"(2, 'Sent'), "
				"(3, 'Ready! wait for pickup'), "
				"(4, 'Delivered');");
			stmt->execute("INSERT INTO store_employees(emp_id, emp_fname, emp_lname, emp_phone, join_date, departure_date) "
				"VALUES "
				"(1, 'Ran', 'Ben', '0523330101', '2018-01-01', '2018-12-30'), "
				"(2, 'Eran', 'Golani', '0528777122', '2019-01-01', '2019-04-04'), "
				"(3, 'Roy', 'Eshkol', '0542277010', '2019-04-01', '2019-07-01'), "
				"(4, 'Omer', 'Eshkol', '0502837000', '2019-07-01', '2019-10-01'), "
				"(5, 'Gad', 'Goldstein', '0502837777', '2019-10-01', '2019-12-30'), "
				"(6, 'Edna', 'Levi', '0502837776', '2020-01-01', '2020-05-01'), "
				"(7, 'Ruti', 'Meron', '0502837775', '2020-02-01', '2020-04-01'), "
				"(8, 'Tamir', 'Melamed', '0508887711', '2020-03-01', NULL), "
				"(9, 'Moshe', 'Atzili', '0521234561', '2020-03-13', NULL);");
			stmt->execute("INSERT INTO employee_salary(emp_id, year, months, hours_per_month) "
				"VALUES "
				"(1, '2018', '01', 30), "
				"(1, '2018', '02', 35), "
				"(1, '2018', '03', 25), "
				"(1, '2018', '04', 40), "
				"(1, '2018', '05', 20), "
				"(1, '2018', '06', 35), "
				"(1, '2018', '07', 35), "
				"(1, '2018', '08', 35), "
				"(1, '2018', '09', 40), "
				"(1, '2018', '10', 30), "
				"(1, '2018', '11', 20), "
				"(1, '2018', '12', 10), "
				"(2, '2019', '01', 30), "
				"(2, '2019', '02', 35), "
				"(2, '2019', '03', 25), "
				"(2, '2019', '04', 10), "
				"(3, '2019', '04', 30), "
				"(3, '2019', '05', 35), "
				"(3, '2019', '06', 25), "
				"(4, '2019', '07', 30), "
				"(4, '2019', '08', 35), "
				"(4, '2019', '09', 25), "
				"(5, '2019', '10', 30), "
				"(5, '2019', '11', 35), "
				"(5, '2019', '12', 25), "
				"(6, '2020', '01', 30), "
				"(6, '2020', '02', 35), "
				"(6, '2020', '03', 25), "
				"(6, '2020', '04', 45), "
				"(7, '2020', '02', 30), "
				"(7, '2020', '03', 35), "
				"(8, '2020', '03', 15), "
				"(8, '2020', '04', 20), "
				"(8, '2020', '05', 25), "
				"(8, '2020', '06', 10), "
				"(8, '2020', '07', 25), "
				"(8, '2020', '08', 25), "
				"(8, '2020', '09', 05), "
				"(9, '2020', '03', 15), "
				"(9, '2020', '04', 20), "
				"(9, '2020', '05', 15), "
				"(9, '2020', '06', 30), "
				"(9, '2020', '07', 15), "
				"(9, '2020', '08', 25), "				
				"(9, '2020', '09', 45);");
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
				"(10, 'Guy', 'Gadi', '0521234562'), "
				"(11, 'Alon', 'Shani', '0501200560'), "
				"(100, 'store', 'yad2', '0501134562');");
			stmt->execute("INSERT INTO book_attributes (book_id, title, author, translator, publisher, publishing_year, "
													   "pages_number, book_weight, condition_id, translate_number) "
				"VALUES "
				"(1, 'The Magician', 'Eyal Berkovic', NULL, 'Carmel Publishing House', '2006', 104, 1, 3, 0), "
				"(2, 'Rich Dad Poor Dad', 'Robert Kiyosaki', 'Sharon Lechter', 'Warner Books Ed', '2000', 207, 2, 4, 2), "
				"(3, 'Ulysses', 'James Joyce', NULL, 'Sylvia Beach', '1922', 730, 6, 6, 0), "
				"(4, 'Parallel Worlds', 'Michio Kaku', NULL, 'Doubleday', '2004', 428, 2, 3, 0), "
				"(5, 'retire young retire rich', 'Robert Kiyosaki', 'Sharon Lechter', 'Warner Books Ed', '2015', 400, 3, 2, 1), "
				"(6, 'Bibi', 'Anshel Pfeffer', NULL, 'The Hurst Publishers', '2018', 432, 3, 1, 0), "
				"(7, 'The Little Prince', 'Antoine de Saint Exupery', 'Katherine Woods', 'Reynal and Hitchcock', '1942', 128, 1, 6, 3), "
				"(8, 'Room for Rent', 'Leah Goldberg', NULL, 'Publishing of a workers', '1959', 56, 1, 6, 0), "
				"(9, 'The Great Gatsby', 'Scott Fitzgerald', NULL, 'Charles Scribners Sons', '1926', 218, 2, 5, 0), "
				"(10, 'Harry Potter and the Goblet of Fire', 'J. K. Rowling', 'Nathan Granot', 'Bloomsbury Publishing plc', '2000', 636, 3, 4, 4), "
				"(11, 'The Sellout', 'Paul Beatty', 'Matt Buck', '	Farrar, Straus and Giroux', '2015', 304, 2, 2, 2), "
				"(12, 'The Little Prince', 'Irene Testot Ferry', 'Irene Testot-Ferry', 'Gallimard', '1945', 128, 1, 6, 3), "
				"(13, 'The lover', 'Nimrod Zahavi', 'Shay Barlev', 'Carmel Publishing House', '2012', 209, 2, 4, 1), "
				"(14, 'The Glass Hotel', 'Emily St. John Mandel', 'Eliyahu Shabat', 'HarperCollins', '2020', 320, 2, 1, 1);");
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
			stmt->execute("INSERT INTO book_prices(book_id, cust_price, store_price) "
				"VALUES "
				"(1, 20, 15), "
				"(2, 40, 20), "
				"(3, 30, 20), "
				"(4, 80, 50), "
				"(5, 100, 60), "
				"(6, 90, 30), "
				"(7, 70, 35), "
				"(8, 40, 15), "
				"(9, 60, 45), "
				"(10, 50, 40), "
				"(11, 40, 30), "
				"(12, 100, 70);");
			stmt->execute("INSERT INTO transactions (trans_id, book_id, cust_id, trans_date, meth_id, books_num, emp_id) "
				"VALUES "
				"(1, 2, 10, '2018-01-01', 1, 1, 1), "
				"(2, 10, 8, '2018-01-12', 1, 1, 1), "
				"(3, 2, 6, '2018-03-15', 1, 2, 1), "
				"(4, 3, 7, '2018-03-15', 1, 2, 1), "
				"(5, 1, 11, '2018-03-17', 2, 1, 1), "
				"(6, 2, 1, '2018-05-25', 3, 4, 1), "
				"(7, 11, 9,  '2018-05-27', 3, 0, 1), "
				"(8, 7, 3,  '2018-07-04', 1, 1, 1), "
				"(9, 11, 5,  '2018-07-12', 3, 5, 1), "
				"(10, 10, 9,  '2018-09-22', 1, 3, 1), "
				"(11, 4, 9,  '2018-09-30', 3, 4, 1), "
				"(12, 2, 10,  '2019-02-07', 3, 1, 2), "
				"(13, 10, 6,  '2019-02-17', 3, 7, 2), "
				"(14, 6, 7, '2019-04-19', 3, 6, 3), "
				"(15, 5, 7,  '2019-06-24', 1, 0, 3), "
				"(16, 10, 7,  '2019-06-11', 1, 8, 3), "
				"(17, 2, 2, '2019-08-18', 1, 2, 4), "
				"(18, 9, 3, '2019-10-24', 1, 4, 5), "
				"(19, 8, 4, '2019-10-02', 1, 5, 5), "
				"(20, 10, 9, '2019-12-23', 3, 5, 5), "
				"(21, 4, 10, '2019-12-04', 1, 7, 5), "
				"(22, 2, 5, '2020-01-14', 3, 2, 6), "
				"(23, 6, 2, '2020-02-21', 3, 0, 6), "
				"(24, 2, 3, '2020-03-28', 3, 5, 6), "
				"(25, 3, 4, '2020-04-01', 3, 9, 6), "
				"(26, 2, 1, '2020-05-04', 2, 1, 8), "
				"(27, 11, 2, '2020-05-10', 2, 8, 9), "
				"(28, 4, 3, '2020-05-15', 2, 9, 8), "
				"(29, 1, 4, '2020-05-19', 2, 1, 9), "
				"(30, 10, 6, '2020-05-23', 2, 1, 9), "
				"(31, 10, 6, '2020-05-25', 2, 1, 9), "
				"(32, 12, 4, '2020-05-27', 2, 1, 8);");
			stmt->execute("INSERT INTO orders (order_id, book_id, cust_id, order_date, meth_id, ship_id, status_id, books_num, dest_id1, dest_id2) "
				"VALUES "
				"(1, 7, 3, '2018-05-04', 3, 5, 4, 1, 2, NULL), "
				"(1, 2, 3, '2018-05-04', 3, 51, 4, 1, 2, NULL), "
				"(1, 12, 3, '2018-05-04', 3, 51, 4, 1, 2, NULL), "

				"(2, 4, 7, '2018-05-13', 1, 5, 4, 2, 2, NULL), "
				"(2, 5, 7, '2018-05-13', 1, 51, 4, 2, 2, NULL), "
				"(2, 6, 7, '2018-05-13', 1, 51, 4, 2, 2, NULL), "

				"(3, 5, 10, '2018-05-19', 3, 5, 4, 4, 1, NULL), "
				"(4, 7, 3, '2018-05-26', 2, 5, 4, 3, 2, NULL), "
				"(5, 2, 9, '2018-10-19', 3, 3, 4, 1, 1, NULL), "
				"(6, 4, 4,  '2018-10-24', 1, 2, 4, 2, 2, NULL), "
				"(7, 4, 1,  '2018-11-11', 1, 4, 4, 3, 2, NULL), "
				"(8, 3, 5, '2018-11-18', 1, 5, 4, 2, 1, NULL), "
				"(9, 1, 3, '2018-11-24', 1, 2, 4, 2, 2, NULL), "
				"(10, 9, 4, '2018-12-02', 1, 2, 4, 1, 2, NULL), "
				"(11, 6, 5, '2019-04-23', 3, 4, 3, 2, 1, NULL), "
				"(12, 6, 4, '2019-06-04', 1, 1, 2, 2, 2, NULL), "
				"(13, 2, 1, '2019-07-14', 3, 3, 1, 3, 2, NULL), "
				"(14, 11, 9, '2019-07-21', 3, 1, 1, 4, 2, NULL), "
				"(15, 1, 9, '2019-08-28', 3, 1, 2, 5, 2, NULL), "
				"(16, 2, 1, '2020-08-01', 3, 1, 2, 5, 2, NULL), "
				"(17, 12, 7, '2020-01-05', 2, 2, 1, 6, 1, NULL), "
				"(18, 8, 5, '2020-08-30', 2, 5, 2, 3, 2, NULL), "
				"(19, 7, 10, '2020-03-10', 2, 4, 3, 1, 2, NULL), "
				"(20, 11, 8, '2020-02-28', 2, 4, 2, 2, 2, NULL), "
				"(21, 6, 100, '2018-01-02', 2, 5, 4, 20, 1, NULL), "
				"(22, 9, 100, '2018-02-22', 2, 5, 4, 10, 1, NULL), "
				"(23, 3, 100, '2019-06-17', 2, 5, 4, 8, 1, NULL), "
				"(24, 10, 100, '2019-11-22', 2, 5, 4, 12, 1, NULL), "
				"(25, 2, 100, '2020-03-11', 2, 5, 4, 30, 1, NULL), "
				"(26, 10, 6, '2020-01-14', 2, 2, 2, 4, 2, NULL), "
				"(27, 13, 2, '2020-04-14', 2, 5, 2, 4, 2, NULL), "
				"(28, 12, 9, '2020-05-19', 1, 5, 1, 9, 2, NULL), "
				"(29, 7, 1, '2020-05-29', 3, 4, 3, 2, 1, NULL), "
				"(30, 12, 10, '2020-06-23', 2, 5, 2, 3, 1, 2), "
				"(300, 11, 10, '2020-06-23', 2, 51, 4, 2, 2, 2), "
				"(31, 12, 100, '2020-06-23', 2, 5, 4, 3, 1, NULL), "
				"(32, 2, 6, '2020-09-14', 2, 5, 3, 2, 2, NULL);");

			
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