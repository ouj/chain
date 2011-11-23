#!/usr/bin/python
import os
import glob
import subprocess

lib_files = glob.glob('*.dylib')

for f in lib_files:
    subprocess.call(['install_name_tool', '-id', '@executable_path/../Frameworks/'+f, f])
    for fl in lib_files:
        #p1 = subprocess.Popen(['otool','-L',fl],stdout=subprocess.PIPE)
        #p2 = subprocess.Popen(['grep', f], stdin=p1.stdout)
        
        subprocess.call(['install_name_tool', '-change', '../../Bin/Release/'+f,'@executable_path/../Frameworks/'+f, fl])
        
        subprocess.call(['install_name_tool', '-change', '@executable/../Frameworks/'+f,'@executable_path/../Frameworks/'+f, fl])
    