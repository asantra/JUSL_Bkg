#PBS -S /bin/bash
#PBS -q instant
#PBS -l nodes=1:ppn=20
#PBS -l mem=64gb
#PBS -l walltime=06:00:00
#PBS -N myscript
#PBS -o myscript.out
#PBS -e myscript.err

#### going to the directory with executables
cd /home/bsms.1/Monalisa/JUSL_Bkg/B1_JUSL_Gaisser/build
### activate conda geant4
echo "Activating conda"
source /home/bsms.1/Monalisa/anaconda3/bin/activate
conda init
conda activate myg4
### run the script
echo "runnng the script"
./exampleB1 run2.mac

### putting some output after the run
echo "The simulation ran. I am exiting!"
