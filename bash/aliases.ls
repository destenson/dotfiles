# Autoincluded by .bash_profile (TODO: review this file)

# ls

LS_COLORS=`is-supported "ls --color" --color -G`
LS_TIMESTYLEISO=`is-supported "ls --time-style=long-iso" --time-style=long-iso`
LS_GROUPDIRSFIRST=`is-supported "ls --group-directories-first" --group-directories-first`

alias l="ls -lahA $LS_COLORS $LS_TIMESTYLEISO $LS_GROUPDIRSFIRST"
alias ll="ls -lA $LS_COLORS"
alias lt="ls -lhAtr $LS_COLORS $LS_TIMESTYLEISO $LS_GROUPDIRSFIRST"
alias ld="ls -ld $LS_COLORS */"

unset LS_COLORS LS_TIMESTYLEISO LS_GROUPDIRSFIRST

# Set LSCOLORS
[ -f "$DOTFILES_DIR"/system/dir_colors ] && eval "$(dircolors "$DOTFILES_DIR"/system/dir_colors)"

