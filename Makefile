obj-m := mostima.o

KDIR ?= /lib/modules/$(shell uname -r)/build
GEN_COMPDB := $(KDIR)/scripts/clang-tools/gen_compile_commands.py

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules
	python3 $(GEN_COMPDB) -d $(CURDIR) -o $(CURDIR)/compile_commands.json $(CURDIR)

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean

