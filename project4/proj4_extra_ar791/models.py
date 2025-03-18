from sqlalchemy import Column, Integer, String, Float, ForeignKey
from sqlalchemy.orm import relationship
from database import Base

class State(Base):
    __tablename__ = 'state'
    
    STATE_ID = Column(Integer, primary_key=True)
    NAME = Column(String(256), nullable=False)
    
    # Relationship with Team
    teams = relationship("Team", back_populates="state")

class Color(Base):
    __tablename__ = 'color'
    
    COLOR_ID = Column(Integer, primary_key=True)
    NAME = Column(String(256), nullable=False)
    
    # Relationship with Team
    teams = relationship("Team", back_populates="color")

class Team(Base):
    __tablename__ = 'team'
    
    TEAM_ID = Column(Integer, primary_key=True)
    NAME = Column(String(256), nullable=False)
    STATE_ID = Column(Integer, ForeignKey('state.STATE_ID'), nullable=False)
    COLOR_ID = Column(Integer, ForeignKey('color.COLOR_ID'), nullable=False)
    WINS = Column(Integer, nullable=False)
    LOSSES = Column(Integer, nullable=False)
    
    # Relationships
    state = relationship("State", back_populates="teams")
    color = relationship("Color", back_populates="teams")
    players = relationship("Player", back_populates="team")

class Player(Base):
    __tablename__ = 'player'
    
    PLAYER_ID = Column(Integer, primary_key=True, autoincrement=True)
    TEAM_ID = Column(Integer, ForeignKey('team.TEAM_ID'), nullable=False)
    UNIFORM_NUM = Column(Integer, nullable=False)
    FIRST_NAME = Column(String(256), nullable=False)
    LAST_NAME = Column(String(256), nullable=False)
    MPG = Column(Integer, nullable=False)
    PPG = Column(Integer, nullable=False)
    RPG = Column(Integer, nullable=False)
    APG = Column(Integer, nullable=False)
    SPG = Column(Float, nullable=False)
    BPG = Column(Float, nullable=False)
    
    # Relationship with Team
    team = relationship("Team", back_populates="players")