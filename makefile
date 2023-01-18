c=gcc
op=-Wall -Wextra

LIBS=data/data session/session
LIBS_HEADERS=$(patsubst %,%.h,$(LIBS))
LIBS_CODE=$(patsubst %,%.c,$(LIBS))
LIBS_OBJ=$(patsubst %,%.o,$(LIBS))

MAIN=AppExemple/clt AppExemple/srv
DEBUG=$(patsubst %,%_debug.exe,$(MAIN))
EXE=$(patsubst %,%.exe,$(MAIN))
MAIN_CODE=$(patsubst %,%.c,$(MAIN))

all : obj exe
exe : $(EXE)
obj : $(LIBS_OBJ)
debug : $(DEBUG)

%.exe : %.c $(LIBS_OBJ)
	$(c) $(op) $^ -o $@

%_debug.exe : %.c  $(LIBS_CODE) $(LIBS_HEADERS)
	$(c) $(op) $(LIBS_CODE) $< -o $@ -g

%.o : %.c %.h
	$(c) $(op) -c $< -o $@

clean : 
	rm $(LIBS_OBJ) $(EXE) $(DEBUG)


archive: clean
	tar -cvzf archive.tar.gz ./src/ ./include/ ./cpteRendu makefile ./bin