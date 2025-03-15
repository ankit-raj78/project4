#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <string>
#include <sstream>

#include "query_funcs.h"
#include "exerciser.h"

using namespace std;
using namespace pqxx;

// Function to parse player data from file
void parsePlayerData(connection *C, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
        double spg, bpg;
        string first_name, last_name;
        
        if (ss >> player_id >> team_id >> uniform_num >> first_name >> last_name 
              >> mpg >> ppg >> rpg >> apg >> spg >> bpg) {
            add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
        }
    }
    file.close();
}

// Function to parse team data from file
void parseTeamData(connection *C, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int team_id, state_id, color_id, wins, losses;
        string name;
        
        if (ss >> team_id >> name >> state_id >> color_id >> wins >> losses) {
            add_team(C, team_id, name, state_id, color_id, wins, losses);
        }
    }
    file.close();
}

// Function to parse state data from file
void parseStateData(connection *C, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int state_id;
        string name;
        
        if (ss >> state_id >> name) {
            add_state(C, state_id, name);
        }
    }
    file.close();
}

// Function to parse color data from file
void parseColorData(connection *C, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int color_id;
        string name;
        
        if (ss >> color_id >> name) {
            add_color(C, color_id, name);
        }
    }
    file.close();
}

int main (int argc, char *argv[]) {
    // Allocate & initialize a Postgres connection object
    connection *C;
    
    try {
        // Establish a connection to the database
        C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd hostaddr=127.0.0.1");
        if (C->is_open()) {
            cout << "Opened database successfully: " << C->dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return 1;
    }
    
    // Drop existing tables (in reverse order of dependencies)
    work W(*C);
    W.exec("DROP TABLE IF EXISTS PLAYER CASCADE;");
    W.exec("DROP TABLE IF EXISTS TEAM CASCADE;");
    W.exec("DROP TABLE IF EXISTS STATE CASCADE;");
    W.exec("DROP TABLE IF EXISTS COLOR CASCADE;");
    W.commit();
    
    // Create tables (in order of dependencies)
    work W2(*C);
    
    // Create STATE table
    W2.exec("CREATE TABLE STATE ("
           "STATE_ID INT PRIMARY KEY, "
           "NAME VARCHAR(256) NOT NULL);");
           
    // Create COLOR table
    W2.exec("CREATE TABLE COLOR ("
           "COLOR_ID INT PRIMARY KEY, "
           "NAME VARCHAR(256) NOT NULL);");
           
    // Create TEAM table with foreign keys
    W2.exec("CREATE TABLE TEAM ("
           "TEAM_ID INT PRIMARY KEY, "
           "NAME VARCHAR(256) NOT NULL, "
           "STATE_ID INT NOT NULL REFERENCES STATE(STATE_ID), "
           "COLOR_ID INT NOT NULL REFERENCES COLOR(COLOR_ID), "
           "WINS INT NOT NULL, "
           "LOSSES INT NOT NULL);");
           
    // Create PLAYER table with foreign key
    W2.exec("CREATE TABLE PLAYER ("
           "PLAYER_ID SERIAL PRIMARY KEY, " 
           "TEAM_ID INT NOT NULL REFERENCES TEAM(TEAM_ID), "
           "UNIFORM_NUM INT NOT NULL, "
           "FIRST_NAME VARCHAR(256) NOT NULL, "
           "LAST_NAME VARCHAR(256) NOT NULL, "
           "MPG INT NOT NULL, "
           "PPG INT NOT NULL, "
           "RPG INT NOT NULL, "
           "APG INT NOT NULL, "
           "SPG DECIMAL(3,1) NOT NULL, "
           "BPG DECIMAL(3,1) NOT NULL);");
    W2.commit();
    
    // Load data from files
    parseStateData(C, "state.txt");
    parseColorData(C, "color.txt");
    parseTeamData(C, "team.txt");
    parsePlayerData(C, "player.txt");
    
    // Execute example queries
    exercise(C);
    
    // Close database connection - just delete the object
    delete C;
    
    return 0;
}


