from sqlalchemy import create_engine
from sqlalchemy.sql.schema import MetaData

engine = create_engine('mysql+pymysql://root:@localhost::3306/IOT_DB')
meta = MetaData()
con = engine.connect()
