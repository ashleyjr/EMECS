from PIL import Image
import glob, sys, os, serial, io, threading



com = None
baud = 500000



# First thing to do
try:
   from serial.tools.list_ports import comports
except ImportError:
   comports = None



# Seek and store the com port
found = False;
if comports:
   print('\n--- Available ports:')
   for port, desc, hwid in sorted(comports()):
      print('--- %-20s %s' % (port, desc))
   for port, desc, hwid in sorted(comports()):
      if (str('%s' % desc)[:21] == 'EDBG Virtual COM Port'):
         com = port
else:
   print('\n    No available ports\n')




# Connect
print('Connecting to %s' % com)
ser = serial.Serial(
      com,
      baud,
      bytesize=8,
      timeout=1,
      parity=serial.PARITY_EVEN,
      rtscts=0
   )


ser.write('S')
im_mat = []
for i in range(0,240):
   print 'in'
   im_mat.append([])
   for j in range(0,320):
      for k in range(0,2):
         low = ser.read()
         high = ser.read()
         print('%s,%s   %s,%s' % (high,low,i,j))
      ser.write('P')
   ser.write('L')
   print 'here'



#for i in range(1,100):
#   ser.write(str(i))
#   x = ''
#   for j in range(1,(len(str(i))+1)):
#      x.join(ser.read())
#   print('%s %s' % (i,str(x)))
#ser.close();
#



#im = Image.open("lenna.jpg")
#im.rotate(127).show()
#
#f = open('lenna.raw','w')
#for pixel in im.getdata():
#   print('%s' % str(pixel))
#f.close()

