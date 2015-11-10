#!/bin/bash

EXE_NAME=$0
#. cmn.inc

TESTING=yes

# TODO: take an existing repo init command line and mirror it

cmdline="mirror-repo.sh init -b release -m LA.BF64.1.2.1.c1-02410-8x94.0.xml -u git://codeaurora.org/platform/manifest.git --repo-url=git://codeaurora.org/tools/repo.git"
#cmdline="$*"

cmdline="${cmdline} --mirror"

echo $EXE_NAME && exit 1

replace_existing_tree_ok()
{
	if [ "x$REPLACE_EXISTING_TREE" == "xyes" ]; then
		return 0
	else
		return 1
	fi
}

replace_existing_tree()
{
	if [[ replace_existing_tree && -e $TREE_DIR ]]; then
		echo "Removing old programming directory"
		#TODO: Check if there are unsaved changes
		rm -rf $TREE_DIR
		eif
	fi
}

if [ "x$TESTING" == "xyes" ]; then
	REPLACE_EXISTING_TREE=yes
	echo "$EXE - Create Programming Tree in $TREE_DIR, replacing it if it already exists"
else
	echo "$EXE - Create Programming Tree in $TREE_DIR"
fi

echo;

MANIFEST="$LOCAL_MANIFEST"
if [ -e "$1" ]; then
	MANIFEST="$1"
fi

if [ "x$MANIFEST" == "x" ]; then
	echo "Must specify repo manifest file"
	exit 1
else
	if [ -e "$MANIFEST/default.xml" ]; then
		MANIFEST="file://$MANIFEST"
	else
		curl $MANIFEST 2> /dev/null
		if [ $? -ne 0 ]; then
			echo "Manifest location ($MANIFEST) was not found"
			exit 1
		fi
	fi
fi

replace_existing_tree
echo "Making programming directory at: $TREE_DIR"
mkdir $TREE_DIR && cd $TREE_DIR
eif

echo "Initializing manifest from: $MANIFEST"
repo init -u $MANIFEST 2> /dev/null || eif

echo "Syncing repos from the manifest"
repo sync -j 4 2> /dev/null || eif

echo "Running one-time configuration script"
./COMMON/BUILD/bin/runme1st.sh || eif

echo; echo "$EXE_NAME completed normally"
exit 0

# EOF
