#!/bin/env bash

function make_fun()
{
    local f force
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
        eecho "Usage: make-fun-from-function.sh [function-name] <-f | --force>";
        return 1
    };
    [[ -e "$f" && -z $force ]] && {
        eecho "A file named \"$f\" already exists. Delete it and try again if you really want to (or use --force).";
        return 3
    };
	[ type "$f" | egrep 'is a function$' &> /dev/null ] && \
	type "$f" | tail -n +2 > "$f" &&
	( echo '#!/bin/env bash

'; cat "$f"; echo '

# EOF $f
' ) > "$f"
}

#type make_fun

make_fun "$@" && dbg_et || dbg_ef
exit $?

# EOF make-fun-from-template.sh
