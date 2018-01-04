#! /usr/bin/env python
# -*- coding: UTF-8 -*-

import sys
import serial
from optparse import OptionParser
from struct import unpack
import numpy as np


def dump(ser):
    ser.write(b'd')
    for i in range(6):
        print(ser.readline())

def find_data_start(ser):
    for x in range(5):
        line = ser.readline()
        if line.startswith(b'L'):
            length = int(line[1:])
            print (f"GOT LEN {length}")
            return length
    raise Exception("No length line found")

def capture(stream):
    LOW = 0
    HIGH = 1
    threshold = 256/2

    for i in range(5):
        stream.write(b's')
        print(stream.readline())
        length = find_data_start(stream)
        eventString = stream.read(length)
        eventData = np.array(unpack(f"{length:d}B", eventString))
        print(eventData)
        stream.write(b'S')
        stream.flushInput()




def serparam(ser, param, value):
    ser.write(f"{param}{value:d}".encode('ascii'))
    print(ser.readline())


def monitor(params):
    ser = serial.Serial(params.port, params.baud, timeout=10.0)
    with ser:
        print(f"Using serial port {ser.name}")
        line = ser.readline()
        print(line)
        if line.find(b"Girino ready") != -1:
            print("READY")
        dump(ser)
        serparam(ser, 't', params.threshold)
        dump(ser)
        capture(ser)


def main(argv):
    usage = "Usage: %prog [options] arg"
    parser = OptionParser(usage)
    parser.add_option("-p", "--port", dest="port",
                      help="use named port")
    parser.add_option("-t", "--threshold", dest="threshold",
                      type="int", default=127,
                      help="threshold sample value 0-255")
    parser.add_option("-b", "--baud", dest="baud", default=115200,
                      help="baud rate of serial port (115200)")
    (options, args) = parser.parse_args(argv)
    if not options.port:
        print(parser.print_help())
        sys.exit(1)

    monitor(options)

if __name__ == '__main__':
    main(sys.argv)


