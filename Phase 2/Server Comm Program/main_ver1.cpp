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

using std::cout;
using std::endl;

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

	try {
		std::string s;
		sql::mysql::MySQL_Driver *driver;	// Connect to mySQL database
		sql::Connection *con;			// Allow use of mySQL functions
		sql::Statement *stmt;			// Used to perform tasks
		sql::PreparedStatement *pstmt;

		int last = 0;

		// Connect to mySQL
		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "user", "password");

		//driver->threadInit();

		// Create statement
		stmt = con->createStatement();

		//stmt->execute("USE CAN");
		con->setSchema("CAN");
		stmt->execute("DROP TABLE IF EXISTS Logging");
		stmt->execute("CREATE TABLE Logging(id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, Node CHAR(1), Inst CHAR(1), CMD CHAR(1))");
		pstmt = con->prepareStatement("INSERT INTO Logging(Node, Inst, CMD) VALUES (?, ?, ?)");

		//std::auto_ptr< sql::PreparedStatement >  pstmt;
		//pstmt.reset(con->prepareStatement("INSERT INTO Logging(Node, Inst, CMD) VALUES (?, ?, ?)"));

		try {
			BufferedAsyncSerial serial("/dev/ttyUSB0",9600);
	
			for(;;)
			{
		
				if(last != s.length())
				{
					last = s.length();
					cout << "length: " << last << endl;
				}

				s += serial.readString();		
				if(s.length() == 3)
				{
					std::cout << s << std::endl;
					//pstmt = con->prepareStatement("INSERT INTO Logging(Node, Inst, CMD) VALUES (?, ?, ?)");
					//stmt->execute("INSERT INTO Logging(Node, Inst, CMD) VALUES (?,?,?)");
					pstmt->setString(1, s.substr(0,1));
					pstmt->setString(2, s.substr(1,1));
					pstmt->setString(3, s.substr(2,1));
					//pstmt->setString(1, "2");
					//pstmt->setString(2, "0");
					//pstmt->setString(3, "5");
					int result = pstmt->execute();
					std::cout << result << std::endl;
					s.clear();
				
					//delete pstmt; 
				}
			

			

			}

			serial.close();

		} catch(boost::system::system_error& e)
		{
			std::cout <<"Error: " << e.what() << std::endl;
			return 1;
		}

		delete stmt;
		delete pstmt;
		delete con;
		//driver->threadEnd();

	} catch (sql::SQLException &e) 
	{
	  std::cout << "# ERR: SQLException in " << __FILE__;
	  std::cout << "(" << __FUNCTION__ << ") on line "   << __LINE__ << std::endl;
	  std::cout << "# ERR: " << e.what();
	  std::cout << " (MySQL error code: " << e.getErrorCode();
	  std::cout << ", SQLState: " << e.getSQLState() <<  " )" << std::endl;
	}

	return 0;
}
