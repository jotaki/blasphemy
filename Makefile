FLEX= /usr/bin/flex
SUDO= /usr/bin/sudo

all:
	@echo "Usage:"
	@echo "      make [home|pcalc|xidle|root|common-bin|fresh]"

fresh: home pcalc xidle common-bin
	@echo
	@echo
	@echo "Fresh install complete :-)"
	@echo

home:
	@echo "Creating Symbolic Links"
	@echo "    -> bashrc"
	@ln -sf    $(PWD)/bashrc $(HOME)/.bashrc
	@echo "    -> vimrc"
	@ln -sf    $(PWD)/vimrc $(HOME)/.vimrc
	@echo "    -> xinitrc"
	@ln -sf    $(PWD)/xinitrc $(HOME)/.xinitrc
	@echo "    -> xsession"
	@ln -sf    $(PWD)/xinitrc $(HOME)/.xsession
	@echo "    -> fluxbox"
	@rm -f     $(HOME)/.fluxbox
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
	@echo "    -> Local media directories"
	@mkdir -p  $(HOME)/media/{local,remote}
	@echo "    -> Local media subdirectories"
	@mkdir -p  $(HOME)/media/local/{documents,vms,wallpaper,music,video}

pcalc:
	@[ -x "$(FLEX)" ] || (echo "Flex not installed"; exit 1)
	@make -C src/pcalc-2 all local-install distclean

xidle:
	@make -C src/xidle all local-install clean

common-bin:
	@echo "Creating symlinks to common scripts"
	@echo "    ->  random background script (setrbkground)"
	@ln -sf    $(PWD)/bin/setrbkground $(HOME)/bin/setrbkground
	@echo "    ->  change background script (setbkground)"
	@ln -sf    $(PWD)/bin/setbkground $(HOME)/bin/setbkground

root:
	@$(SUDO) -k
	$(SUDO) -k install -m 0600 $(PWD)/bashrc /root/.bashrc
	$(SUDO) -k install -m 0600 $(PWD)/vimrc /root/.vimrc
	$(SUDO) -k mkdir -p /root/bin /root/.profile.d
