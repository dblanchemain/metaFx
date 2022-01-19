LIB ?= /usr/local/lib
INC = ../../architecture

DESTDIR ?=
PREFIX ?= /usr/local

prefix := $(DESTDIR)$(PREFIX)


metaFx: metaFx.cpp
	$(CXX)  -std=c++17 -O3 mixer.cpp metaFx.cpp selectFile.cpp info.cpp fx.cpp module.cpp connector.cpp graph.cpp contrainte.cpp viewContraintes.cpp pave.cpp externe.cpp viewGreffon.cpp newGreffon.cpp apropos.cpp -I $(INC) -lstdc++fs -lsfml-graphics -lsfml-window -lsfml-system -llo -lGL -lGLU  -lGL  -DGL_GLEXT_PROTOTYPES -lboost_system -lboost_filesystem  -ljack -lsndfile -lfaust  -lpthread `pkg-config --cflags --libs  jack sndfile gtk+-2.0  glib-2.0  ` -lOSCFaust -o metaFx


install:
	cp metaFx.sh $(prefix)/bin
	chmod 755 $(prefix)/bin/metaFx.sh
	cp metaFx  $(prefix)/bin
	chmod 755 $(prefix)/bin/metaFx
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
	
	
