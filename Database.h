#pragma once
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace sql;
using namespace std;
#define DB_NAME "yad_two_store"
#define DB_HOST "tcp://127.0.0.1/"
#define DB_PORT 3306
#define DB_USER "root"
#define DB_PASS ""

class Database {
private:
	// singleton instance
	static Database* instance;

	Driver* driver;
	ConnectOptionsMap connection_properties;

	// private CTOR (singleton)
	Database();
public:
	static Database& getInstance();

	Connection* getConnection();

	virtual ~Database() {}
};
