##
# Exit if not interactive.
[[ $- != *i* ]] && return

##
# Safely add path to PATH environment variable
add_path() {
    if [[ -d "${1:-/dev/null}" ]]; then
        if [[ "${2:-last}" = "first" ]]; then
            PATH="${1}${PATH:+:$PATH}"
        else
            PATH="${PATH:+$PATH:}${1}"
        fi
        export PATH
        return 0
    fi

    return 1
}

##
# Return true if user is root
is_root() {
    [[ 0 -eq $(id -u) ]] && return 0
    return 1
}

##
# Safely include files.
require() {
    [[ -f "${1:-/dev/null}" ]] && source "$1";
}

##
# destroy history file.
nuke_history() {
    cat /dev/null > $HISTFILE
    echo "$(date +%s).$$ ($(id -nu))" >> $HISTFILE.nuked
    export HISTFILE=/dev/null
}

##
# returns true if terminal supports color.
terminal_supports_color() {
    [[ -x /usr/bin/tput ]] && tput setaf 1 >&/dev/null;
}

##
# grep with less (color support) use with care.
lgrep() {
    local clr=always

    terminal_supports_color || clr=auto
    \grep --color=$clr $@ | \less -R
}

##
# find editor
find_editor() {
    local editor=

    while [[ 0 < ${#@} ]]; do
        editor=$(which $1)
        [[ -z "$editor" ]] || break
        shift;
    done

    [[ -z "$editor" ]] &&
        echo -n echo No available editor: ||
        echo -n $editor
}


##
# Ignore space and duplicates. Store only a single instance of a given command.
export HISTCONTROL=ignoreboth:erasedups

##
# Preferred editor.
export EDITOR=$(find_editor vim vi ed)
export VISUAL="$EDITOR"

##
# customize prompt
if terminal_supports_color; then
    dclr="34m"; hclr="32m"; pclr="34m"; ps1="$"
    if is_root; then
        dclr="36m"
        hclr="31;9m"
        pclr="31m"
        ps1='#'
    fi

    workdir="\[\033[01;$dclr\]\w\[\033[00m\]"
    hostnm="[\[\033[01;$hclr\]\h\[\033[00m\]]"
    prompt="\[\033[01;$pclr\]$ps1\[\033[00m\] "
    export PS1="$workdir$hostnm$prompt"
    unset workdir hostnm prompt dclr hclr pclr ps1
else
    if is_root; then
        export PS1="\w [\h]# "
    else
        export PS1="\w [\h]$ "
    fi
fi

##
# Append to history, don't overwrite.
shopt -s histappend

##
# Update LINES and COLUMNS environment variables after each command.
shopt -s checkwinsize

##
# Enable color (if applicable)
alias grep='grep --color=auto'
alias ls='ls --color=auto'

##
# Useful on new systems.
alias installrvm='bash < <(curl -s https://rvm.beginrescueend.com/install/rvm)'

##
# Nuke history
alias killhistory=nuke_history

##
# info to behave like vi
alias info="info --vi-keys"

##
# control characters for less
alias cless='less -r'

##
# allow colors with less
alias less='less -R'

##
# Add ~/bin to executable path
add_path "$HOME/bin" first

##
# Source RVM if necessery.
require "$HOME/.rvm/scripts/rvm"

##
# source some additional bashrc files.
if [ -d "$HOME/.profile.d/" ]; then
    for srcfile in $HOME/.profile.d/*; do
        require "$srcfile"
    done
fi
