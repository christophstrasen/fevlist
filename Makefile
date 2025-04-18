NAME = fevlist

ifndef CPU
    $(error Specify CPU=[x86|x86_64|arm|armhf])
endif

ifndef FMOD_STUDIO_PATH
    $(error Specify FMOD_STUDIO_PATH)
endif

SOURCE_FILES = \
    fevlist.cpp \

INCLUDE_DIRS = \
    -I${FMOD_STUDIO_PATH}/api/core/inc \
    -I${FMOD_STUDIO_PATH}/api/studio/inc

ifdef DEBUG
    SUFFIX = L
    CXXFLAGS += -DDEBUG -g
else
    SUFFIX =
endif

CORE_LIB = ${FMOD_STUDIO_PATH}/api/core/lib/${CPU}/libfmod${SUFFIX}.so
STUDIO_LIB = ${FMOD_STUDIO_PATH}/api/studio/lib/${CPU}/libfmodstudio${SUFFIX}.so

RPATH_FLAGS = \
    -Wl,-rpath=\$$ORIGIN/$(dir ${CORE_LIB}),-rpath=\$$ORIGIN/$(dir ${STUDIO_LIB})

all:
	${CXX} ${CXXFLAGS} -pthread -o ${NAME} ${SOURCE_FILES} ${RPATH_FLAGS} ${CORE_LIB} ${STUDIO_LIB} ${INCLUDE_DIRS}

