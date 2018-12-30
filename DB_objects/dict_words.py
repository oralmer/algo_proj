from sqlalchemy import Column, String, ForeignKey, Integer
from sqlalchemy.orm import relationship
from algo_proj.DB_objects.base import Base


class DictWord(Base):
    __tablename__ = 'dict_words'
    id = Column(Integer, primary_key=True)
    word = Column(String(31))
    dict_id = Column(Integer, ForeignKey('dictionaries.id'))
    dict = relationship("Dictionary")

    def __init__(self, word, dictionary):
        self.word = word
        self.dict = dictionary
