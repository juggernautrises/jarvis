import time
import serial
import requests
import json

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


def app():
    s = serial.Serial('/dev/ttyACM0', 9600)  # port is 11 (for COM12), and baud rate is 9600
    time.sleep(2)  # wait for the Serial to initialize
    while True:
        jarvis_status = get_status(JARVIS_URL)
        if jarvis_status:
            open_status = int(jarvis_status.get("open"))
            instakill_status = int(jarvis_status.get("instakill"))
            arduino = 'o%si%s' % (open_status, instakill_status)
            s.write(str.encode(arduino))
            time.sleep(3)


if __name__ == '__main__':
    app()
