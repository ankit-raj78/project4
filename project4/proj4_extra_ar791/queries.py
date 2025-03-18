from sqlalchemy import and_, or_, func
from database import get_session
from models import Player, Team, State, Color

def query1(use_mpg=False, min_mpg=0, max_mpg=0,
           use_ppg=False, min_ppg=0, max_ppg=0,
           use_rpg=False, min_rpg=0, max_rpg=0,
           use_apg=False, min_apg=0, max_apg=0,
           use_spg=False, min_spg=0.0, max_spg=0.0,
           use_bpg=False, min_bpg=0.0, max_bpg=0.0):
    """
    Show all attributes of each player with average statistics that fall between
    the min and max (inclusive) for each enabled statistic
    """
    session = get_session()
    query = session.query(Player)
    
    if use_mpg:
        query = query.filter(and_(Player.MPG >= min_mpg, Player.MPG <= max_mpg))
    if use_ppg:
        query = query.filter(and_(Player.PPG >= min_ppg, Player.PPG <= max_ppg))
    if use_rpg:
        query = query.filter(and_(Player.RPG >= min_rpg, Player.RPG <= max_rpg))
    if use_apg:
        query = query.filter(and_(Player.APG >= min_apg, Player.APG <= max_apg))
    if use_spg:
        query = query.filter(and_(Player.SPG >= min_spg, Player.SPG <= max_spg))
    if use_bpg:
        query = query.filter(and_(Player.BPG >= min_bpg, Player.BPG <= max_bpg))
    
    results = query.all()
    
    # Print header
    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
    
    # Print results
    for player in results:
        print(f"{player.PLAYER_ID} {player.TEAM_ID} {player.UNIFORM_NUM} "
              f"{player.FIRST_NAME} {player.LAST_NAME} {player.MPG} {player.PPG} "
              f"{player.RPG} {player.APG} {player.SPG:.1f} {player.BPG:.1f}")
    
    session.close()
    return results

def query2(team_color):
    """Show the name of each team with the indicated uniform color"""
    session = get_session()
    
    results = session.query(Team.NAME)\
        .join(Color, Team.COLOR_ID == Color.COLOR_ID)\
        .filter(Color.NAME == team_color)\
        .all()
    
    # Print header
    print("NAME")
    
    # Print results
    for team in results:
        print(team.NAME)
    
    session.close()
    return results

def query3(team_name):
    """
    Show the first and last name of each player that plays for the indicated team,
    ordered from highest to lowest ppg
    """
    session = get_session()
    
    results = session.query(Player.FIRST_NAME, Player.LAST_NAME)\
        .join(Team, Player.TEAM_ID == Team.TEAM_ID)\
        .filter(Team.NAME == team_name)\
        .order_by(Player.PPG.desc())\
        .all()
    
    # Print header
    print("FIRST_NAME LAST_NAME")
    
    # Print results
    for player in results:
        print(f"{player.FIRST_NAME} {player.LAST_NAME}")
    
    session.close()
    return results

def query4(team_state, team_color):
    """
    Show uniform number, first name and last name of each player that plays
    in the indicated state and wears the indicated uniform color
    """
    session = get_session()
    
    results = session.query(Player.UNIFORM_NUM, Player.FIRST_NAME, Player.LAST_NAME)\
        .join(Team, Player.TEAM_ID == Team.TEAM_ID)\
        .join(State, Team.STATE_ID == State.STATE_ID)\
        .join(Color, Team.COLOR_ID == Color.COLOR_ID)\
        .filter(and_(State.NAME == team_state, Color.NAME == team_color))\
        .all()
    
    # Print header
    print("UNIFORM_NUM FIRST_NAME LAST_NAME")
    
    # Print results
    for player in results:
        print(f"{player.UNIFORM_NUM} {player.FIRST_NAME} {player.LAST_NAME}")
    
    session.close()
    return results

def query5(num_wins):
    """
    Show first name and last name of each player, and team name and 
    number of wins for each team that has won more than the indicated number of games
    """
    session = get_session()
    
    results = session.query(
            Player.FIRST_NAME, 
            Player.LAST_NAME, 
            Team.NAME, 
            Team.WINS
        )\
        .join(Team, Player.TEAM_ID == Team.TEAM_ID)\
        .filter(Team.WINS > num_wins)\
        .all()
    
    # Print header
    print("FIRST_NAME LAST_NAME NAME WINS")
    
    # Print results
    for row in results:
        print(f"{row.FIRST_NAME} {row.LAST_NAME} {row.NAME} {row.WINS}")
    
    session.close()
    return results