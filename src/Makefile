#####################################################################

# gts-tools Makefile - src

# Copyright (C) 2006  Toby Breckon
# License : GPL

#####################################################################

INCLUDE=-Icommon/ -Iann_1.1/include/
GTS_CFLAGS=`gts-config --cflags`
GTS_LIBS=`gts-config --libs`

TOOLS=gtsinfo gts2vtk gts2xyz gtstesselate gtscutter gtsorientate \
      gtsnormals gtstricutter gtsnormalise gtssubtract

DEST=../tools/

#####################################################################

ANN_DIR=ann_1.1
ANN_LIB_PATH=-L$(ANN_DIR)/lib/
ANN_LIB=-lANN
ANN_MAKE_TARGET=linux-g++

ANN_TREE_OBJ=common/ann_tree.o

#####################################################################

OBJS=common/gts_common.o \
	 common/gts_input.o \
	 common/compressed.o \
	 common/output.o

HEADERS=common/general.h $(OBJS:.o=.h)

#####################################################################

CC=g++
CFLAGS=-Wall -O3

#####################################################################

all: 
	make $(TOOLS); strip $(TOOLS); cp $(TOOLS) $(DEST)

#####################################################################

gtssubtract: gtssubtract.cc $(OBJS) $(ANN_TREE_OBJ) $(HEADERS) $(ANN_LIB_PATH)/libANN.a
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtssubtract.cc $(OBJS) $(ANN_TREE_OBJ) -o gtssubtract \
	$(ANN_LIB_PATH) $(ANN_LIB) 

gtsnormalise: gtsnormalise.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtsnormalise.cc $(OBJS) -o gtsnormalise

gtstricutter: gtstricutter.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtstricutter.cc $(OBJS) -o gtstricutter

gtsnormals: gtsnormals.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtsnormals.cc $(OBJS) -o gtsnormals

gtsorientate: gtsorientate.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtsorientate.cc $(OBJS) -o gtsorientate

gtscutter: gtscutter.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtscutter.cc $(OBJS) -o gtscutter

gtstesselate: gtstesselate.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtstesselate.cc $(OBJS) -o gtstesselate

gts2xyz: gts2xyz.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gts2xyz.cc $(OBJS) -o gts2xyz

gts2vtk: gts2vtk.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gts2vtk.cc $(OBJS) -o gts2vtk
	
gtsinfo: gtsinfo.cc $(OBJS) $(HEADERS)
	$(CC) $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) $(GTS_LIBS) \
	gtsinfo.cc $(OBJS) -o gtsinfo

$(ANN_LIB_PATH)/libANN.a:
	cd $(ANN_DIR); make $(ANN_MAKE_TARGET)

#####################################################################

$(OBJS) $(XYZ_OBJ):
	cd $(@D); make $(@F)
	@echo

.cc.o:
	$(CC) -c $(CC_OPTS) $(CFLAGS) $(INCLUDE) $(GTS_CFLAGS) -o $@ $<

#####################################################################

clean:
	rm -f *.o $(TOOLS) common/*.o; cd $(ANN_DIR); \
	make clean; make realclean

#####################################################################
