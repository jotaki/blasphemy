#! /bin/sh

mkdir -p $HOME/.profile.d/

for script in $PWD/*; do
    test "$(basename $script)" = "install.sh" && continue
    ln -svf $script $HOME/.profile.d/
done

for script in $HOME/.profile.d/*; do
    if [ ! -e "$script" ]; then
        rm -rvf -- "$script"
    fi
done
