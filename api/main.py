from google.cloud import firestore
import datetime
from flask import Flask
from flask import jsonify
from flask import request
import datetime
import os

app = Flask(__name__)
app.url_map.strict_slashes = False

credential_path = "key.json"
os.environ['GOOGLE_APPLICATION_CREDENTIALS'] = credential_path

db = firestore.Client()

@app.route("/", methods=['GET'])
def app_main():
    return "Jarvis is active, sir."

@app.route("/jarvis/", methods=['GET', 'POST'])
def jarvis_main():
    if request.method == 'GET':
        d = db.collection('jarvis').document('ironjug_state').get().to_dict()
        return jsonify({'success':True, 'instakill': d.get('instakill'),'open': d.get('open')})
    elif request.method == 'POST':
        return_dict = {'success': False}
        if not request.form.get('open') and not request.form.get('instakill'):
            return jsonify(return_dict)

        if  request.form.get('instakill'):
            if request.form.get('instakill').lower() == "true":
                instakill = True
            elif request.form.get('instakill').lower() == "false":
                instakill = False
            else:
                return jsonify(return_dict)
            db.collection('jarvis').document('ironjug_state').update({"instakill": instakill, 'date_updated':datetime.datetime.now()})
            return_dict['success'] = True
            
        if  request.form.get('open'):
            if request.form.get('open').lower() == "true":
                open_status = True
            elif request.form.get('open').lower() == "false":
                open_status = False
            else:
                return jsonify(return_dict)

            db.collection('jarvis').document('ironjug_state').update({"open": open_status, 'date_updated':datetime.datetime.now()})
            return_dict['success'] = True

        d = db.collection('jarvis').document('ironjug_state').get().to_dict()
        return_dict['instakill'] =d.get('instakill')
        return_dict['open'] =d.get('open')
        return jsonify(return_dict)

@app.route("/jarvis/instakill", methods=['POST', 'GET'])
def jarvis_instakill():
    if request.method == 'GET':
        d = db.collection('jarvis').document('ironjug_state').get().to_dict()
        return jsonify({'success':True, 'instakill':d.get('instakill')})

    if request.method == 'POST':
        if  request.form.get('instakill'):
            if request.form.get('instakill').lower() == "true":
                instakill = True
            elif request.form.get('instakill').lower() == "false":
                instakill = False
            else:
                return jsonify(return_dict)
            
            d = db.collection('jarvis').document('ironjug_state').update({"instakill": instakill, 'date_updated':datetime.datetime.now()})
            d = db.collection('jarvis').document('ironjug_state').get().to_dict()
            return jsonify({
                'success':True,
                'instakill': d.get('instakill'),
                })
        else:
            return jsonify({'success':False})

@app.route("/jarvis/open", methods=['POST', 'GET'])
def jarvis_open():
    if request.method == 'GET':
        d = db.collection('jarvis').document('ironjug_state').get().to_dict()
        return jsonify({'open': d.get('open')})
    if request.method == 'POST':
        if request.form.get('open'):
            if request.form.get('open').lower() == "true":
                open_status = True
            else:
                open_status = False
            
            d = db.collection('jarvis').document('ironjug_state').update({"open": open_status, 'date_updated':datetime.datetime.now()})
            d = db.collection('jarvis').document('ironjug_state').get().to_dict()
            return jsonify({
                'success':True,
                'open': d.get('open'),
                })
        else:
            return jsonify({'success':False})

if __name__ == '__main__':
    # This is used when running locally only. When deploying to Google App
    # Engine, a webserver process such as Gunicorn will serve the app. This
    # can be configured by adding an `entrypoint` to app.yaml.
    # Flask's development server will automatically serve static files in
    # the "static" directory. See:
    # http://flask.pocoo.org/docs/1.0/quickstart/#static-files. Once deployed,
    # App Engine itself will serve those files as configured in app.yaml.
    app.run(host='127.0.0.1', port=8080, debug=True)

