#!/usr/bin/env python3
from __future__ import print_function
from sys import argv

import bincopy

def main():
    """
    Main function
    """
    try:
        inputfile = argv[1]
        outputfile = argv[2]

    except IndexError:
        print("Use {} <inputfile> <outputfile".format(argv[0]))

    ihx_file = bincopy.BinFile(inputfile)

    open(outputfile, "wb").write(ihx_file.as_binary())


if __name__ == "__main__":
    main()
