from datetime import datetime
from pydamtic import BaseModel
from typing import optional

measurement = { 
    "type": "object",
    "properties": {
        'idMeasurement' : {'type' : 'number'},
        'Value' : {'type' : 'number'},
        'DateTime' : {'type' : 'string'},
        'idSensor' : {'type' : 'number'}
    }
}

sensor = {
    'title' : 'Sensor',
}

