/*
Purpose: Program to store data from the serial port in a database
 Author: Josh Osborne
   Date: June 10, 2016
Version: 1
*/

#include <stdlib.h>
#include <iostream>
#include <string>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <boost/asio/serial_port.hpp> 
#include <boost/asio.hpp>
#include "AsyncSerial.h"
#include "BufferedAsyncSerial.h"

/*int main(void)
{
	sql::mysql::MySQL_Driver *driver;	// Connect to mySQL database
	sql::Connection *con;			// Allow use of mySQL functions
	sql::Statement *stmt;			// Used to perform tasks		

	// Connect to the database
	driver = sql::mysql::get_mysql_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "user", "password");

	// Create statement
	stmt = con->createStatement();
	// MySQL code
	stmt->execute("USE CAN");
	stmt->execute("DROP TABLE IF EXISTS test");
	stmt->execute("CREATE TABLE test(id INT, label CHAR(1))");
	stmt->execute("INSERT INTO test(id, label) VALUES (6, 'z')");

	delete stmt;
	delete con;

	return 0;
}*/

int main(int argc, char* argv[])
{
	std::string s;
	sql::mysql::MySQL_Driver *driver;	// Connect to mySQL database
	sql::Connection *con;			// Allow use of mySQL functions
	sql::Statement *stmt;			// Used to perform tasks
	sql::PreparedStatement *pstmt;

	// Connect to mySQL
	driver = sql::mysql::get_mysql_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "user", "password");

	// Create statement
	stmt = con->createStatement();

	stmt->execute("USE CAN");
	stmt->execute("DROP TABLE IF EXISTS Logging");
	stmt->execute("CREATE TABLE Logging(id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, Node CHAR(1), Inst CHAR(1), CMD CHAR(1))");
	pstmt = con->prepareStatement("INSERT INTO Logging(Node, Inst, CMD) VALUES (?, ?, ?)");

	try {
		BufferedAsyncSerial serial("/dev/ttyUSB1",9600);
	
		for(;;)
		{
		
			s += serial.readString();		
			if(s.length() == 3)
			{
				std::cout << s << std::endl;
				//stmt->execute("INSERT INTO Logging(Node, Inst, CMD) VALUES ('"s.at(1)"', '"s.at(2)"', '"s.at(3)"')");
				pstmt->setString(1, s.substr(0,1));
				pstmt->setString(1, s.substr(1,2));
				pstmt->setString(1, s.substr(2,3));
				//pstmt->setChar(2, '0');
				//pstmt->setString(3, "5");
				//pstmt->execute();
				s.clear();
			}

		}

		serial.close();

	} catch(boost::system::system_error& e)
	{
		std::cout <<"Error: " << e.what() << std::endl;
		return 1;
	}

	delete stmt;
	delete con;

	return 0;
}
