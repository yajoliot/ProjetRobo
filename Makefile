include variables

.PHONY: all install

all:
	cd $(CURRENT_DIR) && $(MAKE)

install:
	cd $(CURRENT_DIR) && $(MAKE) install
