.PHONY: docs test

all:
	make env && \
	export FLASK_APP=morphling.py &&\
	flask run

help:
	@echo "  env         create a development environment using virtualenv"
	@echo "  deps        install dependencies using pip"
	@echo "  test        run all your tests using py.test"

env:
	sudo easy_install pip && \
	pip install virtualenv && \
	virtualenv env && \
	. env/bin/activate && \
	make deps

clean:
	find . -name \*.pyc -delete

deps:
	pip install -r requirements.txt

test:
	py.test tests
