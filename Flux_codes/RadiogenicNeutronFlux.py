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
    parser.add_argument('-f', type=str, default="LabSpec_combined.dat")  # change the files accordingly here
    parser.add_argument('-t', type=str, default="lab")         # lab or detector
    args = parser.parse_args()

    ### output root file
    # outRoot = TFile("preliminaryHistograms_Neutrons_Nominal_Trial_Check_5_2_DETCATII_Final_check_"+args.t+".root", "RECREATE")
    outputFolder = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/RadNeutronFlux"
    outRoot = TFile("TrialCheck_555m_10_2_26_"+args.t+".root", "RECREATE")
    outRoot.cd()

    histNeutronEnergy = TH1D("histNeutronEnergy",
                             "Neutron Energy Spectrum; Energy (MeV); Flux (neutrons/cm^{2}/s/100 KeV)",
                             150, 0, 15)

    ### input folder and file
    inputfolder = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/newrun"
    filename = args.f
    fullFileName = inputfolder + "/" + filename

    # --- Geometry and physics inputs ---
    rho = 2.89   # Rock density (g/cm^3)    #2.95 for systematic up and 2.83 for systematic down or 2.89 for nominal
    r = 220.1    # Inner radius (cm)
    R = 420.1    # Outer radius (cm)
    h = 200.0    # floor thickness (cm)
    V_hemi = (2.0/3.0) * ROOT.TMath.Pi() * (R**3 - r**3)
    V_cyl = ROOT.TMath.Pi() * (R**2) * h
    V_rock = V_hemi + V_cyl
    M_rock = V_rock * rho        # Rock mass (g)
    yield_per_g_yr = 15.53     # Neutron yield per g per year #15.53  #18.07(for systematic up)  # 12.99 (for systematic down)
    yield_per_g_s = yield_per_g_yr / (365*24*3600.0)  # per g per second
    R_real = M_rock * yield_per_g_s                   # Total neutron rate (n/s)

    # ---------------------------
    # SELECT AREA BASED ON ARGUMENT
    # ---------------------------
    if args.t == "lab":
        A = 4.56e5       # lab area
    else:
        A = 2.9468e3     # detector area

    print(f"\nUsing geometry type: {args.t}")
    print(f"Effective area A = {A} cm^2")

    # --- Fill histogram with neutron energies ---
    counted_neutrons = set()
    counter = 0
    with open(fullFileName) as inFile:
        for line in inFile:
            counter += 1
            if counter % 100 == 0:
                print("processed:", counter)
            eachLine = line.rstrip().split()
            eventID  = int(eachLine[0])   # Event ID
            pdg      = int(eachLine[1])   # PDG code
            # trackID  = int(eachLine[2])
            energy = float(eachLine[3])

            # if pdg != 2112:
            #     continue
            # neutron_key = (eventID)

            # # Skip if this neutron was already counted
            # if neutron_key in counted_neutrons:
            #     continue

            # # Mark neutron as counted
            # counted_neutrons.add(neutron_key)
            histNeutronEnergy.Fill(energy)
 
    # print("Unique neutrons counted =", len(counted_neutrons))
 
    print(f"\nRock volume = {V_rock:.3e} cm^3")
    print(f"Rock mass   = {M_rock:.3e} g")
    print(f"Real neutron production rate R_real = {R_real:.3e} n/s")

    # --- Equivalent real-world simulation time ---
    T_sec = 20598740 / R_real
    print(f"Equivalent simulation time T_sec = {T_sec:.3e} s")

    # --- Normalize histogram to flux ---
    for bin_idx in range(0, histNeutronEnergy.GetNbinsX() + 1 + 1):
        N = histNeutronEnergy.GetBinContent(bin_idx)
        deltaE = histNeutronEnergy.GetBinWidth(bin_idx)  # MeV
        flux = N / (A * T_sec * deltaE)                  # neutrons/cm^2/s/MeV
        # delta_flux = math.sqrt(N) / (A * T_sec * deltaE)
        histNeutronEnergy.SetBinContent(bin_idx, flux)
        # histNeutronEnergy.SetBinError(bin_idx, delta_flux)

     # --- Integrate to get total flux ---
    total_flux = 0.0
    # total_uncertainty_sq = 0.0
    for bin_idx in range(1, histNeutronEnergy.GetNbinsX() + 1 + 1):
        flux_density = histNeutronEnergy.GetBinContent(bin_idx)  # neutrons/cm^2/s/MeV
        # delta_flux = histNeutronEnergy.GetBinError(bin_idx)
        deltaE = histNeutronEnergy.GetBinWidth(bin_idx)
        total_flux += flux_density * deltaE
        # total_uncertainty_sq += (delta_flux * deltaE)**2

    # total_uncertainty = math.sqrt(total_uncertainty_sq)

    # --- Format for scientific display (×10⁻⁴) ---
    flux_val = total_flux * 1e4
    # flux_err = total_uncertainty * 1e4
    # print(f"\nFinal integrated flux = ({flux_val:.2f} ± {flux_err:.3f}) ×10⁻⁴ cm⁻² s⁻¹")
    print(f"\nFinal integrated flux = ({flux_val:.2f}) ×10⁻⁴ cm⁻² s⁻¹")

    print("\nTotal integrated flux from histogram =",
         total_flux, "neutrons/cm^2/s")

    # --- Write both histograms ---
    outRoot.cd()
    histNeutronEnergy.Write()      # Flux density (/MeV)
    outRoot.Close()
    print("Histograms written to ROOT file.")

if __name__=="__main__":
   main()

