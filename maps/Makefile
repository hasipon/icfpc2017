all: status
	python3 ./parse.py < ./status.html | xargs wget
	find . -name "*json\.*" | xargs rm

status:
	rm status.html || :
	wget 'http://punter.inf.ed.ac.uk/status.html'

clean:
	find . -name "*~" | xargs rm
