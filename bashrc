##
# Exit if not interactive.
[[ $- != *i* ]] && return

##
# Safely add path to PATH environment variable
add_path() {
  [[ -d "${1:-/dev/null}" ]] && export PATH=${PATH:+$PATH:}$1;
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
# Toggles output profiles 
toggle_ao() {
    local current_profile=$(pacmd list-cards | grep 'active profile' |
        tr '[<>]' '~' | cut -f2 -d'~')

    if [ "${current_profile:-}" = "output:analog-surround-40" ]; then
        echo "Switching audio to HDMI."
        pacmd set-card-profile 0 output:hdmi-stereo >& /dev/null
    else
        echo "Switching audio to internal speakers."
        pacmd set-card-profile 0 output:analog-surround-40 >& /dev/null
    fi
}

##
# Ignore space and duplicates. Store only a single instance of a given command.
export HISTCONTROL=ignoreboth:erasedups

##
# I prefer vim.
export VISUAL=/usr/bin/vim
export EDITOR=/usr/bin/vim

##
# customize prompt
if [[ terminal_supports_color ]]; then
  workdir="\[\033[01;34m\]\w\[\033[00m\]"
  hostnm="[\[\033[01;32m\]\h\[\033[00m\]]"
  prompt="\[\033[01;34m\]$\[\033[00m\] "
  prompt="$workdir$hostnm$prompt"
  export PS1=$prompt
  unset workdir hostnm prompt
else
  export PS1="\w [\h]$ "
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
# Add ~/bin to executable path
add_path "$HOME/bin"

##
# Source RVM if necessery.
require "$HOME/.rvm/scripts/rvm"
