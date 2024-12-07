SRCDIR ?= /opt/fpp/src
include /opt/fpp/src/makefiles/common/setup.mk
include /opt/fpp/src/makefiles/platform/*.mk

all: libfpp-media-extended-commands.$(SHLIB_EXT)
debug: all

OBJECTS_fpp_media_extended_commands_so += src/FPPMediaExtendedCommands.o
LIBS_fpp_media_extended_commands_so += -L${SRCDIR} -lfpp -ljsoncpp -lhttpserver
CXXFLAGS_src/FPPMediaExtendedCommands.o += -I${SRCDIR}


%.o: %.cpp Makefile
	$(CCACHE) $(CC) $(CFLAGS) $(CXXFLAGS) $(CXXFLAGS_$@) -c $< -o $@

libfpp-media-extended-commands.$(SHLIB_EXT): $(OBJECTS_fpp_media_extended_commands_so) ${SRCDIR}/libfpp.$(SHLIB_EXT)
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_media_extended_commands_so) $(LIBS_fpp_media_extended_commands_so) $(LDFLAGS) -o $@

clean:
	rm -f libfpp-media-extended-commands.$(SHLIB_EXT) $(OBJECTS_fpp_media_extended_commands_so)
