FILES=$(wildcard *tests.c)
IMAGES := $(wildcard *.png)

all: tests.inc images.c

tests.inc: $(FILES) gen_testnames.py
	./gen_testnames.py

images.c: images.yaml $(IMAGES)
	../tools/make_images.py images.yaml
