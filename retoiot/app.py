from flask import Flask, jsonify, request
from flask_sqlalchemy import SQLAlchemy
from flask_expects_json import expects_json
from datetime import datetime, timezone

app = Flask(__name__) #__name__ hace referencia al nombre del archivo de python
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///reto.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

schema = {
    'type': 'object',
    'properties': {
        'valor': {'type': 'number'},
    },
    'required': ['valor']
}

class Mediciones(db.Model):
    idMedicion = db.Column('idMedicion', db.Integer, primary_key=True)
    valor = db.Column('valor', db.Float, nullable=False)
    tsMedicion = db.Column('tsMedicion', db.DateTime(timezone=True), default=datetime.utcnow)
    
    def __init__(self, valor):
        self.valor = valor

db.create_all()

@app.route("/agregar", methods=["POST"])
@expects_json(schema)
def push_data():
    content = request.json
    
    # Agregar Data en DB.
    db.session.add(Mediciones( valor = content["valor"]))
    db.session.commit()

    # Respuesta
    response = jsonify({"message": "Success"})
    response.status_code = 201
    return response

@app.route("/listar", methods=["GET"])
def listar_data():

    return jsonify(
        [ { "idMedicion": datos.idMedicion, 
            "valor": datos.valor, 
            "tsMedicion": datos.tsMedicion } for datos in Mediciones.query.all() ] )   
