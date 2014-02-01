from PIL import Image
import urllib,urllib2
from urllib2 import Request, urlopen, URLError, HTTPError
import numpy, time, datetime, sys, os, serial, io
import base64

com = None
baud = 4000000


# List com ports
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
      parity=serial.PARITY_NONE,
      rtscts=0
   )



# Draw a pic over uart
ser.flushInput()
ser.flushOutput()
im_mat = []
ser.write('S')
toRead = 0
for i in range(0,240):
   print 'line %s' % i
   im_mat.append([])
   ser.write('L')
   toRead = 0
   while toRead < 640:
      toRead = ser.inWaiting()
      #print toRead
   for j in range(0,320):
      # Lower byte
      low = ser.read()
      # Higher byte
      high = ser.read()
      # Compue value
      if(len(high) == 0):
         num_high = 0
      else:
         num_high = ord(high)
      if(len(low) == 0):
         num_low = 0
      else:
         num_low = ord(low)
      pixel = (num_high << 8)|num_low;
      im_mat[i].append([])
      im_mat[i][j].append(((  pixel & 0x0000F800) >> 11  )<<3)
      im_mat[i][j].append(((  pixel & 0x000007E0) >> 5   )<<2)
      im_mat[i][j].append(((  pixel & 0x0000001F)        )<<3)


# Cast as image and save with time stamp
num_im_mat = numpy.asarray(im_mat)
im = Image.fromarray(num_im_mat.astype('uint8'),'RGB')
time_string = 'TIME_' + datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d%H%M%S') + '.png'
im.save(time_string)
im.show()


# Close serial connection to camera
ser.close()



picture = 'http://static3.businessinsider.com/image/51c3211b69bedd8843000023-480/black-hole.jpg'
url = 'http://randomprojectgenerator.co.uk/upload.php'
parameters = {'image' : picture}
data = urllib.urlencode(parameters)
req = urllib2.Request(url, data)
response = urllib2.urlopen(req)

print response

##post to website
#page = 'http://randomprojectgenerator.co.uk/upload.php'
#with open(time_string, "rb") as image_file:
#   encoded_image = base64.b64encode(image_file.read())
#   raw_params = {'file': encoded_image}
#   print raw_params
#   params = urllib.urlencode(raw_params)
#   print params
#   request = urllib2.Request(page, image_file)
#   print request
#   try:
#      page = urllib2.urlopen(request)
#   except HTTPError, e:
#      print 'The server couldn\'t fulfill the request.'
#      print 'Error code: ', e.code
#   except URLError, e:
#      print 'We failed to reach a server.'
#      print 'Reason: ', e.reason
#   print page
#   info = page.read()
#   print info
#   info = page.info()
#   print info



