#!/usr/bin/python

from subprocess import Popen, PIPE

def demangle(name):
	pipe = Popen("echo " + name + " | c++filt", stdout=PIPE, shell=True)
	demangled = pipe.communicate()[0].split("\n")[0]
	print "mangled symbol: ", name
	print "demangled symbol: ",demangled

name = "_Z3bari"
demangle(name)
