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
			stmt->execute("CREATE TABLE IF NOT EXISTS shipping_destination("
				"dest_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"address VARCHAR(100)"
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
				"language VARCHAR(100)"
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
				"cust_id INT UNSIGNED,"
				"FOREIGN KEY(cust_id) REFERENCES customers(cust_id),"
				"trans_date DATE,"
				"meth_id INT UNSIGNED,"
				"FOREIGN KEY(meth_id) REFERENCES method_options(meth_id),"
				"emp_id INT UNSIGNED,"
				"FOREIGN KEY(emp_id) REFERENCES store_employees(emp_id)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS orders("
				"order_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"cust_id INT UNSIGNED,"
				"FOREIGN KEY(cust_id) REFERENCES customers(cust_id),"
				"order_date DATE,"
				"meth_id INT UNSIGNED,"
				"ship_id INT UNSIGNED,"
				"status_id INT UNSIGNED,"
				"FOREIGN KEY(meth_id) REFERENCES method_options(meth_id),"
				"FOREIGN KEY(ship_id) REFERENCES shipping_options(ship_id),"
				"FOREIGN KEY(status_id) REFERENCES shipping_status(status_id)"
				");");

			stmt->execute("CREATE TABLE IF NOT EXISTS transaction_details("
				"trans_id INT UNSIGNED,"
				"FOREIGN KEY(trans_id) REFERENCES transactions(trans_id),"
				"book_id INT UNSIGNED,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"book_amount INT UNSIGNED"
				");");

			stmt->execute("CREATE TABLE IF NOT EXISTS order_details("
				"order_id INT UNSIGNED,"
				"FOREIGN KEY(order_id) REFERENCES orders(order_id),"
				"book_id INT UNSIGNED,"
				"FOREIGN KEY(book_id) REFERENCES book_attributes(book_id),"
				"book_amount INT UNSIGNED,"
				"dest_id INT UNSIGNED,"
				"FOREIGN KEY(dest_id) REFERENCES shipping_destination(dest_id)"
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
			stmt->execute("INSERT INTO shipping_destination(dest_id, address) "
				"VALUES "
				"(1, 'Oranit 10 Bnei Brak'), "
				"(2, 'Ben-Gurion 26 Tel Aviv'), "
				"(3, 'Hohit 8 Hod Hasharon'), "
				"(4, 'Ephraim Katzir 4 Ramat Hasharon'), "
				"(5, 'Ana Frank 1 Ramat Gan'), "
				"(6, 'Hadkalim 12 Ramat Efal'), "
				"(7, 'Harbaa 2 Givatim'), "
				"(8, 'Herzel 16 Eilat'), "
				"(9, 'Levi Eshkol 19 Haifa'), "
				"(10, 'Einsten 11 Savion'), "
				"(11, 'Zvi Shapira 2 Tel Mond'), "
				"(100, 'Dizingoff 1 Tel Aviv');");
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
													   "pages_number, book_weight, condition_id, language) "
				"VALUES "
				"(1, 'The Magician', 'Eyal Berkovic', NULL, 'Carmel Publishing House', '2006', 104, 1, 3, 'Hebrew'), "
				"(2, 'Rich Dad Poor Dad', 'Robert Kiyosaki', 'Sharon Lechter', 'Warner Books Ed', '2000', 207, 2, 4, 'English'), "
				"(3, 'Ulysses', 'James Joyce', NULL, 'Sylvia Beach', '1922', 730, 6, 6, 'English'), "
				"(4, 'Parallel Worlds', 'Michio Kaku', NULL, 'Doubleday', '2004', 428, 2, 3, 'English'), "
				"(5, 'retire young retire rich', 'Robert Kiyosaki', 'Sharon Lechter', 'Warner Books Ed', '2015', 400, 3, 2, 'English'), "
				"(6, 'Bibi', 'Anshel Pfeffer', NULL, 'The Hurst Publishers', '2018', 432, 3, 1, 'Hebrew'), "
				"(7, 'The Little Prince', 'Antoine de Saint Exupery', 'Katherine Woods', 'Reynal and Hitchcock', '1942', 128, 1, 6, 'English'), "
				"(8, 'Room for Rent', 'Leah Goldberg', NULL, 'Publishing of a workers', '1959', 56, 1, 6, 'Hebrew'), "
				"(9, 'The Great Gatsby', 'Scott Fitzgerald', NULL, 'Charles Scribners Sons', '1926', 218, 2, 5, 'French'), "
				"(10, 'Ulysses', 'James Joyce', NULL, 'Sylvia Beach', '1922', 730, 6, 6, 'French'), "
				"(11, 'Harry Potter and the Goblet of Fire', 'J. K. Rowling', 'Nathan Granot', 'Bloomsbury Publishing plc', '2000', 636, 3, 4, 'English'), "
				"(12, 'The Sellout', 'Paul Beatty', 'Matt Buck', '	Farrar, Straus and Giroux', '2015', 304, 2, 2, 'English'), "
				"(13, 'The Little Prince', 'Irene Testot Ferry', 'Irene Testot-Ferry', 'Gallimard', '1945', 128, 1, 6, 'French'), "
				"(14, 'Ulysses', 'James Joyce', NULL, 'Sylvia Beach', '1922', 730, 6, 6, 'Spanish'), "
				"(15, 'The lover', 'Nimrod Zahavi', 'Shay Barlev', 'Carmel Publishing House', '2012', 209, 2, 4, 'Hebrew'), "
				"(16, 'The Glass Hotel', 'Emily St. John Mandel', 'Eliyahu Shabat', 'HarperCollins', '2020', 320, 2, 1, 'English');");
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
				"(13, 1), "
				"(14, 1), "
				"(15, 1), "
				"(16, 90);");
			stmt->execute("INSERT INTO storage_items(book_id, amount) "
				"VALUES "
				"(1, 30), "
				"(2, 40), "
				"(3, 20), "
				"(4, 30), "
				"(5, 50), "
				"(6, 100), "
				"(7, 70), "
				"(8, 40), "
				"(9, 50), "
				"(10, 10), "
				"(11, 80), "
				"(12, 10), "
				"(13, 30), "
				"(14, 20), "
				"(15, 17), "
				"(16, 70);");
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
				"(12, 75, 35), "
				"(13, 60, 40), "
				"(14, 60, 40), "
				"(15, 60, 40), "
				"(16, 110, 70);");
			stmt->execute("INSERT INTO transactions (trans_id, cust_id, trans_date, meth_id, emp_id) "
				"VALUES "
				"(1, 10, '2018-01-01', 1, 1), "
				"(2,  8, '2018-02-12', 1, 1), "
				"(3, 6, '2018-03-11', 1, 1), "
				"(4, 7, '2018-03-15', 1, 1), "
				"(5, 11, '2018-03-17', 2, 1), "
				"(6, 1, '2018-04-25', 3, 1), "
				"(7, 9,  '2018-05-27', 3, 1), "
				"(8, 3,  '2018-06-04', 1, 1), "
				"(9, 5,  '2018-08-12', 3, 1), "
				"(10, 9,  '2018-09-22', 1, 1), "
				"(11, 9,  '2018-09-30', 3, 1), "
				"(12, 10,  '2019-02-07', 3, 2), "
				"(13, 6,  '2019-03-17', 3,  2), "
				"(14, 7, '2019-04-19', 3, 3), "
				"(15, 7,  '2019-05-04', 1, 3), "
				"(16, 7,  '2019-06-11', 1, 3), "
				"(17, 2, '2019-08-18', 1, 4), "
				"(18, 3, '2019-10-24', 1, 5), "
				"(19, 4, '2019-10-26', 1, 5), "
				"(20, 9, '2019-11-23', 3, 5), "
				"(21, 10, '2019-12-04', 1, 5), "
				"(22, 5, '2020-01-14', 3, 6), "
				"(23, 2, '2020-02-21', 3, 6), "
				"(24, 3, '2020-03-28', 3, 6), "
				"(25, 4, '2020-04-01', 3, 6), "
				"(26, 1, '2020-05-04', 2, 8), "
				"(27, 2, '2020-05-10', 2, 9), "
				"(28, 3, '2020-06-15', 2, 8), "
				"(29, 4, '2020-06-19', 2, 9), "
				"(30, 4, '2020-06-21', 2, 9), "
				"(31, 6, '2020-07-10', 2, 9), "
				"(32, 3, '2020-07-15', 2, 8), "
				"(33, 6, '2020-07-25', 2, 9), "
				"(34, 4, '2020-08-26', 2, 9), "
				"(35, 4, '2020-08-27', 2, 8);");
			stmt->execute("INSERT INTO transaction_details (trans_id, book_id, book_amount) "
				"VALUES "
				"(1, 2, 1), "
				"(2, 10, 1), "
				"(3, 2, 2), "
				"(4, 3, 2), "
				"(5, 15, 1), "
				"(6, 2, 4), "
				"(7, 11, 0), "
				"(8, 7, 1), "
				"(9, 11, 5), "
				"(10, 10, 3), "
				"(11, 4, 4), "
				"(12, 2, 1), "
				"(13, 10, 7), "
				"(14, 6, 6), "
				"(15, 5, 0), "
				"(16, 1, 8), "
				"(17, 2, 2), "
				"(18, 9, 4), "
				"(19, 8, 5), "
				"(20, 10, 5), "
				"(21, 4, 7), "
				"(22, 2, 2), "
				"(23, 16, 0), "
				"(24, 2, 5), "
				"(25, 13, 9), "
				"(26, 2, 1), "
				"(27, 11, 8), "
				"(28, 14, 9), "
				"(29, 15, 1), "
				"(30, 2, 1), "
				"(31, 10, 1), "
				"(32, 1, 9), "
				"(33, 10, 1), "
				"(34, 5, 1), "
				"(35, 12, 1);");

			stmt->execute("INSERT INTO orders (order_id, cust_id, order_date, meth_id, ship_id, status_id) "
				"VALUES "
				"(1, 3, '2018-01-04', 3, 5, 4), "
				"(2, 7, '2018-02-13', 1, 5, 4), "
				"(3, 10, '2018-03-19', 3, 5, 4), "
				"(4, 3, '2018-04-26', 2, 5, 4), "
				"(5, 9, '2018-05-19', 3, 3, 4), "
				"(6, 4,  '2018-06-24', 1, 2, 4), "
				"(7, 1,  '2018-07-11', 1, 4, 4), "
				"(8, 5, '2018-08-18', 1, 5, 4), "
				"(9, 3, '2018-09-24', 1, 2, 4), "
				"(10, 4, '2018-10-02', 1, 2, 4), "
				"(11, 4, '2018-11-02', 1, 2, 4), "
				"(12, 1, '2018-12-02', 2, 3, 4), "
				"(13, 5, '2019-01-23', 3, 4, 3), "
				"(14, 4, '2019-02-04', 1, 1, 2), "
				"(15, 1, '2019-03-14', 3, 3, 1), "
				"(16, 9, '2019-04-21', 3, 1, 1), "
				"(17, 9, '2019-05-28', 3, 1, 2), "
				"(18, 1, '2019-06-01', 3, 1, 2), "
				"(19, 7, '2019-07-05', 2, 2, 1), "
				"(20, 5, '2019-08-30', 2, 5, 2), "
				"(21, 10, '2019-09-10', 2, 4, 3), "
				"(22, 8, '2019-10-28', 2, 4, 2), "
				"(23, 100, '2019-11-02', 2, 5, 3), "
				"(24, 1, '2019-12-22', 2, 5, 3), "
				"(25, 11, '2019-12-23', 2, 5, 4), "
				"(26, 100, '2020-01-17', 2, 5, 4), "
				"(27, 100, '2020-01-22', 2, 1, 4), "
				"(28, 100, '2020-02-11', 2, 5, 4), "
				"(29, 6, '2020-02-14', 2, 2, 2), "
				"(30, 2, '2020-03-14', 2, 1, 2), "
				"(31, 9, '2020-04-19', 1, 5, 1), "
				"(32, 1, '2020-05-29', 3, 4, 3), "
				"(33, 10, '2020-06-23', 2, 2, 2), "
				"(34, 6, '2020-06-25', 2, 3, 3), "
				"(35, 2, '2020-07-02', 2, 1, 4), "
				"(36, 6, '2020-07-01', 2, 5, 2), "
				"(37, 2, '2020-07-10', 2, 1, 1), "
				"(38, 9, '2020-07-17', 2, 3, 3), "
				"(39, 6, '2020-07-30', 2, 3, 3), "
				"(40, 2, '2020-08-01', 2, 3, 3), "
				"(41, 7, '2020-08-04', 2, 5, 3);");

			stmt->execute("INSERT INTO order_details (order_id, book_id, book_amount, dest_id) "
				"VALUES "
				"(1, 3, 2, 1), "
				"(1, 2, 1, 1), "
				"(1, 7, 3, 1), "
				"(2, 7, 1, 2), "
				"(2, 1, 2, 2), "
				"(2, 12, 4, 100), "
				"(3, 11, 2, 3), "
				"(4, 3, 3, 4), "
				"(5, 9, 2, 5), "
				"(6, 4, 5, 6), "
				"(7, 1, 2, 7), "
				"(8, 5, 7, 8), "
				"(9, 3, 7, 9), "
				"(10, 4, 9, 10), "
				"(11, 5, 10, 100), "
				"(12, 4, 11, 1), "
				"(13, 1, 3, 9), "
				"(14, 9, 1, 1), "
				"(15, 9, 4, 4), "
				"(16, 1, 6, 1), "
				"(17, 7, 3, 3), "
				"(17, 13, 1, 2), "
				"(18, 5, 9, 1), "
				"(19, 10, 8, 100), "
				"(20, 8, 7, 1), "
				"(21, 14, 1, 100), "
				"(22, 12, 3, 1), "
				"(23, 12, 9, 3), "
				"(24, 14, 11, 1), "
				"(25, 14, 12, 2), "
				"(26, 6, 12, 5), "
				"(27, 2, 9, 1), "
				"(28, 9, 8, 2), "
				"(28, 7, 8, 6), "
				"(28, 13, 8, 2), "
				"(29, 1, 12, 1), "
				"(30, 10, 6, 7), "
				"(31, 10, 3, 3), "
				"(32, 11, 2, 6), "
				"(33, 14, 12, 4), "
				"(34, 11, 13, 10), "
				"(35, 14, 10, 100), "
				"(36, 14, 1, 10), "
				"(37, 10, 10, 6), "
				"(38, 10, 7, 100), "
				"(39, 10, 6, 100), "
				"(40, 10, 1, 100), "
				"(41, 12, 4, 3);");

			
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



