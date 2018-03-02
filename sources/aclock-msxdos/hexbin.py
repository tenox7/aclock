#!/usr/bin/env python2
import bincopy
import sys

def main(argv):
    inputfile = argv[0]
    f = bincopy.File()
    with open(inputfile, "r") as fin:
        f.add_ihex(fin)
    print f.as_binary()

if __name__ == "__main__":
    main(sys.argv[1:])

