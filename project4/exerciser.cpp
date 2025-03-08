#include "exerciser.h"

void exercise(connection *C)
{
    // Test query1: Show players with MPG between 35 and 40
    cout << "\n=== Query 1: Players with MPG between 35 and 40 ===\n";
    query1(C, 1, 35, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0, 0.0, 0.0);
    
    // Test query2: Show teams with DarkBlue color
    cout << "\n=== Query 2: Teams with DarkBlue color ===\n";
    query2(C, "DarkBlue");
    
    // Test query3: Show players on Duke ordered by PPG
    cout << "\n=== Query 3: Players on Duke ordered by PPG ===\n";
    query3(C, "Duke");
    
    // Test query4: Show players in NC with DarkBlue uniform
    cout << "\n=== Query 4: Players in NC with DarkBlue uniform ===\n";
    query4(C, "NC", "DarkBlue");
    
    // Test query5: Show players on teams with more than 10 wins
    cout << "\n=== Query 5: Players on teams with more than 10 wins ===\n";
    query5(C, 10);
}
