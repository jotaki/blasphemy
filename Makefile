FLEX= /usr/bin/flex
SUDO= /usr/bin/sudo

all:
	@echo "Usage:"
	@echo "      make [option]"
	@echo
	@echo "Where [option] is:"
	@echo "    home        -  build home root"
	@echo "    pcalc       -  build pcalc"
	@echo "    xidle       -  build xidle"
	@echo "    root        -  install for root user"
	@echo "    rotty       -  build rot13/rot47"
	@echo "    xor         -  build simple xor program"
	@echo "    checklist   -  build simple checklist program"
	@echo "    prime       -  build prime checker"
	@echo "    common-bin  -  symlink common binaries"
	@echo "    fresh       -  All of the above. (Except root)"
	@echo "    iplookup    -  build iplookup"
	@echo "    rgb-display -  Display 6-digit hex color code"

fresh: home pcalc xidle common-bin rotty xor prime checklist iplookup \
	rgb-display
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
	@rm -rf    $(HOME)/.fluxbox
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

prime:
	@make -C src/prime all local-install clean

xidle:
	@make -C src/xidle all local-install clean

rotty:
	@make -C src/rotty all local-install clean

xor:
	@make -C src/xor all local-install clean

iplookup:
	@make -C src/iplookup all local-install clean

rgb-display:
	@make -C src/rgb-display all local-install clean

checklist:
	@make -C src/checklist all
	install -m 0755 src/checklist/checklist $(HOME)/bin/checklist
	@make -C src/checklist clean

common-bin:
	@echo "Creating symlinks to common scripts"
	@echo "    ->  random background script (setrbkground)"
	@ln -sf    $(PWD)/bin/setrbkground $(HOME)/bin/setrbkground
	@echo "    ->  change background script (setbkground)"
	@ln -sf    $(PWD)/bin/setbkground $(HOME)/bin/setbkground
	@echo "    ->  magic 8-ball script."
	@ln -sf    $(PWD)/bin/8ball $(HOME)/bin/8ball
	@echo "    ->  google-chrome proxy wrapper"
	@ln -sf    $(PWD)/bin/chrome-proxy $(HOME)/bin/chrome-proxy
	@echo "    ->  markdown viewer"
	@ln -sf    $(PWD)/bin/view-markdown $(HOME)/bin/viewmd
	@echo "    ->  build local rpms wrapper"
	@ln -sf    $(PWD)/bin/make_rpm $(HOME)/bin/make_rpm

root:
	@$(SUDO) -k
	$(SUDO) -k install -m 0600 $(PWD)/bashrc /root/.bashrc
	$(SUDO) -k install -m 0600 $(PWD)/vimrc /root/.vimrc
	$(SUDO) -k mkdir -p /root/bin /root/.profile.d
