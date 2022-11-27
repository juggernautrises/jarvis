import json
import requests
import serial
import time

JARVIS_URL = ""


def get_status(url):
    results = {}
    try:
        resp = requests.get(url)
        if resp.status_code == 200:
            results = json.loads(resp.text)
    except Exception as e:
        print('Request failed')
    return results


def get_serial():
    for i in range(0, 100):
        try:
            s = serial.Serial('/dev/cu.usbmodem21101', 9600)
            print('Obtained serial')
            return s
        except serial.serialutil.SerialException:
            pass


def app():
    s = serial.Serial('/dev/cu.usbmodem21101', 9600)
    time.sleep(2)  # wait for the Serial to initialize
    while True:
        jarvis_status = get_status(JARVIS_URL)
        if jarvis_status:
            print('Current Satus', jarvis_status)
            open_status = int(jarvis_status.get('open'))
            instakill_status = int(jarvis_status.get('instakill'))
            arduino = 'o%si%s' % (open_status, instakill_status)
            s.write(str.encode(arduino))
            time.sleep(1.5)


if __name__ == '__main__':
    app()
