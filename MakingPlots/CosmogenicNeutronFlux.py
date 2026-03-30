import os, sys, time
import argparse
import ROOT
import math
import numpy as np
from ROOT import TFile, TH1D, TH1
from scipy.integrate import dblquad

def main():
   TH1.SetDefaultSumw2()
   ### taking arguments from the command line
   parser = argparse.ArgumentParser()
   parser.add_argument('-f', type=str, default="CosmNeutronTrSpec_combined.dat")  # change the .dat files accoridngly
   parser.add_argument('-t', type=str, default="lab")                  # lab or detector
   args = parser.parse_args()

   ### output root file
   outRoot = TFile("preliminaryHist_CosmogenicNeutron_Nominal(DetCatII)_555m_"+args.f+".root", "RECREATE")    # change the name accordingly
   outRoot.cd()

   nbins = 1000
   emin = 1   # MeV
   emax = 1e5   # MeV
   log_edges = np.logspace(np.log10(emin), np.log10(emax), nbins + 1)

    # Create the histogram with variable bins
   histLogEnergy = ROOT.TH1D("histLogEnergy", 
                           "Energy vs Flux; Energy (MeV); Flux (neutrons/cm^{2}/s/MeV)",
                      nbins, log_edges)

   #    )
   histLinearEnergy =TH1D("histLinearEnergy", "Energy vs Flux; Energy (MeV); Flux (neutrons/cm^{2}/s/MeV)", 100000, 0, 100000)

