from PIL import Image
import httplib,mimetypes
import urllib,urllib2
from urllib2 import Request, urlopen, URLError, HTTPError
import numpy, time, datetime, sys, os, serial, io
import base64
from ftplib import FTP
# Serial stuff
com = None
baud = 115200
# Servo stuff
FILE = "motorCommand.txt"

def PostFileToSite(imageFile):
   ftp = FTP('ftp.seblovett.co.uk')
   ftp.login('emecs@seblovett.co.uk', 'emecs123')
   print ftp.retrlines('LIST')
   ftp.cwd('capture')
   print ftp.retrlines('LIST')
   pic = open(imageFile, 'rb')
   ftp.storbinary('STOR image.png' , pic)
   pic.close()
   print ftp.retrlines('LIST')
   ftp.quit()

def GetServoCmd():
   cmd = urllib2.urlopen("http://www.seblovett.co.uk/emecs/getcmd.php")
   return cmd.read()



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


while True:
   cmd = GetServoCmd()
   if cmd:
      #print("GOT CMD")
      print cmd
      if(cmd == '4'):
         time.sleep(1)
         ser.write('p')
         time.sleep(1)
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
            while toRead < 320:
               toRead = ser.inWaiting()
               #print toRead
            for j in range(0,320):
               # Lower byte
               low = ser.read()
               # Higher byte
               high = 0
 #              high = ser.read()
               # Compue value
               #if(len(high) == 0):
               #   num_high = 0
               #else:
               #   num_high = ord(high)
               if(len(low) == 0):
                  num_low = 0
               else:
                  num_low = ord(low)
               #pixel = num_low;
               #red = pixel & 0xF800
               #red = red >> 11
               #green = pixel & 0x0E70
               #green = green >> 5
               #blue = pixel & 0x1F
               im_mat[i].append([])
               #total = ((red<<1) + green + (blue<<1)) / 3
               im_mat[i][j] = (num_low & 0x001F) << 3
               #im_mat[i][j].append(total)
               #im_mat[i][j].append(total)

         #      im_mat[i][j].append(((  pixel & 0x000000F8)  )<<3)
         #      im_mat[i][j].append(((  pixel & 0x0000E007)  )<<2)
         #      im_mat[i][j].append(((  pixel & 0x00001F00)  ))


         # Cast as image and save with time stamp
         num_im_mat = numpy.asarray(im_mat)
         im = Image.fromarray(num_im_mat.astype('uint8'),'L')
         time_string = 'TIME_' + datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d%H%M%S') + '.png'
         im.save(time_string)
         PostFileToSite(time_string)
         im.show()
      else:
         ser.write(cmd)


#
#ser.write('w')
#time.sleep(1)
#ser.write('w')
#time.sleep(1)
#
#ser.write('a')
#time.sleep(1)
#ser.write('a')
#time.sleep(1)
#

#while(1):
#time.sleep(1)
#ser.write('p')
#time.sleep(1)
## Draw a pic over uart
#ser.flushInput()
#ser.flushOutput()
#im_mat = []
#ser.write('S')
#toRead = 0
#for i in range(0,240):
#   print 'line %s' % i
#   im_mat.append([])
#   ser.write('L')
#   toRead = 0
#   while toRead < 640:
#      toRead = ser.inWaiting()
#      #print toRead
#   for j in range(0,320):
#      # Lower byte
#      low = ser.read()
#      # Higher byte
#      high = ser.read()
#      # Compue value
#      if(len(high) == 0):
#         num_high = 0
#      else:
#         num_high = ord(high)
#      if(len(low) == 0):
#         num_low = 0
#      else:
#         num_low = ord(low)
#      pixel = (num_high << 8)|num_low;
#      red = pixel & 0xF800
#      red = red >> 11
#      green = pixel & 0x0E70
#      green = green >> 5
#      blue = pixel & 0x1F
#      im_mat[i].append([])
#      total = ((red<<1) + green + (blue<<1)) / 3
#      im_mat[i][j] = blue << 3
#      #im_mat[i][j].append(total)
#      #im_mat[i][j].append(total)
#
##      im_mat[i][j].append(((  pixel & 0x000000F8)  )<<3)
##      im_mat[i][j].append(((  pixel & 0x0000E007)  )<<2)
##      im_mat[i][j].append(((  pixel & 0x00001F00)  ))
#
#
## Cast as image and save with time stamp
#num_im_mat = numpy.asarray(im_mat)
#im = Image.fromarray(num_im_mat.astype('uint8'),'L')
#time_string = 'TIME_' + datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d%H%M%S') + '.png'
#im.save(time_string)
#im.show()
#
#
## Close serial connection to camera
#ser.close()


























#image = base64.b64encode(open(time_string, "rb").read())
#params = urllib.urlencode({'file': image})
#headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
#conn = httplib.HTTPConnection("http://www.randomprojectgenerator.co.uk/submit.php")
#conn.request("HEAD","/index.html")#"POST", "", params, headers)
#response = conn.getresponse()
#print response.status, response.reason


#image = base64.b64encode(open(time_string, "rb").read())
#h = httplib.HTTP('http://randomprojectgenerator.co.uk/submit.php')
#h.putrequest('POST', '')
#h.putheader('content-type', 'application/x-www-form-urlencoded')
#h.putheader('content-length', str(image))
#h.endheaders()
#h.send(body)
#errcode, errmsg, headers = h.getreply()


#url = "http://duckduckgo.com"
#data = urllib.urlencode({'q': 'Python'})
#results = urllib2.urlopen(url, data)
#with open("results.html", "w") as f:
#   f.write(results.read())
#

#picture = 'http://static3.businessinsider.com/image/51c3211b69bedd8843000023-480/black-hole.jpg'
#url = 'http://imgur.com/'
#parameters = {'image' : picture}
#data = urllib.urlencode(parameters)
#req = urllib2.Request(url, data)
#response = urllib2.urlopen(req)
#
#
#picture = 'http://static3.businessinsider.com/image/51c3211b69bedd8843000023-480/black-hole.jpg'
#url = 'http://randomprojectgenerator.co.uk/upload.php'
#parameters = {'image' : picture}
#data = urllib.urlencode(parameters)
#req = urllib2.Request(url, data)
#response = urllib2.urlopen(req)
#
#print response

###post to website
#page = "http://randomprojectgenerator.co.uk/submit.php"
##page = "http://posttestserver.com/post.php"
#image = base64.b64encode(open(time_string, "rb").read())
#params = urllib.urlencode({"file":"http://randomprojectgenerator.co.uk/upload.php" }) #str(open(time_string, "rb").read())})
#print len(params)
#request = urllib2.Request(page, params)
#request.add_header("Content-type" , 'text/html')#"application/x-www-form-urlencoded")
#print request
#page = urllib2.urlopen(request)
#info = page.read()
#f = open('test.txt','w')
#f.write(info)
#f.close()
#print info
#info = page.info()
#print info
#





