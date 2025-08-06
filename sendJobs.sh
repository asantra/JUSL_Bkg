#! /bin/bash

nWant=${1:-"20"}
nJobs=${2:-"-1"}
runid=0


for i in $(seq 1 1 $nWant); do 
    echo $i
    b=1
    ### runid increased by one
    runid=$(( $runid + $b ))
    DESTINATION="/home/bsms.1/Monalisa/GridOutput"

    ### if main directory/run_id exists, delete
    # if [[ -d "${DESTINATION}/run_$runid" ]]; then
    #     echo "Found a directory with output ${DESTINATION}/run_$runid! Deleting the previous one."
    #     rm -rf ${DESTINATION}/run_$runid
    # fi

    #### create the run directory
    mkdir -p ${DESTINATION}"/run_"$runid"/"
    #### from where you are submitting jobs
    PRESENTDIRECTORY=${PWD}

    ### submission command
    qsub -l mem=32gb,nodes=1:ppn=2,walltime=01:00:00 -v parname1=${runid},parname2=${PRESENTDIRECTORY} -q instant -N "my_${runid}" -o "${DESTINATION}/run_$runid/my_$runid.out" -e "${DESTINATION}/run_$runid/my_$runid.err" runClusterScript.sh
    ### sleep for 1 s, so that there is no problem in submitting jobs to the grid
    sleep 1s
    ### if number of jobs required is reached then break the loop
    if [[ $runid -eq $nJobs ]]; then
        break
    fi
done