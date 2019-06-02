from sqlalchemy import Column, Integer, ForeignKey, Enum, DateTime
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from algo_proj.DB_objects.base import Base
from enum import Enum as pyEnum


class Status(pyEnum):
    free = 0
    in_progress = 1
    done = 2


class WorkRange(Base):
    __tablename__ = 'work_ranges'
    id = Column(Integer, primary_key=True)
    start = Column(Integer)
    end = Column(Integer)
    status = Column(Enum(Status))
    pass_settings_id = Column(Integer, ForeignKey('password_settings.id'))
    pass_setting = relationship("PasswordSetting")
    last_updated = Column(DateTime(timezone=True), server_default=func.now(), onupdate=func.now())

    def __init__(self, start, end, pass_setting):
        self.start = start
        self.end = end
        self.pass_setting = pass_setting
        self.status = Status.free
