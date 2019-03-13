include variables

.PHONY: all

all:
ifndef DEBUG_CMD
	cd $(CURRENT_DIR) && $(MAKE)
else
	cd $(CURRENT_DIR) && $(MAKE) $(DEBUG_CMD)
endif
