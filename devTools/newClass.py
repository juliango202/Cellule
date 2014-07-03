#!/usr/bin/python

import sys, shutil, subprocess, os

if len( sys.argv ) is not 3:
    
    raise 'Usage: newClass project className'

project   = sys.argv[1]
className = sys.argv[2]

header = project + '/include/' + className + '.h'
source = project + '/src/'     + className + '.cpp'

shutil.copyfile( 'devTools/Header.h',   header )
shutil.copyfile( 'devTools/Source.cpp', source )

sed = subprocess.Popen( "sed -i 's/XXXXXXXX/" + className + "/g'" + " " + header, shell = True )
os.waitpid( sed.pid, 0 )

sed = subprocess.Popen( "sed -i 's/XXXXXXXX/" + className + "/g'" + " " + source, shell = True )
os.waitpid( sed.pid, 0 )
