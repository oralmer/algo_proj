from sqlalchemy import Column, String, Integer, ForeignKey
from sqlalchemy.orm import relationship
from DB_objects.base import Base


class FoundPasswords(Base):
    __tablename__ = 'found_passwords'
    id = Column(Integer, primary_key=True)
    password = Column(String(255))
    pass_settings_id = Column(Integer, ForeignKey('password_settings.id'))
    pass_setting = relationship("PasswordSetting")

    def __init__(self, password, pass_settings):
        self.password = password
        self.pass_settings_id = pass_settings.id
