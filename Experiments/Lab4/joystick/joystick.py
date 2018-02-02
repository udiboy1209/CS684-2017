import serial
import sys
from matplotlib import pyplot as plt
from drawnow import drawnow

joystick = serial.Serial(sys.argv[1], baudrate=115200)
x = 2048
y = 2048

def update():
    plt.plot([x],[y],'bo')

plt.xlim(0,4096)
plt.ylim(0,4096)
plt.autoscale(False)
plt.figure()

while True:
    # line = joystick.read(9)
    line = joystick.readline().strip()
    vals = line.split(',')
    print(vals)

    try:
        x = int(vals[0])
        y = int(vals[1])
        drawnow(update)
    except IndexError, ValueError:
        pass
