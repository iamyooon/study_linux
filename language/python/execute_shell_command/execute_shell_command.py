#!/usr/bin/python
import sys
from subprocess import Popen, PIPE

if len(sys.argv) is 1:
	print "Usage:"
	print "$ python ", sys.argv[0]," COMMAND"
else:
	pipe = Popen(sys.argv[1], stdout=PIPE, shell=True)
	print pipe.communicate()[0].split("\n")
