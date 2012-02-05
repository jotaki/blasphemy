FLEX= /usr/bin/flex
SUDO= /usr/bin/sudo

all:
	@echo "Usage:"
	@echo "      make [home|pcalc|root]"

home:
	ln -sf $(PWD)/bashrc $(HOME)/.bashrc
	ln -sf $(PWD)/vimrc $(HOME)/.vimrc
	mkdir -p $(HOME)/bin $(HOME)/.profile.d/

pcalc:
	@[ -x "$(FLEX)" ] || (echo "Flex not installed"; exit 1)
	make -C src/pcalc-2 all local-install distclean

root:
	@$(SUDO) -k
	$(SUDO) -k install -m 0600 $(PWD)/bashrc /root/.bashrc
	$(SUDO) -k install -m 0600 $(PWD)/vimrc /root/.vimrc
	$(SUDO) -k mkdir -p /root/bin /root/.profile.d
