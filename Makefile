all:
	ln -sf $(PWD)/bashrc $(HOME)/.bashrc
	ln -sf $(PWD)/vimrc $(HOME)/.vimrc
	mkdir -p $(HOME)/bin $(HOME)/.profile.d/
