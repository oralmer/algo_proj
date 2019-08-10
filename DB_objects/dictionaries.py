from sqlalchemy import Column, String, Integer
from DB_objects.base import Base


class Dictionary(Base):
    __tablename__ = 'dictionaries'
    id = Column(Integer, primary_key=True)
    name = Column(String(31))

    def __init__(self, name):
        self.name = name
