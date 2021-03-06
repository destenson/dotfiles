#!/usr/bin/env bash


# Get current dir (so this script can run from anywhere)
export DOTFILES_DIR EXTRA_DIR
DOTFILES_DIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"
EXTRA_DIR="$HOME/.extra"

# Update dotfiles itself first
[ -d "$DOTFILES_DIR/.git" ] && git --work-tree="$DOTFILES_DIR" --git-dir="$DOTFILES_DIR/.git" pull origin master

. $DOTFILES_DIR/bash/_profile

# Bunch of symlinks
grep "bash/_profile" ~/.bash_profile &> /dev/null || { echo; echo ". $DOTFILES_DIR/bash/_profile"; } >> ~/.bash_profile


ln -sfv "$DOTFILES_DIR/git/global.gitconfig" ~/.gitconfig
#TODO: copy contents of files in gitignore project intelligently
cp "$DOTFILES_DIR/git/global.gitignore" ~/.gitignore
if [ -d ~/gitignore/Global ]; then
  for ig in ~/gitignore/Global/*.gitignore ; do
	cat $ig >> ~/.gitignore
  done
  unset ig
fi

echo "TODO: install repo"
# Install repo (always get the latest)
#if is-windows ; then
  # Google's version of repo doesn't work with MINGW/MSYS (Git Bash on Windows)
  #curl https://raw.githubusercontent.com/esrlabs/git-repo/master/repo > ~/bin/repo && chmod a+x ~/bin/repo &&\
  #curl https://raw.githubusercontent.com/esrlabs/git-repo/master/repo.cmd > ~/bin/repo.cmd || exit 1
#else
  #curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo && chmod a+x ~/bin/repo || exit 1
#fi

echo "TODO: install python"
# Install python
#if [ ! is-executable python ]; then
  #echo "A Python interpreter cannot be found (repo requires it)"
  #if [ is-git-bash && ! -x /c/Python*/python.exe ]; then
    #echo "Installing Python for Windows..."
    #if is-win32 ; then
    #  curl https://www.python.org/ftp/python/2.7.8/python-2.7.8.msi > /tmp/python-2.7.8.msi &&\
    #  chmod +x /tmp/python-2.7.8.msi && /tmp/python-2.7.8.msi && rm -f /tmp/python-2.7.8.msi ||\
    #  { echo "failed to install Python for Windows"; exit 1; }
    #elif is-win64 ; then
    #  curl https://www.python.org/ftp/python/2.7.8/python-2.7.8.amd64.msi > /tmp/python-2.7.8.amd64.msi &&\
    #  chmod +x /tmp/python-2.7.8.amd64.msi && /tmp/python-2.7.8.amd64.msi && rm -f /tmp/python-2.7.8.amd64.msi ||\
    #  { echo "failed to install Python for Windows"; exit 1; }
    #fi
  #fi
#fi

# Install extra stuff
[ -d "$EXTRA_DIR" ] && if [[ -f "$EXTRA_DIR/install.sh" && -r "$EXTRA_DIR/install.sh" ]]; then
    . "$EXTRA_DIR/install.sh"
fi

