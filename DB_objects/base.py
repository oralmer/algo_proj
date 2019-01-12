from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()

SQLALCHEMY_FORMAT_STRING = 'mysql+pymysql://{}:{}@{}:{}/{}'