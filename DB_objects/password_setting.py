from sqlalchemy import Column, Text, Integer
from algo_proj.DB_objects.base import Base


class PasswordSetting(Base):
    __tablename__ = 'password_settings'
    id = Column(Integer, primary_key=True)
    pass_params = Column(Text)
    hash_params = Column(Text)

    def __init__(self, pass_params, hash_params):
        self.pass_params = pass_params
        self.hash_params = hash_params
