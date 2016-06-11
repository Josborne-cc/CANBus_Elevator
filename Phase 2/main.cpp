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
#include <boost/asio/serial_port.hpp> 
#include <boost/asio.hpp>

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
 
int main (void) 
{
	boost::asio::io_service io;
	boost::asio::serial_port port(io);
 
	port.open("/dev/ttyUSB0");
	port.set_option(boost::asio::serial_port_base::baud_rate(9600));
 
	char c;
 
	// Read 1 character into c, this will block
	// forever if no character arrives.
	boost::asio::read(port, boost::asio::buffer(&c,1));
	std::cout << "c: " << c << std::endl;
	boost::asio::read(port, boost::asio::buffer(&c,1));
	std::cout << "c: " << c << std::endl;
	boost::asio::read(port, boost::asio::buffer(&c,1));
	std::cout << "c: " << c << std::endl;
 
	port.close();

	return 0;
}
