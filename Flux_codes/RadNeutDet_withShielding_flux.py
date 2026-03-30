# NeutronTrSpec_combined.dat = Neutrons without the backscattered ones in the lab 
# LabSpec_combined.dat = Neutrons including the backscattered ones in the lab 
# NeutronDetSpec_combined.dat = Neutrons without the backscattered ones on the detector
# DetSpec_combined.dat = Neutrons including the backscattered ones on the detector

import os, sys, time, math
import argparse
import ROOT
import math
from ROOT import TFile, TH1D

def main():
    ### taking arguments from the command line
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', type=str, default="NeutronDetSpec_22.dat")  # change the files accordingly here
    parser.add_argument('-t', type=str, default="Detector")         # lab or detector
    args = parser.parse_args()

    ### output root file
    outputFolder = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Shielding_Glycerin_data/HDPE_Shield_Data"
    outRoot = TFile(outputFolder + "GLYCERIN_HITS_OUTSide_steel_20cm_"+args.t+".root", "RECREATE")
    outRoot.cd()

    histNeutronEnergy = TH1D("histNeutronEnergy",
                             "Neutron Energy Spectrum; Energy (MeV); Flux (neutrons/cm^{2}/s/MeV)",
                             150, 0, 15)

    ### input folder and file
    inputfolder = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/hdpe_shield/hdpe_outside_steel"
    filename = args.f
    fullFileName = inputfolder + "/" + filename

    # --- Geometry and physics inputs ---
    # rho = 2.89   # Rock density (g/cm^3)    #2.95 for systematic up and 2.83 for systematic down or 2.89 for nominal
    # r = 220.1    # Inner radius (cm)
    # R = 420.1    # Outer radius (cm)
    # h = 200.0    # floor thickness (cm)
    # V_hemi = (2.0/3.0) * ROOT.TMath.Pi() * (R**3 - r**3)
    # V_cyl = ROOT.TMath.Pi() * (R**2) * h
    # V_rock = V_hemi + V_cyl
    # M_rock = V_rock * rho        # Rock mass (g)
    # yield_per_g_yr = 15.53     # Neutron yield per g per year #15.53  #18.07(for systematic up)  # 12.99 (for systematic down)
    # yield_per_g_s = yield_per_g_yr / (365*24*3600.0)  # per g per second
    # R_real = M_rock * yield_per_g_s                   # Total neutron rate (n/s)

    # ---------------------------
    # SELECT AREA BASED ON ARGUMENT
    # ---------------------------
    # if args.t == "lab":
    #     A = 4.56e5       # lab area
    # else:
    #     A = 2.9468e3     # detector area

    # print(f"\nUsing geometry type: {args.t}")
    # print(f"Effective area A = {A} cm^2")

    # --- Fill histogram with neutron energies ---
    counter = 0
    with open(fullFileName) as inFile:
        for line in inFile:
            counter += 1
            if counter % 100 == 0:
                print("processed:", counter)
            eachLine = line.rstrip().split()
            energy = float(eachLine[3])
            histNeutronEnergy.Fill(energy)

    # print(f"\nRock volume = {V_rock:.3e} cm^3")
    # print(f"Rock mass   = {M_rock:.3e} g")
    # print(f"Real neutron production rate R_real = {R_real:.3e} n/s")

    Neutron_root_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/NeutronHistogramPlots/preliminaryHistograms_Neutrons_Nominal_DETCATII_1_1_26_DETCATII_withoutErrorBars.root"         # for 555m DEPTH
    # muon_root_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/MuonFlux_815m/preliminaryHistograms_Muons2_secondary3.root"        # for 815m DEPTH
    f = TFile(Neutron_root_file, "READ")
    hist = f.Get("histNeutronEnergy") 
    total_flux_root = hist.Integral(0, hist.GetNbinsX()+1, "width")

    f.Close()
    print(f"Total Neutron flux = {total_flux_root:.3e} cm^-2 s^-1")

    # --- Equivalent real-world simulation time ---
    Area = 2.9468e3 
    T_sec = 1000000 / (total_flux_root * Area) 
    print(f"Equivalent simulation time T_sec = {T_sec:.3e} s")

    # A = 3.7699e2         # r = 4cm, h = 11cm , cylyndrical area
    # A = 1.3823e2         # r = 4cm, h = 11cm , half curved area of cylinder
    # A = 1.3823e2         # r = 4cm, h = 11cm , half curved area of cylinder
    # A = 88         # r = 4cm, diameter = 8cm, h = 11cm , rectangular area of the cylinder
    A = 70                # r = 3.5cm, diameter = 7cm, h = 10cm. 

    # --- Normalize histogram to flux ---
    for bin_idx in range(0, histNeutronEnergy.GetNbinsX() + 1 + 1):
        N = histNeutronEnergy.GetBinContent(bin_idx)
        deltaE = histNeutronEnergy.GetBinWidth(bin_idx)  # MeV
        # flux = N / (A * T_sec * deltaE)                  # neutrons/cm^2/s/MeV
        flux = N / (A * 1000000 * deltaE)                  # neutrons/cm^2/s/MeV
        # delta_flux = math.sqrt(N) / (A * T_sec * deltaE)
        delta_flux = math.sqrt(N) / (A * 1000000 * deltaE)
        histNeutronEnergy.SetBinContent(bin_idx, flux)
        histNeutronEnergy.SetBinError(bin_idx, delta_flux)

     # --- Integrate to get total flux ---
    total_flux = 0.0
    total_uncertainty_sq = 0.0
    for bin_idx in range(1, histNeutronEnergy.GetNbinsX() + 1 + 1):
        flux_density = histNeutronEnergy.GetBinContent(bin_idx)  # neutrons/cm^2/s/MeV
        delta_flux = histNeutronEnergy.GetBinError(bin_idx)
        deltaE = histNeutronEnergy.GetBinWidth(bin_idx)
        total_flux += flux_density * deltaE
        total_uncertainty_sq += (delta_flux * deltaE)**2

    total_uncertainty = math.sqrt(total_uncertainty_sq)

    # --- Format for scientific display (×10⁻⁴) ---
    flux_val = total_flux * 1e4
    flux_err = total_uncertainty * 1e4
    print(f"\nFinal integrated flux = ({flux_val:.3f} ± {flux_err:.3f}) ×10⁻⁴ cm⁻² primary⁻¹")

    print("\nTotal integrated flux from histogram =",
         total_flux, "neutrons/cm^2/primary")

    # --- Write both histograms ---
    outRoot.cd()
    histNeutronEnergy.Write()      # Flux density (/MeV)
    outRoot.Close()
    print("Histograms written to ROOT file.")

if __name__=="__main__":
   main()

