import ROOT
import math

# ROOT style
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)

# # --- Input ROOT files ---
# muon_file       = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/MuonFlux_815m/preliminaryHistograms_Muons2_trial_LogBinning.root"
# radiogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/RadNeutronFlux/preliminaryHistograms_Neutrons_LogBinning.root"
# cosmogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/CosmNeutronFlux/FluxHistograms_cosmogenic.root"
# muon_file       = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Final_Flux/depth_555m/preliminaryHistograms_Muons_secondary.root"
# radiogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Final_Flux/depth_555m/preliminaryHistograms_Neutrons_Nominal_LABCATII_1_1_26_lab_withoutErrorBars.root"
# cosmogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Final_Flux/depth_555m/CosmogenicNeutron_NOMINAL_Depth_555m_84M_5_2_26_LabCatII_WithoutErrorBars_LABCATII_COMBINED.root"
muon_file       = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Final_Flux/depth_815m/preliminaryHistograms_Muons2_secondary3.root"
radiogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Final_Flux/depth_815m/preliminaryHistograms_Neutrons_Nominal_LABCATII_1_1_26_lab_withoutErrorBars.root"
cosmogenic_file = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Final_Flux/depth_815m/CosmogenicNeutron_NOMINAL_Depth_815m_80M_5_2_26_LabCATII_WithoutErrorBars_LABCATII_COMBINED.dat.root"

# --- Load histograms ---
f_muon = ROOT.TFile(muon_file)
h_muon = f_muon.Get("histEnergy")
h_muon.SetLineColor(ROOT.kBlue)
h_muon.SetLineWidth(1)
h_muon.SetTitle("")
h_muon.GetYaxis().SetTitle("Flux (particles/cm^{2}/s/GeV)")

f_cosmogenic = ROOT.TFile(cosmogenic_file)
# h_cosmogenic = f_cosmogenic.Get("histCosm_Log")
h_cosmogenic = f_cosmogenic.Get("histLinearEnergy")
# --- Convert cosmogenic histogram from MeV to GeV ---
nbins_cos = h_cosmogenic.GetNbinsX()
x_min_cos = h_cosmogenic.GetXaxis().GetXmin() / 1000.0
x_max_cos = h_cosmogenic.GetXaxis().GetXmax() / 1000.0

h_cosmogenic_GeV = ROOT.TH1D("h_cosmogenic_GeV",
                             "Cosmogenic Neutrons",
                             nbins_cos, x_min_cos, x_max_cos)

for b in range(1, nbins_cos + 1):
    content = h_cosmogenic.GetBinContent(b)
    error   = h_cosmogenic.GetBinError(b)

    # Scale for GeV (same logic as radiogenic)
    h_cosmogenic_GeV.SetBinContent(b, content * 1000.0)
    h_cosmogenic_GeV.SetBinError(b, error * 1000.0)

h_cosmogenic_GeV.SetLineColor(ROOT.kGreen)
h_cosmogenic_GeV.SetLineWidth(1)
h_cosmogenic_GeV.SetTitle("Cosmogenic Neutrons")
h_cosmogenic_GeV.GetYaxis().SetTitle("Flux (particles/cm^{2}/s/GeV)")

f_radiogenic = ROOT.TFile(radiogenic_file)
h_radiogenic = f_radiogenic.Get("histNeutronEnergy")
# --- Convert radiogenic histogram from MeV to GeV ---
nbins = h_radiogenic.GetNbinsX()
x_min = h_radiogenic.GetXaxis().GetXmin() / 1000.0
x_max = h_radiogenic.GetXaxis().GetXmax() / 1000.0

h_radiogenic_GeV = ROOT.TH1D("h_radiogenic_GeV",
                            "Radiogenic Neutrons",
                            nbins, x_min, x_max)

for b in range(1, nbins + 1):
    content = h_radiogenic.GetBinContent(b)
    error   = h_radiogenic.GetBinError(b)

    # IMPORTANT: scale for GeV
    h_radiogenic_GeV.SetBinContent(b, content * 1000.0)
    h_radiogenic_GeV.SetBinError(b, error * 1000.0)

