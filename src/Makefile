COMP = /usr/bin/g++
OBJECTS = BufferManager.o GramDictOrder.o HashFunction.o Node.o SBPTree.o StrComp.o \
 StrDictOrder.o main.o Page.o InvertedList.o Queue.o GramLocOrder.o NormGramDictOrder.o
LFLAG = -o
CFLAG = -c -O3 -Wno-deprecated

Stringindex : $(OBJECTS)
	$(COMP) $(LFLAG) ../bin/bedtree $(OBJECTS) $(OPTION)

GramLocOrder.o : GramLocOrder.cpp
	$(COMP) $(CFLAG) GramLocOrder.cpp

NormGramDictOrder.o : NormGramDictOrder.cpp
	$(COMP) $(CFLAG) NormGramDictOrder.cpp

Queue.o : Queue.cpp
	$(COMP) $(CFLAG) Queue.cpp

InvertedList.o : InvertedList.cpp
	$(COMP) $(CFLAG) InvertedList.cpp

Page.o : Page.cpp
	$(COMP) $(CFLAG) Page.cpp

BufferManager.o : BufferManager.cpp
	$(COMP) $(CFLAG) BufferManager.cpp

GramDictOrder.o : GramDictOrder.cpp
	$(COMP) $(CFLAG) GramDictOrder.cpp

HashFunction.o : HashFunction.cpp
	$(COMP) $(CFLAG) HashFunction.cpp

Node.o : Node.cpp
	$(COMP) $(CFLAG) Node.cpp

SBPTree.o : SBPTree.cpp
	$(COMP) $(CFLAG) SBPTree.cpp

StrComp.o : StrComp.cpp
	$(COMP) $(CFLAG) StrComp.cpp

StrDictOrder.o : StrDictOrder.cpp
	$(COMP) $(CFLAG) StrDictOrder.cpp

main.o : main.cpp
	$(COMP) $(CFLAG) main.cpp

.PHONY : clean
clean :
	-rm ../bin/bedtree $(OBJECTS)
