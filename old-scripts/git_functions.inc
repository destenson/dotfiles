# This file is intended to be sourced by a shell script

#VERBOSE=true

# include useful variables and functions made available by git for shell scripts
# git_broken_path_fix: ??
# die: { die_with_status 1 "$@" }

. "$(git --exec-path)/git-sh-setup"


LOCAL_MANIFEST=~/git-repos/initial

ERR_MSG() { echo "$*" > /dev/stderr ;}
OUT_MSG() { echo "$*" > /dev/stdout ;}

ONLY_ECHO_COMMANDS=yes

if [ "x$ONLY_ECHO_COMMANDS" == "xyes" ]; then
	__=echo
else
	__=
fi

# tab auto-completion for functions requiring a valid branch as input
_git_branch_autocomplete_helper()
{
	COMPREPLY=()	# array variable storing possible completions
	local cur=${COMP_WORDS[COMP_CWORD]}
	COMPREPLY=( $(compgen -W "$(git_list_branches)" -- "$cur") )
	return 0
}

git_list_branches() { git branch | sed "s/^. //" ;}

complete -F _git_branch_autocomplete_helper git_is_valid_branch_name
git_is_valid_branch_name()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branchname=$1
	local b=$(git_list_branches | grep $branchname)
	if [ "x$b" != "x$branchname" ]; then return 1; fi # the branch existed in the output of git_list_branches
	return 0
}

complete -F _git_branch_autocomplete_helper git_is_invalid_branch_name
git_is_invalid_branch_name()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branchname=$1
	local b=$(list_branch | grep $branchname)
	if [ "x$b" != "x$branchname" ]; then return 0; fi
	return 1
}

complete -F _git_branch_autocomplete_helper git_archive
git_archive()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branchname=$1
	git_is_valid_branch_name $branchname || { ERR_MSG "Invalid branch"; exit 1; }

	$__ git tag archive/$branchname $branchname &&\
	$__ git branch -d $branchname
}

complete -F _git_branch_autocomplete_helper git_merge_and_push
git_merge_and_push()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branchname=$1
	git_is_valid_branch_name $branchname || { ERR_MSG "Invalid branch"; exit 1; }

	$__ git merge $branchname &&\
	$__ git push
}

git_save_all_and_commit()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local msg=$1
	
	$__ git add . &&\
	$__ git commit -m "$msg"
}

complete -F _git_branch_autocomplete_helper git_save_all_and_switch_to_branch
git_save_all_and_switch_to_branch()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branchname=$1
	git_is_valid_branch_name $branchname || { ERR_MSG "Invalid branch"; exit 1; }
	
	$__ git add . &&\
	$__ git commit -m "committing changes before switching to $branchname" &&\
	$__ git checkout $branchname
}



# the following functions come roughly in the order of a git tutorial at (https://try.github.io)

git_create_repository_here() { $__ git init ;}

# it is healthy to run git_status often, things can change without you noticing
git_status() { $__ git status ;}

# git_add prepares files for committing to the repository by staging them
#git_add() { git add $* ;} # not valid? wildcards can be used however
git_add_all() { $__ git add --all ;}
git_add() 
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi
	
	$__ git add $1
}
git_add_all_src()
{
	git_add '*.c'
	git_add '*.cc'
	git_add '*.cpp'
	git_add '*.h'
	git_add '*.hpp'
	git_add '*.hxx'
	git_add '*.sh'
	git_add '*.cs'
	git_add '*.js'
}

git_stage() { git_add $* ;}
git_stage_all() { git_add_all $* ;}
git_stage_all_src() { git_add_all_src $* ;}

git_commit_locally() { $__ git commit -m "$*" ;}

git_log() { $__ git log ;}

