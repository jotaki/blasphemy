all:
	@echo "Usage:"
	@echo "      make [home|pcalc]"

home:
	ln -sf $(PWD)/bashrc $(HOME)/.bashrc
	ln -sf $(PWD)/vimrc $(HOME)/.vimrc
	mkdir -p $(HOME)/bin $(HOME)/.profile.d/

pcalc:
	make -C src/pcalc-2 all local-install distclean
