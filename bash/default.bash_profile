#!/bin/env bash


_no_dotfiles()
{
	{
		echo "DOTFILES_DIR(=$DOTFILES_DIR) not found"
		echo "You will be missing a lot of custom functionality until that is fixed."
		echo "To get dotfiles, use:"
		echo "	get_dotfiles"
	} &> /dev/stderr
}

_set_dotfiles_dir()
{
	[ -d $HOME/dotfiles ] && export DOTFILES_DIR=${DOTFILES_DIR:-$HOME/dotfiles}
	[ -z "$DEBUGGING_DOTFILES" ] || echo "DOTFILES_DIR=$DOTFILES_DIR"
	[ -d $DOTFILES_DIR ] || {\
		function get_dotfiles()
		{
			[ -d "$DOTFILES_DIR" ] && {
				pushd "$DOTFILES_DIR" && {
					git fetch --all
					popd
				}
			} || {
				pushd "$HOME" && {
					git clone --progress -b windows https://www.github.com/destenson/dotfiles
					popd
				}
			}
		}	
	}
}


_load_bash_profile()
{
	[ -d $DOTFILES_DIR ] || {
		_no_dotfiles
		return 1
	}

	local BASH_PROFILE="$DOTFILES_DIR/bash/_profile"

	[[ -e "$BASH_PROFILE" && -f "$BASH_PROFILE" && "$BASH_PROFILE" ]] &&\
	. $BASH_PROFILE ||\
	echo "$BASH_PROFILE not found" > /dev/stderr
}


_set_dotfiles_dir

_load_bash_profile



# EOF
