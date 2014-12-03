CONTIKI = /home/vaibhav/test/contiki/
include $(CONTIKI)/Makefile.include
CONTIKI_PROJECT=interferer_example_micro interferer_example_1 interferer_example_2 interferer_example_3 interferer_example_4 interferer_example_8
APPS+=powertrace
DEFINES=PROJECT_CONF_H=\"project-conf.h\"
all:$(CONTIKI_PROJECT)
#UIP_CONF_IPV6=1
CONTIKI_TARGET_SOURCEFILES += settings_cc2420_rssi.c
CONTIKI_TARGET_SOURCEFILES += settings_cc2420_interferer.c
CONTIKI_TARGET_SOURCEFILES += powertrace.c
CUSTOM_RULE_LINK=1
%.$(TARGET): %.co $(PROJECT_OBJECTFILES) $(PROJECT_LIBRARIES) contiki-$(TARGET).a
	$(LD) $(LDFLAGS) $(TARGET_STARTFILES) ${filter-out %.a,$^} ${filter %.a,$^} $(TARGET_LIBFILES) -o $@ -lm



