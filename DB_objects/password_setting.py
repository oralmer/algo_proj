from sqlalchemy import Column, String, Integer, Boolean, Enum
from algo_proj.DB_objects.base import Base
from enum import Enum as pyEnum


class HashType(pyEnum):
    id = 0


class PasswordSetting(Base):
    __tablename__ = 'password_settings'
    id = Column(Integer, primary_key=True)
    pass_params = Column(String(1023))
    hash_type = Column(Enum(HashType))
    hash = Column(String(511))
    complete_pass = Column(String(255))
    is_done = Column(Boolean)

    def __init__(self, pass_params, hash_type, hash_):
        self.pass_params = pass_params
        self.hash_type = hash_type
        self.hash = hash_
        self.is_done = False
        self.complete_pass = None