git_add_remote()
{
	# git_add_remote "(remote)origin" "URL"
	if [[ $# != 2 ]]; then die "Invalid number of arguments"; fi
	
	local remote=$1
	local url=$2
	$__ git remote add $remote $url
}
git_add_remote_origin()
{
	# git_add_remote_origin "URL"
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi
	
	local url=$1
	git_add_remote origin $url
}

# should this be called git_push_upstream() instead of git_push() since -u is specified?
git_push_upstream()
{
	# git push (from branch) -> (to remote)
	# git_push "(local branch)master" "(remote)origin"
	if [[ $# != 2 ]]; then die "Invalid number of arguments"; fi

	local branch=$1
	local remote=$2
	git_is_valid_branch_name $branch || { ERR_MSG "Invalid branch"; exit 1; }
	$__ git push -u $remote $branch
}

# used to inform the local git repo of the latest version of the remote
git_query_remote()
{
	# git pull (to branch) <- (from remote)
	if [[ $# != 2 ]]; then die "Invalid number of arguments"; fi

	local branch=$1
	local remote=$2
	git_is_valid_branch_name $branch || { ERR_MSG "Invalid branch"; exit 1; }
	$__ git pull $remote $branch
}

# show differences from last commit (explain?)
git_diff_head() { $__ git diff HEAD ;}

# show differences from staged files (explain?)
git_diff_staged() { $__ git diff --stages ;}

# remove files from the staging area
git_unstage() { $__ git reset $* ;}

git_checkout_() { $__ git checkout -- $* ;} # not sure how this works yet

complete -F _git_branch_autocomplete_helper git_checkout_branch
git_checkout_branch()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branch=$1
	git_is_valid_branch_name $branch || { ERR_MSG "Invalid branch"; exit 1; }
	$__ git checkout $branch
}


git_remove_and_unstage() { $__ git rm $* ;}

git_switch() { get_checkout_branch $* ;}

complete -F _git_branch_autocomplete_helper git_merge_with_branch
git_merge_with_branch()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branch=$1
	git_is_valid_branch_name $branch || { ERR_MSG "Invalid branch"; exit 1; }
	$__ git merge $branch
}

complete -F _git_branch_autocomplete_helper git_delete_branch
git_delete_branch()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branch=$1
	git_is_valid_branch_name $branch || { ERR_MSG "Invalid branch"; exit 1; }
	$__ git branch -d $branch
}


git_create_and_checkout_branch()
{
	if [[ $# != 1 ]]; then die "Invalid number of arguments"; fi

	local branch=$1
	git_is_invalid_branch_name $branch || { ERR_MSG "Branch already exists"; exit 1; }
	$__ git checkout -b $branch
}

git_create_and_checkout_branch_from_another()
{
	if [[ $# != 2 ]]; then die "Invalid number of arguments"; fi

	local new_branch=$1
	local old_branch=$2
	git_is_invalid_branch_name $new_branch || { ERR_MSG "Branch already exists"; exit 1; }
	git_is_valid_branch_name $old_branch || { ERR_MSG "Invalid branch"; exit 1; }
	$__ git checkout -b $new_branch $old_branch
}














#
#
#
#
# What follows is copied from git-wip. I should try to modify it to make it
# more perfect for our purposes.
#
#
#
#








# sets up useful GIT_*
. "$(git --exec-path)/git-sh-setup"

require_work_tree

TMP="$GIT_DIR/.git-wip.$$"
trap 'rm -f "$TMP-*"' 0

WIP_INDEX="$TMP-INDEX"

WIP_PREFIX=refs/wip/
WIP_COMMAND=
WIP_MESSAGE=WIP
EDITOR_MODE=false

dbg() {
	if test -n "$WIP_DEBUG"
	then
		printf '# %s\n' "$*"
	fi
}

# some errors are not worth reporting in --editor mode
report_soft_error () {
	$EDITOR_MODE && exit 0
	die "$@"
}

cleanup () {
	rm -f "$TMP-*"
}

git_get_working_branch () {
	ref=$(git symbolic-ref -q HEAD) || report_soft_error "git-wip requires a branch"

	branch=${ref#refs/heads/}
	if [ $branch = $ref ] ; then
		die "git-wip requires a local branch"
	fi

	echo $branch
}

get_wip_branch () {
	return 0
}

check_files () {
	local files=$@

	for f in $files
	do
		[ -f "$f" -o -d "$f" ] || die "$f: No such file or directory."
	done
}

build_new_tree () {
	local untracked=$1 ; shift
	local ignored=$1 ; shift
	local files=$@

	(
	set -e
	rm -f "$WIP_INDEX"
	cp -p "$GIT_DIR/index" "$WIP_INDEX"
	export GIT_INDEX_FILE="$WIP_INDEX"
	git read-tree $wip_parent
	if [ -n "$files" ]; then
		git add -f $files
	else
		git add --update .
	fi
	[ -n "$untracked" ] && git add .
	[ -n "$ignored" ] && git add -f -A .
	git write-tree
	rm -f "$WIP_INDEX"
	)
}

do_save () {
	local msg="$1" ; shift
	local add_untracked=
	local add_ignored=

	while test $# != 0; do
		case "$1" in
		-e|--editor)
			EDITOR_MODE=true
			;;
		-u|--untracked)
			add_untracked=t
			;;
		-i|--ignored)
			add_ignored=t
			;;
		--)
			shift
			break
			;;
		*)
			[ -f "$1" ] && break
			die "Unknown option '$1'."
			;;
		esac
		shift
	done
	local files=$@
	local "add_untracked=$add_untracked"
	local "add_ignored=$add_ignored"

	if test ${#files} -gt 0; then
		check_files $files
	fi

	dbg "msg=$msg"
	dbg "files=$files"

	local work_branch=$(git_get_working_branch)
	local wip_branch="$WIP_PREFIX$work_branch"

	dbg "work_branch=$work_branch"
	dbg "wip_branch=$wip_branch"

	# enable reflog
	local wip_branch_file="$GIT_DIR/logs/$wip_branch"
	dbg "wip_branch_file=$wip_branch_file"
	mkdir -p "$(dirname "$wip_branch_file")"
	: >>"$wip_branch_file"

	if ! work_last=$(git rev-parse --verify $work_branch); then
		report_soft_error "'$work_branch' branch has no commits."
	fi

	dbg "work_last=$work_last"

	if wip_last=$(git rev-parse --quiet --verify $wip_branch)
	then
		local base=$(git merge-base $wip_last $work_last) \
		|| die "'work_branch' and '$wip_branch' are unrelated."

		if [ $base = $work_last ] ; then
			wip_parent=$wip_last
		else
			wip_parent=$work_last
		fi
	else
		wip_parent=$work_last
	fi

	dbg "wip_parent=$wip_parent"

	new_tree=$( build_new_tree "$add_untracked" "$add_ignored" $files ) \
	|| die "Cannot save the current worktree state."

	dbg "new_tree=$new_tree"

	if git diff-tree --exit-code --quiet $new_tree $wip_parent ; then
		report_soft_error "no changes"
	fi

	dbg "... has changes"

	new_wip=$(printf '%s\n' "$msg" | git commit-tree $new_tree -p $wip_parent) \
	|| die "Cannot record working tree state"

	dbg "new_wip=$new_wip"

	msg1=$(printf '%s\n' "$msg" | sed -e 1q)
	git update-ref -m "git-wip: $msg1" $wip_branch $new_wip $wip_last

	dbg "SUCCESS"
}

do_info () {
	local branch=$1

	die "info not implemented"
}

do_log () {
	local work_branch=$1
	[ -z $branch ] && work_branch=$(git_get_working_branch)
	local wip_branch="$WIP_PREFIX$work_branch"

	local log_cmd="log"
	local graph=""
	local pretty=""
	local stat=""
	while [ -n "$1" ]
	do
		case "$1" in
			-p|--pretty)
				graph="--graph"
				pretty="--pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr)%Creset' --abbrev-commit --date=relative"
				;;
			-s|--stat)
				stat="--stat"
				;;
			-r|--reflog)
				log_cmd="reflog"
				;;
			*)
				break
				;;
		esac
		shift
	done

	if [ $log_cmd = reflog ]
	then
		echo git reflog $stat $pretty $wip_branch | sh
		return $?
	fi

	if ! work_last=$(git rev-parse --verify $work_branch)
	then
		die "'$work_branch' branch has no commits."
	fi

	dbg work_last=$work_last

	if ! wip_last=$(git rev-parse --quiet --verify $wip_branch)
	then
		die "'$work_branch' branch has no commits."
	fi

	dbg wip_last=$wip_last

	local base=$(git merge-base $wip_last $work_last)

	dbg base=$base

	echo git log $graph $stat $pretty $@ $wip_last $work_last "^$base~1" | sh
}

do_delete () {
	local branch=$1

	die "delete not implemented"
}

do_help () {
	local rc=$1

	cat <<END
Usage: git wip $USAGE

$LONG_USAGE
END
	exit $rc
}


if test $# -eq 0
then
	dbg "no arguments"

	do_save "WIP"
	exit $?
fi

dbg "args: $@"

case "$1" in
save)
	WIP_COMMAND=$1
	shift
	if [ -n "$1" ]
	then
		WIP_MESSAGE="$1"
		shift
	fi
	;;
info|log|delete)
	WIP_COMMAND=$1
	shift
	;;
help)
	do_help 0
	;;
--*)
	;;
*)
	[ -f "$1" ] || die "Unknown command '$1'."
	;;
esac

case $WIP_COMMAND in
save)
	do_save "$WIP_MESSAGE" $@
	;;
info)
	do_info $@
	;;
log)
	do_log $@
	;;
delete)
	do_delete $@
	;;
*)
	usage
	exit 1
	;;
esac

# vim: set noet sw=8


# EOF (git_functions.inc)

