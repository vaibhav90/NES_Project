CONTIKI =/home/vaibhav/test/contiki/
DEFINES=PROJECT_CONF_H=\"project-conf.h\"

# Define the target platform
ifndef TARGET
TARGET=sky
endif

# Give a name to your project 
CONTIKI =/home/vaibhav/test/contiki/
CONTIKI_PROJECT=receiver

# Compile project typing "make"
all: $(CONTIKI_PROJECT)

# Upload project typing "make upload"
upload: $(CONTIKI_PROJECT).upload

%.class: %.java
	javac $(basename $<).java
	
viewbase: ViewSink.class
	make login | java ViewSink
	
# Including the makefile
include $(CONTIKI)/Makefile.include