h_radiogenic_GeV.SetLineColor(ROOT.kRed)
h_radiogenic_GeV.SetLineWidth(1)

# h_radiogenic.SetLineColor(ROOT.kRed)
# h_radiogenic.SetLineWidth(1)
h_radiogenic_GeV.SetTitle("Radiogenic Neutrons")
h_radiogenic_GeV.GetYaxis().SetTitle("Flux (particles/cm^{2}/s/GeV)")

# --- Create total flux histogram by summing the three ---
h_total = h_muon.Clone("h_total")
h_total.Reset()

nbins_total = h_total.GetNbinsX()
for b in range(1, nbins_total + 1):
    val = h_muon.GetBinContent(b) + h_radiogenic_GeV.GetBinContent(b) + h_cosmogenic_GeV.GetBinContent(b)
    h_total.SetBinContent(b, val)

h_total.SetLineColor(ROOT.kBlack)
h_total.SetLineWidth(1)
h_total.SetLineStyle(2)
h_total.SetTitle("Total Flux")

# --- Create canvas ---
c = ROOT.TCanvas("c", "Flux Comparison", 800, 600)
c.SetLogx()
c.SetLogy()

# # --- Frame for axes (10^-3 GeV to 10^5 GeV) ---
frame = ROOT.TH2D("frame", ";Energy [GeV];Flux [cm^{-2} s^{-1} GeV^{-1}]",
                  100, 1e-3, 1e4, 100, 1e-14, 1)

frame.GetXaxis().SetTitleSize(0.045)
frame.GetYaxis().SetTitleSize(0.045)
frame.Draw()

# --- Draw all histograms ---
h_muon.GetXaxis().SetTitleSize(0.045)
h_muon.GetYaxis().SetTitleSize(0.045)
h_muon.GetYaxis().SetTitleOffset(0.95)
h_muon.GetYaxis().SetRangeUser(1e-14,1)

# h_muon.GetXaxis().SetRangeUser(1e-3, 1e4)

h_muon.Draw("HIST SAME")
h_cosmogenic_GeV.Draw("HIST SAME")
h_radiogenic_GeV.Draw("HIST SAME")
# h_total.Draw("HIST SAME")  # Draw the total flux

# --- Legend ---
legend = ROOT.TLegend(0.6, 0.65, 0.88, 0.9)
legend.SetBorderSize(0)
legend.SetFillStyle(0)
legend.AddEntry(h_muon, "Cosmic Muon", "l")
legend.AddEntry(h_radiogenic_GeV, "Radiogenic Neutron", "l")
legend.AddEntry(h_cosmogenic_GeV, "Cosmogenic Neutron", "l")
# legend.AddEntry(h_total, "Total Flux (Sum)", "l")  # Add total flux to legend
legend.Draw()

# --- Optional text label ---
label = ROOT.TLatex()
label.SetNDC(True)
label.SetTextSize(0.035)
label.DrawLatex(0.14, 0.93, "Flux Spectra Comparison at 815 m")

out_dir = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/Final_Flux/depth_815m"

# --- Save output ---
c.SaveAs(out_dir + "/Combined_Flux_Comparison_withTotal_at815m_correct.pdf")
c.SaveAs(out_dir + "/Combined_Flux_Comparison_withTotal_at815m_correct.png")

root_out = ROOT.TFile(out_dir + "/Combined_Flux_Comparison_at815m_correct.root", "RECREATE")

# Write histograms
h_muon.Write("h_muon")
h_radiogenic_GeV.Write("h_radiogenic")
h_cosmogenic_GeV.Write("h_cosmogenic")
# h_total.Write("h_total")

# Write canvas (very useful to reopen exactly as plotted)
c.Write("canvas_flux")

# Close file
root_out.Close()

print("ROOT file saved with histograms and canvas")
# print("Muon Xmin:", h_muon.GetXaxis().GetXmin())
# print("Radiogenic Xmin:", h_radiogenic_GeV.GetXaxis().GetXmin())
# print("Cosmogenic Xmin:", h_cosmogenic.GetXaxis().GetXmin())