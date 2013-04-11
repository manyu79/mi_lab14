#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
VNAME1="dxyazdi"      # The first vehicle community

for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES] [time_warp]   \n" $0
	printf "  --just_make, -j    \n" 
	printf "  --hazards=file.txt \n" 
	printf "  --help, -h         \n" 
	exit 0;
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done


#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------
START_POS1="0,0"  

nsplug meta_kf.moos kf_$VNAME1.moos -f WARP=$TIME_WARP  \
   VNAME=$VNAME1      START_POS=$START_POS1                    \
   VPORT="9002"       SHARE_LISTEN="9301"                      \
   VTYPE=UUV          SHORE_LISTEN=$SHORE_LISTEN               \
   MASTER="true"      OFFSET="10"                               \
   WIDTH="10"         PATH_WIDTH="5"                           \
   PD="0.98"


nsplug meta_kf.bhv kf_$VNAME1.bhv -f VNAME=$VNAME1      \
    START_POS=$START_POS1 

if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler kf_$VNAME1.moos >& /dev/null &
sleep .25
printf "Done \n"

uMAC kf_$VNAME1.moos

printf "Killing all processes ... \n"
kill %1
printf "Done killing processes.   \n"