#    ### output root file
#    outRoot = TFile("preliminaryHistograms_Muons2_trial_"+args.t+".root","RECREATE")
#    outRoot.cd()
#    histEnergy =TH1D("histEnergy", "Energy vs Flux; Energy (GeV); Flux (muons/cm^{2}/s/MeV)", 1000, 0, 10000)

   ### folder where the input files live
   inputfolder = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/CosmNeut/Cosmogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/depth_555m"          # OR depth_815m


   ### get the text file
   filename = args.f

   fullFileName = inputfolder +"/"+filename 

   # ---------------------------
   # SELECT AREA BASED ON ARGUMENT
   # ---------------------------
   if args.t == "lab":
      A = 3.04e5      # lab area     = 2π(220.1)^2cm^2
   else:
      A = 2.9468e3    # detector area  = 2𝜋𝑟ℎ + 2𝜋𝑟2 = 2𝜋(7.0)(60.0) + 2𝜋(49) ≈ 2638.9cm2 + 307.9cm2 = 2946.8 cm²

   print(f"\nUsing geometry type: {args.t}")
   print(f"Effective area A = {A} cm^2")    

   ### filling up the histograms
   counter  = 0
   with open(fullFileName) as inFile:
       for lines in inFile.readlines():
           counter += 1
           if(counter%100==0): print("processed: ", counter)
           eachLine    = lines.rstrip().split()
           energy = float(eachLine[3]) * 1000                #GeV to MeV
           histLogEnergy.Fill(energy)
           histLinearEnergy.Fill(energy)

   print("just integral: log: ", histLogEnergy.Integral(0,histLogEnergy.GetNbinsX()+1), " linear: ", histLinearEnergy.Integral(0, histLinearEnergy.GetNbinsX()+1) )

       # ----------------------------------------------------------------
       #  Calculate total integrated flux from histogram
       # ----------------------------------------------------------------
   A_outer =  1.108e6     # 2π(420.1)^2cm^2
   #total_flux_root = 1.2702839e-07 # 5.99e-07
   muon_root_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/FluxHistogram/preliminaryHistograms_Muons_secondary_00.root"         # for 555m DEPTH
   # muon_root_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/MuonFlux_815m/preliminaryHistograms_Muons2_secondary3.root"        # for 815m DEPTH
   f = TFile(muon_root_file, "READ")
   hist = f.Get("histEnergy") 
   total_flux_root = hist.Integral("width") 
   f.Close()
   print(f"Total muon flux = {total_flux_root:.3e} cm^-2 s^-1")
   T_eff = 40000000 / (total_flux_root * A_outer)
   #Neutron_flux = 41843 / (A_inner * T_eff)
   #print("\n Neutron flux per per 100 MeV =", Neutron_flux, "Neutrons/cm^2/s")
   for bin_idx in range(1, histLogEnergy.GetNbinsX() + 1):
      N = histLogEnergy.GetBinContent(bin_idx)
      deltaE = histLogEnergy.GetBinWidth(bin_idx)
      flux = N / (A * T_eff * deltaE)
      delta_flux = math.sqrt(N) / (A * T_eff * deltaE)
      histLogEnergy.SetBinContent(bin_idx, flux)
      histLogEnergy.SetBinError(bin_idx, delta_flux)

   total_flux_root = histLogEnergy.Integral("width")
   print("\nTotal integrated flux from log histogram (ROOT Integral) =", total_flux_root, "neutrons/cm^2/s")

   total_flux_log = 0.0
   total_uncertainty_sq = 0.0
   for bin_idx in range(1, histLogEnergy.GetNbinsX() + 1):
        flux_density = histLogEnergy.GetBinContent(bin_idx)  # cm^-2 s^-1 GeV^-1
        delta_flux = histLogEnergy.GetBinError(bin_idx)       # cm^-2 s^-1 GeV^-1
        deltaE = histLogEnergy.GetBinWidth(bin_idx)           # GeV

        # Integrated total flux
        total_flux_log += flux_density * deltaE
        # Integrated uncertainty in quadrature
        total_uncertainty_sq += (delta_flux * deltaE) ** 2

   total_uncertainty = math.sqrt(total_uncertainty_sq)
   flux_val = total_flux_log * 1e8
   flux_err = total_uncertainty *1e8
   print(f"\nFinal integrated flux (log) = ({flux_val:.2f} ± {flux_err:.2f}) * 1e-8 cm⁻² s⁻¹") 


   for bin_idx in range(1, histLinearEnergy.GetNbinsX() + 1):
      N = histLinearEnergy.GetBinContent(bin_idx)
      deltaE = histLinearEnergy.GetBinWidth(bin_idx)
      flux = N / (A * T_eff * deltaE)
      delta_flux = math.sqrt(N) / (A * T_eff * deltaE)
      histLinearEnergy.SetBinContent(bin_idx, flux)
      histLinearEnergy.SetBinError(bin_idx, delta_flux)

   total_flux_root = histLinearEnergy.Integral("width")
   print("\nTotal integrated flux from histogram (ROOT Integral) =", total_flux_root, "neutrons/cm^2/s")

      #   print("\n--------------------------------------")
      #   print(f"Total integrated flux (Linear) = {total_flux_linear:.3e} cm^-2 s^-1")
      #   print("--------------------------------------")

   total_flux_linear = 0.0
   total_uncertainty_sq = 0.0
   for bin_idx in range(1, histLinearEnergy.GetNbinsX() + 1):
        flux_density = histLinearEnergy.GetBinContent(bin_idx)  # cm^-2 s^-1 GeV^-1
        delta_flux = histLinearEnergy.GetBinError(bin_idx)       # cm^-2 s^-1 GeV^-1
        deltaE = histLinearEnergy.GetBinWidth(bin_idx)           # GeV

        # Integrated total flux
        total_flux_linear += flux_density * deltaE
        # Integrated uncertainty in quadrature
        total_uncertainty_sq += (delta_flux * deltaE) ** 2

   total_uncertainty = math.sqrt(total_uncertainty_sq)
   flux_val = total_flux_linear * 1e8
   flux_err = total_uncertainty *1e8
   print(f"\nFinal integrated flux (linear) = ({flux_val:.2f} ± {flux_err:.2f}) * 1e-8 cm⁻² s⁻¹") 

       # ----------------------------------------------------------------

   outRoot.Write()
   outRoot.Close()


if __name__=="__main__":
   main()