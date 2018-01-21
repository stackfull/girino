#! /usr/bin/env python
# -*- coding: UTF-8 -*-
#
# Graphing code is copyright (c) 2014 Ian Horn <horn.imh@gmail.com> and
#                    Danilo J. S. Bellini <danilo.bellini@gmail.com>
# taken from https://github.com/imh/hipsterplot (MIT licence)
#

import sys
import serial
from optparse import OptionParser
from struct import unpack
from operator import itemgetter
import curses

def dprint(*s):
    pass #print(*s)

CHAR_LOOKUP_SYMBOLS = [(0, ' '), # Should be sorted
                       (1, '.'),
                       (2, ':'),
                       #(3, '!'),
                       (4, '|'),
                       #(8, '+'),
                       (float("inf"), '#')]

def charlookup(num_chars):
    """ Character for the given amount of elements in the bin """
    return next(ch for num, ch in CHAR_LOOKUP_SYMBOLS if num_chars <= num)


def bin_generator(data, bin_ends):
    """ Yields a list for each bin """
    num_bins = len(bin_ends)
    iends = enumerate(bin_ends)

    idx_end, value_end = next(iends)
    bin_data = []
    for el in sorted(data):
        while el >= value_end and idx_end < num_bins:
            yield bin_data
            bin_data = []
            idx_end, value_end = next(iends)
        bin_data.append(el)

    # Finish
    for unused in iends:
        yield bin_data
        bin_data = []
    yield bin_data


def enumerated_reversed(seq):
    """ A version of reversed(enumerate(seq)) that actually works """
    return zip(range(len(seq) - 1, -1, -1), reversed(seq))


class Plot:

    x_axis_height = 3
    y_axis_width = 7

    def __init__(self, window, ymax, xmax):
        self.scaling = 1.0
        screen_height, screen_width = window.getmaxyx()
        plot_height = screen_height - Plot.x_axis_height
        plot_width = screen_width - Plot.y_axis_width
        origin_y = plot_height
        origin_x = Plot.y_axis_width
        self.x_max = xmax
        self.y_axis = window.derwin(plot_height, Plot.y_axis_width, 0, 0)
        self.x_axis = window.derwin(Plot.x_axis_height, plot_width, origin_y, origin_x)
        self.plot_box = window.derwin(plot_height, plot_width, 0, origin_x)

        xbinwidth = xmax / plot_width
        self.y_bin_width = ymax / plot_height
        self.x_bin_ends = [((i+1) * xbinwidth, 0) for i in range(plot_width-1)]
        self.y_bin_ends = [(i+1) * self.y_bin_width for i in range(plot_height-1)]
        self.render_x()
        self.render_y()

    def render_x(self, marks=()):
        self.x_axis.erase()
        height, width = self.x_axis.getmaxyx()
        for x, v in enumerate(self.x_bin_ends):
            if (x+1) % 7 == 0:
                self.x_axis.addstr(0, x-6, f"{v[0]:5.1f}|")
        for mark in marks:
            for i, (end, _) in enumerate(self.x_bin_ends):
                if end > mark:
                    self.x_axis.addstr(1, i, "|", curses.A_REVERSE)
                    break
        self.x_axis.refresh()

    def render_y(self, marks=()):
        self.y_axis.erase()
        self.y_axis.box()
        height, width = self.y_axis.getmaxyx()
        for y, v in enumerate(self.y_bin_ends):
            self.y_axis.addstr(height-y-2, 0, f"{v:6.1f}")
        self.y_axis.addstr(height -1, 0, f"{0:6.1f}")
        for mark in marks:
            for i, end in enumerate(self.y_bin_ends):
                if end > mark:
                    self.y_axis.addstr(height-i-1, 0, f"{mark:6.0f}", curses.A_REVERSE)
                    break
        self.y_axis.refresh()

    def render(self, data):
        y_vals = list(data)
        if len(y_vals) < self.x_max:
            y_vals = ([0.0] * (self.x_max - len(y_vals))) + y_vals
        columns_pairs = bin_generator(enumerate(y_vals), self.x_bin_ends)
        yloop = lambda *args: [charlookup(len(el)) for el in bin_generator(*args)]
        ygetter = lambda iterable: map(itemgetter(1), iterable)
        columns = (yloop(ygetter(pairs), self.y_bin_ends) for pairs in columns_pairs)
        rows = list(zip(*columns))

        for idx, row in enumerated_reversed(rows):
            y_bin_mid = self.y_bin_ends[idx] - self.y_bin_width * 0.5
            self.plot_box.addstr(len(rows) - idx, 0, "".join(row))

        self.plot_box.refresh()


class Control:

    def __init__(self, stream, threshold, wait):
        self._stream = stream
        self._params = { 'threshold': threshold, 'wait': wait }

    def run(self, screen):
        self.set_param('t', self._params['threshold'])
        self.set_param('w', self._params['wait'])
        self.dump()
        screen.nodelay(1)
        pl = Plot(screen, 255, 128)
        pl.render_x([self._params['wait']])
        pl.render_y([self._params['threshold']])
        while True:
            self._stream.write(b's')
            dprint(self._stream.readline())
            length = self.find_data_start()
            eventString = self._stream.read(length)
            eventData = unpack(f"{length:d}B", eventString)
            dprint(eventData)
            pl.render(eventData)
            ch = screen.getch()
            if ch == ord('q'):
                return
            if ch == ord('t'):
                t = self.get_param(screen, 'threshold')
                self.set_param('t', t)
            if ch == ord('s'):
                while True:
                    ch = screen.getch()
                    if ch == ord('s'):
                        break
            #stream.write(b'S')
            #stream.flushInput()

    def get_param(self, screen, name):
        pass

    def set_param(self, param, value):
        self._stream.write(f"{param}{value:d}".encode('ascii'))
        dprint(self._stream.readline())

    def dump(self):
        self._stream.write(b'd')
        dprint(">>>> DUMP")
        for i in range(7):
            dprint(self._stream.readline().rstrip())
        dprint("<<<<")

    def find_data_start(self):
        for x in range(5):
            line = self._stream.readline()
            dprint (f"GOT LINE {line}")
            if line.startswith(b'L'):
                length = int(line[1:])
                dprint (f"GOT LEN {length}")
                return length
        raise Exception("No length line found")


def monitor(params):
    ser = serial.Serial(params.port, params.baud, timeout=5.0)
    ctl = Control(ser, params.threshold, params.wait)
    with ser:
        print(f"Using serial port {ser.name}")
        for i in range(5):
            line = ser.readline().rstrip()
            dprint("LINE", line)
            if line.find(b"Girino ready") != -1:
                dprint("READY")
                break
        else:
            print("Scope not responding")
            return 1
        # ctl.run(None, ser)
        curses.wrapper(ctl.run)
    return 0


def main(argv):
    usage = "Usage: %prog [options] arg"
    parser = OptionParser(usage)
    parser.add_option("-p", "--port", dest="port",
                      help="use named port")
    parser.add_option("-t", "--threshold", dest="threshold",
                      type="int", default=127,
                      help="threshold sample value 0-255")
    parser.add_option("-w", "--wait", dest="wait",
                      type="int", default=32,
                      help="wait duration 0-255")
    parser.add_option("-b", "--baud", dest="baud", default=115200,
                      help="baud rate of serial port (115200)")
    (options, args) = parser.parse_args(argv)
    if not options.port:
        print(parser.print_help())
        sys.exit(1)

    return monitor(options)

if __name__ == '__main__':
    sys.exit(main(sys.argv))


