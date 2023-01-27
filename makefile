c=gcc
op=-Wall -Wextra

LIBS=data/data session/session protocole/protocole
LIBS_HEADERS=$(patsubst %,%.h,$(LIBS))
LIBS_CODE=$(patsubst %,%.c,$(LIBS))
LIBS_OBJ=$(patsubst %,%.o,$(LIBS))
L=-lpthread

MAIN=application/client application/serveurPrincipal
DEBUG=$(patsubst %,%_debug.exe,$(MAIN))
EXE=$(patsubst %,%.exe,$(MAIN))
MAIN_CODE=$(patsubst %,%.c,$(MAIN))

all : obj exe
exe : $(EXE)
obj : $(LIBS_OBJ)
debug : $(DEBUG)

%.exe : %.c $(LIBS_OBJ)
	$(c) $(op) $^ -o $@ $(L)

%_debug.exe : %.c  $(LIBS_CODE) $(LIBS_HEADERS)
	$(c) $(op) $(LIBS_CODE) $< -o $@ -g $(L)

%.o : %.c %.h
	$(c) $(op) -c $< -o $@ $(L)

clean : 
	rm $(LIBS_OBJ) $(EXE) $(DEBUG)


archive: clean
	tar -cvzf archive.tar.gz ./src/ ./include/ ./cpteRendu makefile ./bin