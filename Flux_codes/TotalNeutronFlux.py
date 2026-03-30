import ROOT

# -----------------------------
# Input ROOT files (FULL PATHS)
# -----------------------------
radiogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/NeutronHistogramPlots/preliminaryHistograms_Neutrons_Nominal_5_2_LABCATII_rebinning_lab.root"
cosmogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/CosmNeutronFlux/CosmogenicNeutron_NOMINAL_Depth_555m_84M_5_2_26_LabCatII_WithoutErrorBars_LABCATII_COMBINED.dat.root"

# -----------------------------
# Open files
# -----------------------------
f_rad  = ROOT.TFile(radiogenic_file, "READ")
f_cosm = ROOT.TFile(cosmogenic_file, "READ")

# -----------------------------
# Get histograms
# -----------------------------
h_rad  = f_rad.Get("histNeutronEnergy")   # radiogenic
h_cosm = f_cosm.Get("histLogEnergy")       # cosmogenic

# -----------------------------
# Add histograms (TH1D::Add)
# -----------------------------
h_rad.Add(h_cosm)

# -----------------------------
# Output ROOT file
# -----------------------------
output_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/TotalNeutronFlux/TotalNeutronFlux.root"

f_out = ROOT.TFile(output_file, "RECREATE")
h_rad.Write("histTotalNeutronFlux")
f_out.Close()