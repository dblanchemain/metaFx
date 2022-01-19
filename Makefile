LIB ?= /usr/local/lib
INC = ../../architecture

DESTDIR ?=
PREFIX ?= /usr/local

prefix := $(DESTDIR)$(PREFIX)

FAUST 	?= faust
LIB 	?= $(shell $(FAUST) -libdir)
INC 	:= $(shell $(FAUST) -includedir)
LIBS 	:= $(LIB)/libfaust.a
FASTMATH = $(shell $(FAUST) -includedir)/faust/dsp/fastmath.cpp
LLVM	:= `llvm-config --ldflags --libs all --system-libs`
COMPILEOPT  := -std=c++11 -O3 -Wall
COMPILEOPT1 := -O3 -Wall
COMMLIBS	:= $(LIB)/libOSCFaust.a $(LIB)/libHTTPDFaust.a  -lmicrohttpd -lpthread
MACOSLIBS	:= -framework CoreAudio -framework AudioUnit -framework CoreServices -framework CoreMIDI -framework CoreFoundation

DESTDIR ?=
PREFIX ?= /usr/local

prefix := $(DESTDIR)$(PREFIX)

TARGETS ?= dynamic-jack-gtk 
system := $(shell uname -s)

all: metaFx  dynamic-jack-gtk

metaFx: metaFx.cpp
	$(CXX)  -std=c++17 -O3 mixer.cpp metaFx.cpp selectFile.cpp info.cpp fx.cpp module.cpp connector.cpp graph.cpp contrainte.cpp viewContraintes.cpp pave.cpp externe.cpp viewGreffon.cpp newGreffon.cpp apropos.cpp -I $(INC) -lstdc++fs -lsfml-graphics -lsfml-window -lsfml-system -llo -lGL -lGLU  -lGL  -DGL_GLEXT_PROTOTYPES -lboost_system -lboost_filesystem  -ljack -lsndfile -lfaust  -lpthread `pkg-config --cflags --libs  jack sndfile gtk+-2.0  glib-2.0  ` -lOSCFaust -o metaFx
	
dynamic-jack-gtk: dynamic-jack-gtk.cpp $(LIB)/libfaust.a
	$(CXX) -DJACK $(COMPILEOPT) dynamic-jack-gtk.cpp  $(LIBS) -I $(INC) $(LLVM) `pkg-config --cflags --libs sndfile samplerate jack gtk+-2.0` $(STRIP) $(COMMLIBS) -o dynamic-jack-gtk


install:
	cp metaFx.sh $(prefix)/bin
	chmod 755 $(prefix)/bin/metaFx.sh
	cp metaFx  $(prefix)/bin
	chmod 755 $(prefix)/bin/metaFx
	cp dynamic-jack-gtk  $(prefix)/bin
	chmod 755 $(prefix)/bin/dynamic-jack-gtk
	mkdir $(prefix)/share/metaFx
	cp -Rfa Theme $(prefix)/share/metaFx/
	cp -Rfa Lang $(prefix)/share/metaFx
	cp -Rfa faust $(prefix)/share/metaSurface
	cp -Rfa sound $(prefix)/share/metaSurface

clean:
	rm -f metaFx
	
desinstall :
	rm -f $(prefix)/bin/metaFx
	rm -f $(prefix)/bin/metaFx.sh
	rm -r $(prefix)/share/metaFx
	
	
