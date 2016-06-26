/*
Purpose: Program to store data from the serial port in a database
 Author: Josh Osborne
   Date: June 23, 2016
Version: 2
*/

#include <stdlib.h>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <chrono>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "queue.h"
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

void Read_Serial();
void Write_DB();
void Get_CMD();

Queue Serial_Recieve;
Queue Serial_Send;

int main(int argc, char* argv[])
{
	std::thread t1;
	std::thread t2;
	std::thread t3;
	
	t1 = std::thread(Read_Serial);
	t2 = std::thread(Write_DB);
	t3 = std::thread(Get_CMD);
	
	t1.join();
	t2.join();
	t3.join();

	return 0;
}

void Get_CMD()
{
	std::string data;
	std::string last;
	std::ifstream fd;

	//int fd;
	//char * myfifo = "/tmp/cumwart";
	//char buf[100];

	/* open, read, and display the message from the FIFO */
	//fd = open(myfifo, O_RDONLY);
	//fd.open ("/tmp/cumwart", std::ifstream::in); 
	
	for(;;)
	{
		//read(fd, buf, 100);
		//printf("Received: %s\n", buf);
		fd.open ("/tmp/cumwart", std::ifstream::in); 
		fd >> data;		
		fd.close();

		if(data!=last)
		{
			cout << "pipe: " << data << endl;
			Serial_Send.push(data);
			last = data;
		}

		//data.clear();

		
		

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}

	//fd.close();
}

void Write_DB()
{
	//try {
		std::string s;
		sql::mysql::MySQL_Driver *driver;	// Connect to mySQL database
		sql::Connection *con;			// Allow use of mySQL functions
		sql::Statement *stmt;			// Used to perform tasks
		sql::PreparedStatement *pstmt;

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


		for(;;)
		{
			if(!Serial_Recieve.empty())
			{
				s = Serial_Recieve.pop();
				//cout << "string: " << s << endl;
				pstmt->setString(1, s.substr(0,1));
				pstmt->setString(2, s.substr(1,1));
				pstmt->setString(3, s.substr(2,1));
				
				int result = pstmt->execute();
				//std::cout << "result: " << result << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		}

		delete stmt;
		delete pstmt;
		delete con;

	/*} catch (sql::SQLException &e) 
	{
	  std::cout << "# ERR: SQLException in " << __FILE__;
	  std::cout << "(" << __FUNCTION__ << ") on line "   << __LINE__ << std::endl;
	  std::cout << "# ERR: " << e.what();
	  std::cout << " (MySQL error code: " << e.getErrorCode();
	  std::cout << ", SQLState: " << e.getSQLState() <<  " )" << std::endl;
	}*/
}

void Read_Serial()
{
	std::string s;
	unsigned int last = 0;
	
	//try {
			BufferedAsyncSerial serial("/dev/ttyUSB0",9600);
	
			for(;;)
			{
		
				if(last != s.length())
				{
					last = s.length();
					//cout << "length: " << last << endl;
				}

				s += serial.readString();		
				if(s.length() == 3)
				{
					std::cout << s << std::endl;
					Serial_Recieve.push(s);
					s.clear();
				}
				
				if(!Serial_Send.empty())
				{
					serial.writeString(Serial_Send.pop);
				}
			

			
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			serial.close();

		/*} catch(boost::system::system_error& e)
		{
			std::cout <<"Error: " << e.what() << std::endl;
			//return 1;
			exit(-1);
		}*/
}
