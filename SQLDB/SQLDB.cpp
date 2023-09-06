#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;
using namespace sql;
using namespace mysql;

class MySQLfuncs
{
public:
    Connection* ServerConnector(string host, string user, string password);
    void ServerDisconnector(Connection* con);
    void StatementExecuter(string statement, Connection* con);
    int RetrieveTemperatureForCity(string city, Connection* con);

};



Connection* MySQLfuncs::ServerConnector(string host, string user, string password)
{
    MySQL_Driver* driver = get_mysql_driver_instance();
    Connection* con = driver->connect(host, user, password);

    if (con) {
        cout << "Connection was successful" << endl;
    }
    else {
        cout << "Connection attempt was unsuccessful" << endl;
    }

    return con;
}

void MySQLfuncs::ServerDisconnector(Connection* con)
{
    delete con;
    cout << "Connection closed." << endl;
}

void MySQLfuncs::StatementExecuter(string statement, Connection* con)
{
    try {
        Statement* stmt = con->createStatement();
        stmt->execute(statement);
        cout << "Given statement executed successfully" << endl;
        delete stmt;
    }
    catch (SQLException& e) {
        cout << "SQLException: " << e.what() << endl;
    }
}

int MySQLfuncs::RetrieveTemperatureForCity(string city, Connection* con)
{
    int temperature = 0;

    try {
        Statement* stmt = con->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT temperature FROM weather_data WHERE city = '" + city + "'");

        if (res->next()) 
        {
            temperature = res->getInt("temperature");
        }

        delete res;
        delete stmt;
    }
    catch (SQLException& e) {
        cout << "SQLException: " << e.what() << endl;
    }

    return temperature;
}

int main()
{
    MySQLfuncs mysqlfuncs;
    Connection* con = mysqlfuncs.ServerConnector("localhost", "root", "India@11");

    if (con)
    {
        mysqlfuncs.StatementExecuter("CREATE DATABASE IF NOT EXISTS weather", con);
        mysqlfuncs.StatementExecuter("USE weather", con);

        // Create the weather_data table
        mysqlfuncs.StatementExecuter(
            "CREATE TABLE IF NOT EXISTS weather_data ("
            "id INT AUTO_INCREMENT PRIMARY KEY,"
            "city VARCHAR(255),"
            "temperature INT"
            ")", con
        );

        // Insert sample data
        mysqlfuncs.StatementExecuter("INSERT INTO weather_data (city, temperature) VALUES ('Mumbai', 32)", con);
        mysqlfuncs.StatementExecuter("INSERT INTO weather_data (city, temperature) VALUES ('Delhi', 30)", con);

        int temperature = mysqlfuncs.RetrieveTemperatureForCity("Delhi", con);
        if (temperature != 0) {
            cout << "Temperature for Delhi: " << temperature << "°C" << endl;
        }

        mysqlfuncs.ServerDisconnector(con);
    }

    return 0;
}
