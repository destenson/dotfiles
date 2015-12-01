#!/bin/env bash

function make_fun()
{
    local force=;
    local f=;
    while [ $# -gt 0 ]; do
        case "$1" in
            -f | --force)
                force=true;
                shift
            ;;
            *)
                f="$1";
                shift;
				break
            ;;
        esac;
    done;
    [ $# -gt 0 ] || {
        eecho "Usage: make-fun-from-template.sh [function-name] <-f | --force>";
        return 1
    };
    [[ -e TEMPLATE && -r TEMPLATE ]] || {
        eecho "TEMPLATE file was not found, giving up";
        return 2
    };
    [[ -e "$f" && -z $force ]] && {
        eecho "A file named \"$f\" already exists. Delete it and try again if you really want to (or use --force).";
        return 3
    };
    cat TEMPLATE | sed "s/TEMPLATE/$f/g" > "$f"
}

#type make_fun

make_fun "$@" && et || ef
exit $?

# EOF make-fun-from-template.sh
