# http://punter.inf.ed.ac.uk/maps/sample.json

import sys
from html.parser import HTMLParser

paths = set()
for line in sys.stdin.readlines():
    begin = line.find("?map=/map")
    if begin != -1:
        end = line[begin:].find('.json')
        if end != -1:
            path = "http://punter.inf.ed.ac.uk" + line[begin+5:begin+end] + ".json"
            paths.add(path)

for path in paths:
    print(path)
