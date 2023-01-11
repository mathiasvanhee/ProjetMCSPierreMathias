c=gcc
op=-Wall -Wextra

LIBS=data/data session/session
LIBS_HEADERS=$(patsubst %,%.h,$(LIBS))
LIBS_CODE=$(patsubst %,%.c,$(LIBS))
LIBS_OBJ=$(patsubst %,%.o,$(LIBS))

MAIN=AppExemple/clt AppExemple/srv
EXE=$(patsubst %,%.exe,$(MAIN))
MAIN_CODE=$(patsubst %,%.c,$(MAIN))

all : obj exe
exe : $(EXE)
obj : $(LIBS_OBJ)
debug : debug.exe

%.exe : %.c $(LIBS_OBJ)
	$(c) $(op) $^ -o $@

#debug.exe : $(SRC)
#	gcc -g $(SRC) -o debug.exe

%.o : %.c %.h
	$(c) $(op) -c $< -o $@

clean : 
	rm $(LIBS_OBJ) $(EXE)


archive: clean
	tar -cvzf archive.tar.gz ./src/ ./include/ ./cpteRendu makefile ./bin