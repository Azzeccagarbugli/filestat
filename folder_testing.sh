#!/usr/bin/env bash

AARR=( 32 {48..57} {65..90} {97..122} )
aarrcount=${#AARR[@]}

if [ "$1" == "" ] ; then
    OUTDIR="./folder_testing" ;
else
    OUTDIR="$1" ;
fi

if [ "$2" != "" ] ; then
    ASCIIONLY="$2" ;
else
    ASCIIONLY=1 ;
fi

if [ "$3" != "" ] ; then
    DIRDEPTH="$3" ;
else
    DIRDEPTH=3 ;
fi

if [ "$4" != "" ] ; then
    MAXFIRSTLEVELDIRS="$4" ;
else
    MAXFIRSTLEVELDIRS=2 ;
fi

if [ "$5" != "" ] ; then
    MAXDIRCHILDREN="$5" ;
else
    MAXDIRCHILDREN=4 ;
fi

if [ "$6" != "" ] ; then
    MAXDIRNAMELEN="$6" ;
else
    MAXDIRNAMELEN=12 ;
fi

if [ "$7" != "" ] ; then
    MAXFILECHILDREN="$7" ;
else
    MAXFILECHILDREN=4 ;
fi

if [ "$8" != "" ] ; then
    MAXFILENAMELEN="$8" ;
else
    MAXFILENAMELEN=20 ;
fi

if [ "$9" != "" ] ; then
    MAXFILESIZE="$9" ;
else
    MAXFILESIZE=20000 ;
fi

MINDIRNAMELEN=1
MINFILENAMELEN=1
MINDIRCHILDREN=1
MINFILECHILDREN=0
MINFILESIZE=1
FILEEXT=".bin"
VERBOSE=0 #1

get_rand_dirname() {
    if [ "$ASCIIONLY" == "1" ]; then
        for ((i=0; i<$((MINDIRNAMELEN+RANDOM%MAXDIRNAMELEN)); i++)) {
            printf \\$(printf '%03o' ${AARR[RANDOM%aarrcount]});
        }
    else
        cat /dev/urandom | tr -dc '[ -~]' | tr -d '[$></~:`\\]' | head -c$((MINDIRNAMELEN + RANDOM % MAXDIRNAMELEN)) | sed 's/\(["]\)/\\\1/g'
    fi
}

get_rand_filename() {
    if [ "$ASCIIONLY" == "1" ]; then
        for ((i=0; i<$((MINFILENAMELEN+RANDOM%MAXFILENAMELEN)); i++)) {
            printf \\$(printf '%03o' ${AARR[RANDOM%aarrcount]});
        }
    else
        cat /dev/urandom | tr -dc '[ -~]' | tr -d '[$></~:`\\]' | head -c$((MINFILENAMELEN + RANDOM % MAXFILENAMELEN))
    fi
    printf "%s" $FILEEXT
}

echo "Attenzione, verrà creata una serie random di directory e file su questo percorso: ($(pwd)) $OUTDIR"
[ "$VERBOSE" == "1" ] && echo "  MAXFIRSTLEVELDIRS $MAXFIRSTLEVELDIRS ASCIIONLY $ASCIIONLY DIRDEPTH $DIRDEPTH MAXDIRCHILDREN $MAXDIRCHILDREN MAXDIRNAMELEN $MAXDIRNAMELEN MAXFILECHILDREN $MAXFILECHILDREN MAXFILENAMELEN $MAXFILENAMELEN MAXFILESIZE $MAXFILESIZE"

read -p "Procedere (y/n)? " READANS
if [ "$READANS" != "y" ]; then
    exit
fi

if [ -d "$OUTDIR" ]; then
    echo "Rimozione della precedente direcotry: $OUTDIR"
    rm -rf "$OUTDIR"
fi

mkdir "$OUTDIR"

if [ $MAXFIRSTLEVELDIRS -gt 0 ]; then
    NUMFIRSTLEVELDIRS=$((1+RANDOM%MAXFIRSTLEVELDIRS))
else
    NUMFIRSTLEVELDIRS=0
fi

# create directories
for (( ifl=0;ifl<$((NUMFIRSTLEVELDIRS));ifl++ )) {
    FLDIR="$(get_rand_dirname)"
    FLCHILDREN="";
    for (( ird=0;ird<$((DIRDEPTH-1));ird++ )) {
        DIRCHILDREN=""; MOREDC=0;
        for ((idc=0; idc<$((MINDIRCHILDREN+RANDOM%MAXDIRCHILDREN)); idc++)) {
            CDIR="$(get_rand_dirname)" ;
            if [ "$DIRCHILDREN" == "" ]; then DIRCHILDREN="\"$CDIR\"" ;
        else DIRCHILDREN="$DIRCHILDREN,\"$CDIR\"" ; MOREDC=1 ; fi
        }
        if [ "$MOREDC" == "1" ] ; then
            if [ "$FLCHILDREN" == "" ]; then FLCHILDREN="{$DIRCHILDREN}" ;
        else FLCHILDREN="$FLCHILDREN/{$DIRCHILDREN}" ; fi
        else
            if [ "$FLCHILDREN" == "" ]; then FLCHILDREN="$DIRCHILDREN" ;
        else FLCHILDREN="$FLCHILDREN/$DIRCHILDREN" ; fi
        fi
    }
    DIRCMD="mkdir -p $OUTDIR/\"$FLDIR\"/$FLCHILDREN"
    eval "$DIRCMD"
    echo "$DIRCMD"
}

find "$OUTDIR" -type d | while IFS= read D ; do
    QD="$(printf '%q' "$(echo "$D")" )" ;
    [ "$VERBOSE" == "1" ] && echo "$D";
    for ((ifc=0; ifc<$((MINFILECHILDREN+RANDOM%MAXFILECHILDREN)); ifc++)) {
        CFILE="$(get_rand_filename)";
        #echo ln -s folder_testing/$CFILE folder_testing/link_$(get_rand_filename)
        echo -n '> '
        [ "$VERBOSE" == "1" ] && echo "$D"/"$CFILE"
        cat /dev/urandom \
        | head -c$((MINFILESIZE + RANDOM % MAXFILESIZE)) \
        > "$D"/"$CFILE"
        if [[ -f folder_testing/$CFILE ]]; then
            ln -s $CFILE folder_testing/link_$ifc
        fi
    }
done


echo
tree -a --dirsfirst -s "$OUTDIR"
echo "Dimensione totale dei file: $(du -bs $(echo "$OUTDIR"))"