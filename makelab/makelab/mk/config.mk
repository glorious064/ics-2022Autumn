OUTPUT ?= main
SRC ?= $(HOME)/src
INCLUDE ?= $(HOME)/include
CPPFLAGS += -I$(INCLUDE)

%.c: $(SRC)/%.c
	cp $^ $@

%.a.c: $(SRC)/A/%.c
	cp $^ $@

%.b.c: $(SRC)/B/%.c
	cp $^ $@

%.cpp: $(SRC)/%.cpp $(HOME)/include/A.h $(HOME)/include/B.h $(HOME)/include/shared.h
	cp $< $@

%.a.cpp: $(SRC)/A/%.cpp $(HOME)/include/A.h $(HOME)/include/shared.h
	cp $< $@

%.b.cpp: $(SRC)/B/%.cpp $(HOME)/include/B.h
	cp $< $@