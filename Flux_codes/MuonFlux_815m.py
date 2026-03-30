import os, sys, time
import argparse
import ROOT
import math
import numpy as np
from ROOT import TFile, TH1D
from scipy.integrate import dblquad


def main():
   ### taking arguments from the command line
   parser = argparse.ArgumentParser()
   parser.add_argument('-f', type=str, default="Muons_814m_CombinedNew.dat")
   parser.add_argument('-t', type=str, default="primary")
   args = parser.parse_args()


   ### output root file
   outRoot = TFile("Muons_Flux_815m_New_"+args.t+".root","RECREATE")
   outRoot.cd()
   histEnergy =TH1D("histEnergy", "Energy vs Flux; Energy (GeV); Flux (muons/cm^{2}/s/GeV)", 1000, 0, 10000)

   ### folder where the input files live
#    inputfolder = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Muons_815m/815mData/NewRock/SiO2_norm"
   inputfolder = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Muons_815m/NewRuns"


   ### get the text file
   filename = args.f

   fullFileName = inputfolder +"/"+filename 

#    A = 9e10  # 9 km^2 in cm^2 #4400m*4400m in cm2
   A = 1.936e11  # 9 km^2 in cm^2 #4400m*4400m in cm2

   #A_det = 96.4 * 32.1 #in cm^2

   theta_list = []


   ### filling up the histograms
   counter  = 0
   with open(fullFileName) as inFile:
       for lines in inFile.readlines():
           counter += 1
           if(counter%100==0): print("processed: ", counter)
           if args.t=="primary":
               eachLine    = lines.rstrip().split()
               energy      = float(eachLine[1])
               histEnergy.Fill(energy)
               
           else:
               eachLine    = lines.rstrip().split()
               energy      = float(eachLine[4])
               histEnergy.Fill(energy)

   # Estimate theta_max from the data
   #theta_max = max(theta_list)

   def gaisser(E, theta):
       """Gaisser parameterization (E in GeV, theta in radians)."""
       return 0.14 * E**-2.7 * (
           1.0 / (1 + 1.1 * E * np.cos(theta) / 115.0)
           + 0.054 / (1 + 1.1 * E * np.cos(theta) / 850.0)
        )

   def integrand(theta, E):
       return gaisser(E, theta) * np.sin(theta) * 2*np.pi

   exp_flux, flux_err = dblquad(
       integrand,
       400, 15000,   # E limits
       lambda E: 0, lambda E: 7*np.pi/18   # theta limits
   )
   print("exp_flux =", exp_flux, "cm^-2 s^-1")

   #exp_flux = 3.026266755805645e-06  # muons/cm^2/s
   #deltaOmega = 2 * math.pi * (1 - math.cos(theta_max))  # sr
   total_muons = histEnergy.GetEntries()
   T_sec = 900000 / (A * exp_flux)  # seconds                      #1335000 #900000
   #E = histEnergy.Fill(energy)         

   # Normalize histogram to flux
   for bin_idx in range(1, histEnergy.GetNbinsX() + 1):
       N = histEnergy.GetBinContent(bin_idx)
       #T_sec = N / (A * exp_flux) 
       deltaE = histEnergy.GetBinWidth(bin_idx)  # GeV
       flux = N / (A * T_sec * deltaE)
       delta_flux = math.sqrt(N) / (A * T_sec * deltaE)
       histEnergy.SetBinContent(bin_idx, flux)
       histEnergy.SetBinError(bin_idx, delta_flux)


       print(f"Bin {bin_idx}: N = {N}, deltaE = {deltaE}, flux = {flux}, T_sec = {T_sec}")
       print(f"Statistical uncertainty (Poisson) = {delta_flux:.3e} cm⁻² s⁻¹ GeV⁻¹")
       # ----------------------------------------------------------------
       #  Calculate total integrated flux from histogram
       # ----------------------------------------------------------------
   total_flux_root = histEnergy.Integral("width")
   print("\nTotal integrated flux from histogram (ROOT Integral) =",
         total_flux_root, "muons/cm^2/s")

   # Manual check
   total_flux_manual = 0.0
   total_uncertainty_sq = 0.0
   for bin_idx in range(1, histEnergy.GetNbinsX() + 1):
        flux_density = histEnergy.GetBinContent(bin_idx)  # cm^-2 s^-1 GeV^-1
        delta_flux = histEnergy.GetBinError(bin_idx)       # cm^-2 s^-1 GeV^-1
        deltaE = histEnergy.GetBinWidth(bin_idx)           # GeV

        # Integrated total flux
        total_flux_manual += flux_density * deltaE
        # Integrated uncertainty in quadrature
        total_uncertainty_sq += (delta_flux * deltaE) ** 2
   print("\nTotal integrated flux from histogram (Manual) =",
         total_flux_manual, "muons/cm^2/s")
   total_uncertainty = math.sqrt(total_uncertainty_sq)
   flux_val = total_flux_manual * 1e7
   flux_err = total_uncertainty * 1e7
   print(f"\nFinal integrated flux = ({flux_val:.2f} ± {flux_err:.2f}) ×10⁻⁷ cm⁻² s⁻¹") 
       # ----------------------------------------------------------------


   outRoot.Write()
   outRoot.Close()


if __name__=="__main__":
   main()
