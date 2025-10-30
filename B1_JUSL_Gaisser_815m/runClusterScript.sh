#PBS -S /bin/bash
#PBS -m n

runid=${parname1}
presentdir=${parname2}
echo "Working on runid: "$runid" from "${presentdir}
#### going to the directory with executables
cd ${presentdir}/build
### activate conda geant4
echo "Activating conda"
source /home/bsms.1/Monalisa/anaconda3/bin/activate
conda init
conda activate myg4
### run the script
echo "runnng the script"
cp runMASTER.mac runGrid$runid.mac
sed -i -e "s|IIIII|$runid|g" runGrid$runid.mac
echo "I am running on runGrid$runid.mac"
time ./exampleB1 runGrid$runid.mac

### putting some output after the run
echo "The simulation ran. I am exiting!"

