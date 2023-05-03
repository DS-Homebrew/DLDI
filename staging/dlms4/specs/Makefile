ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

-include $(DEVKITARM)/ds_rules

all:
	@echo -n Compiling dldi_crt0...
	@$(CC)  -x assembler-with-cpp -marm -mthumb-interwork -c dldi_crt0.s -o dldi_crt0.o
	@echo done.
	
	@echo -n Rewriting specs file...
	@echo '*startfile:' > dldi.specs
	@echo -n $(shell pwd) >> dldi.specs
	@echo '/../specs/dldi_crt0%O%s crti%O%s crtbegin%O%s' >> dldi.specs
	@echo done.
	
clean:
	@echo Clean...
	rm -f dldi.specs dldi_crt0.o
	@echo done.
	