include variables

.PHONY: all install clean

all:
	cd $(CURRENT_DIR) && $(MAKE)

install:
	cd $(CURRENT_DIR) && $(MAKE) install

clean:
	cd $(CURRENT_DIR) && $(MAKE) clean	
