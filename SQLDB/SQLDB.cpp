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

struct weatherlog
{
    int entryID;
    string city;
    int time;
    string event;
};

int main()
{
    MySQLfuncs mysqlfuncs;
    Connection* con = mysqlfuncs.ServerConnector("localhost", "root", "India@11");

    if (con)
    {
        int choice = 1;

        cout << "***************************************************\nWELCOME TO CPP BASED SERVER EDITOR\n**************************************************\n" << endl;
        cout << "What would you like to do ?" << endl;
        cout << "Enter 1 for creating or selecting a database\nEnter 2 for creating or selecting a table\nEnter 3 for inputting data\nEnter 4 for retrieving data" << endl;

        cin >> choice;

        string tableName, parameters, createdbName, currentdbName;
        bool shouldContinue;


        switch (choice)
        {
        case 1: 
            cout << "Enter database name\n";
            cin >> createdbName;

            mysqlfuncs.StatementExecuter("CREATE DATABASE IF NOT EXISTS "+createdbName, con);
            mysqlfuncs.StatementExecuter("USE "+createdbName, con);
            cout << "Do you want to continue to the next option? (1 for yes, 0 for no)\n";
            cin >> shouldContinue;
            if (!shouldContinue)
            {
                return 0; // This exits the current function
            }
            

            

            
        case 2:
        
            cout << "Enter the current database name\n";
            cin >> currentdbName;
            cout << "Enter table name\n";
            cin >> tableName;
            mysqlfuncs.StatementExecuter("USE " + currentdbName, con);

            mysqlfuncs.StatementExecuter("CREATE TABLE IF NOT EXISTS " + tableName + " (entryID INT, city VARCHAR(255), time INT, event VARCHAR(255));", con);
            

            
            cout << "Do you want to continue to the next option? (1 for yes, 0 for no)\n";
            cin >> shouldContinue;
            if (!shouldContinue)
            {
                return 0; 
            };

        case 3:
            cout << "Enter the current database name\n";
            cin >> currentdbName;
            cout << "Enter table name\n";
            cin >> tableName;
            mysqlfuncs.StatementExecuter("USE " + currentdbName, con);
            bool again;

            do {
                weatherlog weatherlog1;
                cout << "Enter the entry id: ";
                cin >> weatherlog1.entryID;
                cout << "Enter the city: ";
                cin >> weatherlog1.city;
                cout << "Enter the date in ddmmyy: ";
                cin >> weatherlog1.time;
                cout << "Enter the event type: ";
                cin >> weatherlog1.event;

                string insertSQL = "INSERT INTO " + tableName + " (entryID, city, time, event) VALUES ("
                    + to_string(weatherlog1.entryID) + ", '"
                    + weatherlog1.city + "', "
                    + to_string(weatherlog1.time) + ", '"
                    + weatherlog1.event + "');";
                mysqlfuncs.StatementExecuter(insertSQL, con);

                cout << "Do you want to insert another record? (1/0): ";
                cin >> again;
            } while (again);
            cout << "Do you want to continue to the next option? (1 for yes, 0 for no)\n";
            cin >> shouldContinue;
            if (!shouldContinue)
            {
                return 0; // This exits the current function
            };
            
             


        default:
            cout << "enter valid\n";
            break;

        }

      /*  mysqlfuncs.StatementExecuter("CREATE DATABASE IF NOT EXISTS weather", con);
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
        */
       // int temperature = mysqlfuncs.RetrieveTemperatureForCity("Delhi", con);
       // if (temperature != 0) {
      //     cout << "Temperature for Delhi: " << temperature << "°C" << endl;
       // }

        mysqlfuncs.ServerDisconnector(con);
    }

    return 0;
}
