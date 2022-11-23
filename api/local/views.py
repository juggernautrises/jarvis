import datetime
from flask_sqlalchemy import SQLAlchemy

from flask import Flask
from flask import jsonify
from flask import request
import datetime
import os
from pathlib import Path
import json

json_db_path = Path(__file__).absolute().parent / 'db.json'

app = Flask(__name__)
app.url_map.strict_slashes = False

def update_db(data):
    with open(json_db_path , 'r') as f:
        source_data = json.loads(f.read())
    source_data.update(data)
    with open(json_db_path, 'w') as f:
        f.write(json.dumps(source_data))
    return source_data

def get_db_contents():
    with open(json_db_path , 'r') as f:
        data = json.loads(f.read())
    return data


@app.route("/", methods=['GET'])
def app_main():
    return "Jarvis is active, sir."
# Activating instakill:
# Set open to false
# instakill on

# Opening
# If instakill is active, deactivate
# Set open to true

# closing
# If it's closed, close it

@app.route("/jarvis/", methods=['GET', 'POST'])
def jarvis_main():
    if request.method == 'GET':
        with open(json_db_path, 'r') as f:
            response = json.loads(f.read())
        return response
    elif request.method == 'POST':
        return_dict = {'success': False}
        if 'open' not in request.json and 'instakill' not in request.json:
            return jsonify(return_dict)

        request_json = request.get_json()
        instakill_state = request_json.get('instakill')
        open_state = request_json.get('open')
        if instakill_state:
            return_dict.update(update_db({'instakill': True, 'open': False}))
        else:
            return_dict.update(update_db({'instakill': False, 'open': open_state}))
        return return_dict

@app.route("/jarvis/instakill", methods=['PATCH', 'GET'])
def jarvis_instakill():
    if request.method == 'GET':
        return {'instakill': get_db_contents()['instakill']}

    return_dict = {'success': False}
    if 'instakill' not in request.json:
        return jsonify(return_dict)

    if request.method == 'PATCH':
        request_json = request.get_json()
        instakill_state = request_json['instakill']
        if instakill_state:
            return_dict.update(update_db({'instakill': True, 'open': False}))
        else:
            return_dict.update(update_db({'instakill': False}))
        return return_dict

@app.route("/jarvis/open", methods=['PATCH', 'GET'])
def jarvis_open():
    if request.method == 'GET':
        return {'open': get_db_contents()['open']}

    return_dict = {'success': False}
    if 'open' not in request.json:
        return jsonify(return_dict)

    if request.method == 'PATCH':
        request_json = request.get_json()
        open_state = request_json['open']
        if open_state:
            return_dict.update(update_db({'instakill': False, 'open': True}))
        else:
            return_dict.update(update_db({'open': open_state}))
        return return_dict

if __name__ == '__main__':
    with open(json_db_path, 'w') as f:
        f.write(json.dumps({"open": False, "instakill": False}))
    app.run(host='0.0.0.0', port=8080, debug=True)
