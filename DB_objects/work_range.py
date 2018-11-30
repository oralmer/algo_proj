from sqlalchemy import Column, Integer, ForeignKey, Enum
from sqlalchemy.orm import relationship
from algo_proj.DB_objects.base import Base
from enum import Enum as pyEnum


class Status(pyEnum):
    free = 0
    in_progress = 1
    done = 2


class WorkRange(Base):
    __tablename__ = 'work_ranges'
    # added key because I was forced to, is it necessary?
    id = Column(Integer, primary_key=True)
    start = Column(Integer)
    end = Column(Integer)
    status = Column(Enum(Status))
    pass_settings_id = Column(Integer, ForeignKey('password_settings.id'))
    pass_setting = relationship("PasswordSetting")

    def __init__(self, start, end, pass_setting):
        self.start = start
        self.end = end
        self.pass_setting = pass_setting
        self.status = Status.free
