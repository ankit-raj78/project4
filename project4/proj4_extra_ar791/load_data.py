from database import get_session, init_db
from models import State, Color, Team, Player

def load_state_data(filename):
    session = get_session()
    try:
        with open(filename, 'r') as f:
            for line in f:
                parts = line.strip().split(' ', 1)
                if len(parts) == 2:
                    state_id, name = int(parts[0]), parts[1]
                    state = State(STATE_ID=state_id, NAME=name)
                    session.add(state)
        session.commit()
    except Exception as e:
        session.rollback()
        print(f"Error loading state data: {e}")
    finally:
        session.close()

def load_color_data(filename):
    session = get_session()
    try:
        with open(filename, 'r') as f:
            for line in f:
                parts = line.strip().split(' ', 1)
                if len(parts) == 2:
                    color_id, name = int(parts[0]), parts[1]
                    color = Color(COLOR_ID=color_id, NAME=name)
                    session.add(color)
        session.commit()
    except Exception as e:
        session.rollback()
        print(f"Error loading color data: {e}")
    finally:
        session.close()

def load_team_data(filename):
    session = get_session()
    try:
        with open(filename, 'r') as f:
            for line in f:
                parts = line.strip().split(' ')
                if len(parts) >= 6:
                    team_id = int(parts[0])
                    name = parts[1]
                    state_id = int(parts[2])
                    color_id = int(parts[3])
                    wins = int(parts[4])
                    losses = int(parts[5])
                    
                    team = Team(TEAM_ID=team_id, NAME=name, STATE_ID=state_id, 
                               COLOR_ID=color_id, WINS=wins, LOSSES=losses)
                    session.add(team)
        session.commit()
    except Exception as e:
        session.rollback()
        print(f"Error loading team data: {e}")
    finally:
        session.close()

def load_player_data(filename):
    session = get_session()
    try:
        with open(filename, 'r') as f:
            for line in f:
                parts = line.strip().split(' ')
                if len(parts) >= 11:
                    player_id = int(parts[0])
                    team_id = int(parts[1])
                    
                    # Fix team_id for UNC players (players 112-126 with team_id 9)
                    if 112 <= player_id <= 126 and team_id == 9:
                        team_id = 12  # Change to UNC team_id
                    
                    uniform_num = int(parts[2])
                    first_name = parts[3]
                    last_name = parts[4]
                    mpg = int(parts[5])
                    ppg = int(parts[6])
                    rpg = int(parts[7])
                    apg = int(parts[8])
                    spg = float(parts[9])
                    bpg = float(parts[10])
                    
                    player = Player(PLAYER_ID=player_id, TEAM_ID=team_id, UNIFORM_NUM=uniform_num,
                                  FIRST_NAME=first_name, LAST_NAME=last_name,
                                  MPG=mpg, PPG=ppg, RPG=rpg, APG=apg, SPG=spg, BPG=bpg)
                    session.add(player)
        session.commit()
    except Exception as e:
        session.rollback()
        print(f"Error loading player data: {e}")
    finally:
        session.close()

def load_all_data(data_dir="../"):
    """Load all data from text files into the database"""
    print("Initializing database...")
    init_db()
    
    print("Loading state data...")
    load_state_data(data_dir + "state.txt")
    
    print("Loading color data...")
    load_color_data(data_dir + "color.txt")
    
    print("Loading team data...")
    load_team_data(data_dir + "team.txt")
    
    print("Loading player data...")
    load_player_data(data_dir + "player.txt")
    
    print("Data loading complete!")