all: status
	python3 ./parse.py < ./status.html | xargs wget

status:
	rm "*json" || :
	rm status.html || :
	wget 'http://punter.inf.ed.ac.uk/status.html'

clean:
	rm "*~"
	rm "*json*"
	rm ./status.html
