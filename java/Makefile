.SUFFIXES: .java .class

TARGETS=sierpinski.jar

MAIN=SierpinskiFE.java

LIBRARIES = \
	Hybrid.java \
	Sierpinski.java \

CLASSES=$(MAIN:.java=.class) $(LIBRARIES:.java=.class)


all: $(TARGETS)

SierpinskiFE.class: SierpinskiFE.java
	javac $^

sierpinski.jar: $(MAIN) $(CLASSES)
	jar cf $@ *.class

clean:
	$(RM) *.class $(TARGETS)

