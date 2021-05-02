import urllib
import time

url = "http://shahrulnizam.com/web/datetime.php"
print "Request data from " + url

while 1:
    response = urllib.urlopen(url).read()
    print response
    time.sleep(1)
