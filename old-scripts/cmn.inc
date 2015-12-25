# This file is intended to be sourced by a shell script

#VERBOSE=true

DEBUGGING_THIS_FILE=no
#DEBUGGING_THIS_FILE=yes

EXE=$(basename $0)


DBG_MSG()
{
	if [ "x$DEBUGGING_THIS_FILE" == "xyes" ]; then echo "$*"; fi
}

DERR_MSG()
{
	DBG_MSG "$*" > /dev/stderr
}

ERR_MSG()
{
	echo "$*" > /dev/stderr
}

OUT_MSG()
{
	echo "$*"
}

alias eif='ExitIfFail'
alias rqs='RequireSet'
ExitIfFail()
{
	result=$?
	if [ $result != 0 ]; then
		ERR_MSG "Unexpected failure, exiting (code $result)."
		exit $result
	fi
}

RequireSet()
{
	DBG_MSG "Checking that $1 is defined ($1=\"$2\")"
	if [ "x$2" == "x" ]; then
		if [ "x$3" != "xnoexit" ]; then DERR_MSG "ERROR: $1 is not set" && exit 1; fi
	else
		DBG_MSG "$1=\"$2\""
	fi
}

get_tree_root()
{
	#echo "set_tree_root: in $PWD"
	if [ $PWD == / ]; then
		DERR_MSG "Reached root (/), giving up looking for TREE_ROOT"
		return 1
	else
		if [ -e $PWD/COMMON/BUILD/bin/cmn.inc ]; then
			# found it
			DERR_MSG "Found it at $PWD"
			echo $PWD
			return 0
		else
			DERR_MSG "Not yet reached /, trying $PWD/.."
			(cd ..; get_tree_root)
			return $?
		fi
	fi
}

git_is_valid_branch()
{
	if [ $# != 1 ]; then ERR_MSG "git_is_valid_branch: Invalid number of arguments"; return 1; fi
	local branchname=$1
	local b=$(git branch | grep $branchname | sed "s/^. //")
	if [ "x$b" != "x$branchname" ];	then
		DERR_MSG "Invalid branch"
		return 1
	else
		echo "$b"
		return 0
	fi
}

git_is_invalid_branch()
{
	if [ $# != 1 ]; then ERR_MSG "git_is_invalid_branch: Invalid number of arguments"; return 1; fi
	git_is_valid_branch "$branchname" > /dev/null 2>&1 && return 1

	return 0
}

get_repo()
{
	rqs CURL "$CURL"
	if [ "x$REPO" == "x" ]; then
		REPO=~/bin/repo
		if [ "x$REPO" != "x" ]; then
			ERR_MSG "REPO is not found, downloading it"
			$CURL https://storage.googleapis.com/git-repo-downloads/repo > $REPO
			chmod u+x $REPO
		fi
	fi
}

detect_cygwin()
{
if [ -d /cygdrive ]; then
	CYGWIN=yes
	DBG_MSG "Running under Cygwin"
	CYGWIN_SETUP=$(which cygwin-setup 2> /dev/null)
	if [ "x$CYGWIN_SETUP" == "x" ]; then
		echo "WARNING: Please place a link in your path called \"cygwin-setup\" to the cygwin setup.exe executable"
		echo "WARNING: Without it, this script will be unable to automatically install required applications"
		exit 1
	fi
else
	CYGWIN=no
	DBG_MSG "Not running under Cygwin"
	#TODO: use apt-get or whatever else is appropriate instead
fi
}

find_preqs()
{
	detect_cygwin

	if [ -z "$CURL" ]; then CURL=$(which curl 2> /dev/null); fi
	if [ -z "$PYTHON" ]; then PYTHON=$(which python 2> /dev/null); fi
	if [ -z "$REPO" ]; then REPO=$(which repo 2> /dev/null); fi
	if [ -z "$GIT" ]; then GIT=$(which git 2> /dev/null); fi
	if [ -z "$TREE_ROOT" ]; then TREE_ROOT=$(get_tree_root); fi
	
	get_repo

	rqs REPO "$REPO" noexit
	rqs CURL "$CURL" noexit
	rqs PYTHON "$PYTHON" noexit
	rqs GIT "$GIT"

	rqs TREE_ROOT "$TREE_ROOT" noexit
}

find_preqs



# EOF (cmn.inc)
