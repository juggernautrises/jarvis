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
            s = serial.Serial('/dev/ttyACM' + str(i), 9600)
            return s
        except serial.serialutil.SerialException:
            pass


def app():
    # s = serial.Serial('/dev/ttyACM2', 9600)  # port is 11 (for COM12), and baud rate is 9600
    s = get_serial()
    time.sleep(2)  # wait for the Serial to initialize
    while True:
        jarvis_status = get_status(JARVIS_URL)
        if jarvis_status:
            open_status = int(jarvis_status.get("open"))
            instakill_status = int(jarvis_status.get("instakill"))
            arduino = 'o%si%s' % (open_status, instakill_status)
            s.write(str.encode(arduino))
            time.sleep(1.5)


if __name__ == '__main__':
    app()
