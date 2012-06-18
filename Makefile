FLEX= /usr/bin/flex
SUDO= /usr/bin/sudo

all:
	@echo "Usage:"
	@echo "      make [home|pcalc|root]"

home:
	@echo "Creating Symbolic Links"
	@echo "    -> bashrc"
	@ln -sf    $(PWD)/bashrc $(HOME)/.bashrc
	@echo "    -> vimrc"
	@ln -sf    $(PWD)/vimrc $(HOME)/.vimrc
	@echo "    -> xinitrc"
	@ln -sf    $(PWD)/xinitrc $(HOME)/.xinitrc
	@echo "    -> fluxbox"
	@ln -sf    $(PWD)/fluxbox $(HOME)/.fluxbox
	@echo
	@echo "Creating Directories"
	@echo "    -> Local binary directory"
	@mkdir -p  $(HOME)/bin
	@echo "    -> Local bash include directory"
	@mkdir -p  $(HOME)/.profile.d
	@echo "    -> Local source directory"
	@mkdir -p  $(HOME)/src
	@echo "    -> Local downloads directory"
	@mkdir -p  $(HOME)/dwnlds

pcalc:
	@[ -x "$(FLEX)" ] || (echo "Flex not installed"; exit 1)
	make -C src/pcalc-2 all local-install distclean

root:
	@$(SUDO) -k
	$(SUDO) -k install -m 0600 $(PWD)/bashrc /root/.bashrc
	$(SUDO) -k install -m 0600 $(PWD)/vimrc /root/.vimrc
	$(SUDO) -k mkdir -p /root/bin /root/.profile.d
