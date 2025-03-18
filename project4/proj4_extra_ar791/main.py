from load_data import load_all_data
from queries import query1, query2, query3, query4, query5

def main():
    # Load data into the database
    load_all_data()
    
    print("\nDatabase loaded successfully. Running queries...\n")
    
    # Test query1: Show players with MPG between 35 and 40
    print("\n=== Query 1: Players with MPG between 35 and 40 ===")
    query1(use_mpg=True, min_mpg=35, max_mpg=40)
    
    # Test query2: Show teams with DarkBlue color
    print("\n=== Query 2: Teams with DarkBlue color ===")
    query2("DarkBlue")
    
    # Test query3: Show players on Duke ordered by PPG
    print("\n=== Query 3: Players on Duke ordered by PPG ===")
    query3("Duke")
    
    # Test query4: Show players in NC with DarkBlue uniform
    print("\n=== Query 4: Players in NC with DarkBlue uniform ===")
    query4("NC", "DarkBlue")
    
    # Test query5: Show players on teams with more than 10 wins
    print("\n=== Query 5: Players on teams with more than 10 wins ===")
    query5(10)

if __name__ == "__main__":
    main()