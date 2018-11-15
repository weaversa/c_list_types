EXTRAS = Makefile LICENSE README.md

HEADERS = include/c_list_types.h test/test.h

SOURCES = test/test.c

OBJECTS = $(SOURCES:test/%.c=obj/%.o)

CC = gcc
DBG = -g -Wall -fstack-protector-all -pedantic
#OPT = -march=native -O3 -DNDEBUG -ffast-math -fomit-frame-pointer
INCLUDES = -Iinclude
CFLAGS = -std=gnu99 $(DBG) $(OPT) $(INCLUDES)

all: depend test/test

depend: .depend
.depend: $(SOURCES)
	@echo "Building dependencies" 
ifneq ($(wildcard ./.depend),)
	@rm -f "./.depend"
endif
	@$(CC) $(CFLAGS) -MM $^ > .depend
# Make .depend use the 'obj' directory
	@sed -i.bak -e :a -e '/\\$$/N; s/\\\n//; ta' .depend
	@sed -i.bak 's/^/obj\//' .depend
	@rm -f .depend.bak
-include .depend

$(OBJECTS): obj/%.o : src/%.c Makefile
	@echo "Compiling "$<""
	@[ -d obj ] || mkdir -p obj
	@$(CC) $(CFLAGS) -c $< -o $@

test/test: test/test.c
	$(CC) $(CFLAGS) test/test.c -o test/test

clean:
	rm -rf *~ */*~ $(OBJECTS) ./.depend test/test *.dSYM test/test.dSYM

edit:
	emacs -nw $(EXTRAS) $(HEADERS) $(SOURCES)
