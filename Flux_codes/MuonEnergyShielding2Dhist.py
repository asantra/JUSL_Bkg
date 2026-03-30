import os, sys, time
import argparse
import ROOT
import math
import numpy as np
from ROOT import TFile, TH1D
from ROOT import TFile, TH2D
from array import array
from scipy.integrate import dblquad


def main():
   ### taking arguments from the command line
   parser = argparse.ArgumentParser()
   parser.add_argument('-f', type=str, default="Muons_815m_combined.dat")        #FOR 815M DEPTH: GeantGeneratedMuons_814m_combined.dat
   # parser.add_argument('-f2', type=str, default="Muons_554m_NR_terr_c5_4.dat")        #FOR 815M DEPTH: GeantGeneratedMuons_814m_combined.dat
   parser.add_argument('-t', type=str, default="Comaparison_at_815m_new")
   args = parser.parse_args()

   ### output root file
   outRoot = TFile("Muons_shielding_Energy_vs_dE_2D_"+args.t+".root","RECREATE")
   outRoot.cd()

   ### -------- LOG BINNING FOR X AXIS --------
   nbinsX = 100
   emin = 1       # GeV (must be > 0 for log scale)
   emax = 15000   # GeV

   log_edges = np.logspace(np.log10(emin), np.log10(emax), nbinsX + 1)
   log_edges_array = array('d', log_edges)


   histEnergy2D = TH2D("histEnergy2D", "dE vs Initial Energy; Initial Energy (GeV); dE (MeV)", nbinsX, log_edges_array, 500,0,15000)  # no title at all

   ### folder where the input files live
   inputfolder = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Muons_shielding/Depth_815m"
#    inputfolder = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Muons_815m/815mData/NewRock/SiO2_norm"        # FOR 815M DEPTH 

   ### get the text file
   filename = args.f

   fullFileName = inputfolder +"/"+filename

   ### filling up the histograms
   counter  = 0
   with open(fullFileName) as inFile:
       for lines in inFile.readlines():
           counter += 1
           if(counter%100==0): print("processed: ", counter)
           eachLine        = lines.rstrip().split()
           initial_energy  = float(eachLine[3])
           dE              = float(eachLine[13])
           histEnergy2D.Fill(initial_energy, dE)


   outRoot.cd()
   histEnergy2D.Write()
   outRoot.Close()


if __name__=="__main__":
   main()
