#include "query_funcs.h"
#include <iomanip>

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg) {
    work W(*C);
    
    // Create SQL statement with proper escaping for string values
    stringstream sql;
    sql << "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES ("
        << team_id << ", "
        << jersey_num << ", "
        << W.quote(first_name) << ", "
        << W.quote(last_name) << ", "
        << mpg << ", "
        << ppg << ", "
        << rpg << ", "
        << apg << ", "
        << fixed << setprecision(1) << spg << ", "
        << fixed << setprecision(1) << bpg << ")";
    
    W.exec(sql.str());
    W.commit();
}

void add_team(connection *C, int team_id, string name, int state_id, int color_id, int wins, int losses) {
    work W(*C);
    
    stringstream sql;
    sql << "INSERT INTO TEAM (TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ("
        << team_id << ", "
        << W.quote(name) << ", "
        << state_id << ", "
        << color_id << ", "
        << wins << ", "
        << losses << ")";
    
    W.exec(sql.str());
    W.commit();
}

void add_state(connection *C, int state_id, string name) {
    work W(*C);
    
    stringstream sql;
    sql << "INSERT INTO STATE (STATE_ID, NAME) VALUES ("
        << state_id << ", "
        << W.quote(name) << ")";
    
    W.exec(sql.str());
    W.commit();
}

void add_color(connection *C, int color_id, string name) {
    work W(*C);
    
    stringstream sql;
    sql << "INSERT INTO COLOR (COLOR_ID, NAME) VALUES ("
        << color_id << ", "
        << W.quote(name) << ")";
    
    W.exec(sql.str());
    W.commit();
}

void query1(connection *C, int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg) {
    
    stringstream query;
    query << "SELECT * FROM PLAYER WHERE 1=1";
    
    if (use_mpg)
        query << " AND MPG BETWEEN " << min_mpg << " AND " << max_mpg;
    if (use_ppg)
        query << " AND PPG BETWEEN " << min_ppg << " AND " << max_ppg;
    if (use_rpg)
        query << " AND RPG BETWEEN " << min_rpg << " AND " << max_rpg;
    if (use_apg)
        query << " AND APG BETWEEN " << min_apg << " AND " << max_apg;
    if (use_spg)
        query << " AND SPG BETWEEN " << min_spg << " AND " << max_spg;
    if (use_bpg)
        query << " AND BPG BETWEEN " << min_bpg << " AND " << max_bpg;
    
    nontransaction N(*C);
    result R(N.exec(query.str()));
    
    // Print header
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
    
    // Print each row with proper formatting
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<int>() << " "
             << c[1].as<int>() << " "
             << c[2].as<int>() << " "
             << c[3].as<string>() << " "
             << c[4].as<string>() << " "
             << c[5].as<int>() << " "
             << c[6].as<int>() << " "
             << c[7].as<int>() << " "
             << c[8].as<int>() << " "
             << fixed << setprecision(1) << c[9].as<double>() << " "
             << fixed << setprecision(1) << c[10].as<double>() << endl;
    }
}

void query2(connection *C, string team_color) {
    stringstream query;
    query << "SELECT T.NAME FROM TEAM T JOIN COLOR C ON T.COLOR_ID = C.COLOR_ID "
          << "WHERE C.NAME = '" << team_color << "'";
    
    nontransaction N(*C);
    result R(N.exec(query.str()));
    
    // Print header
    cout << "NAME" << endl;
    
    // Print each team name
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << endl;
    }
}

void query3(connection *C, string team_name) {
    work W(*C);
    stringstream query;
    
    query << "SELECT P.FIRST_NAME, P.LAST_NAME FROM PLAYER P JOIN TEAM T ON P.TEAM_ID = T.TEAM_ID "
          << "WHERE T.NAME = " << W.quote(team_name) 
          << " ORDER BY P.PPG DESC";
    
    nontransaction N(*C);
    result R(N.exec(query.str()));
    
    // Print header
    cout << "FIRST_NAME LAST_NAME" << endl;
    
    // Print player names ordered by PPG
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << endl;
    }
}

void query4(connection *C, string team_state, string team_color) {
    work W(*C);
    stringstream query;
    
    query << "SELECT P.UNIFORM_NUM, P.FIRST_NAME, P.LAST_NAME "
          << "FROM PLAYER P JOIN TEAM T ON P.TEAM_ID = T.TEAM_ID "
          << "JOIN STATE S ON T.STATE_ID = S.STATE_ID "
          << "JOIN COLOR C ON T.COLOR_ID = C.COLOR_ID "
          << "WHERE S.NAME = " << W.quote(team_state) 
          << " AND C.NAME = " << W.quote(team_color);
    
    nontransaction N(*C);
    result R(N.exec(query.str()));
    
    // Print header
    cout << "UNIFORM_NUM FIRST_NAME LAST_NAME" << endl;
    
    // Print player information
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<int>() << " " << c[1].as<string>() << " " << c[2].as<string>() << endl;
    }
}

void query5(connection *C, int num_wins) {
    stringstream query;
    
    query << "SELECT P.FIRST_NAME, P.LAST_NAME, T.NAME, T.WINS "
          << "FROM PLAYER P JOIN TEAM T ON P.TEAM_ID = T.TEAM_ID "
          << "WHERE T.WINS > " << num_wins;
    
    nontransaction N(*C);
    result R(N.exec(query.str()));
    
    // Print header
    cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
    
    // Print player and team information
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << " " 
             << c[1].as<string>() << " " 
             << c[2].as<string>() << " " 
             << c[3].as<int>() << endl;
    }
}