import os, sys, time
import argparse
import ROOT
from ROOT import TFile, TH1D

def main():
    ### taking arguments from the command line
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', type=str, default="GeantGeneratedMuons_554m_NR_terr_c5_1.dat")
    parser.add_argument('-t', type=str, default="primary")
    args = parser.parse_args()

    ### output root file
    outRoot = TFile("preliminaryHistograms_"+args.t+".root","RECREATE")
    outRoot.cd()
    histEnergy = TH1D("histEnergy","histEnergy; Energy (GeV); Entries",1000,0,10000)
    histPosX   = TH1D("histPosX","histPosX; x (m); Entries",1000,-3000,3000)
    histPosY   = TH1D("histPosY","histPosY; y (m); Entries",1000,-3000,3000)

    ### folder where the input files live
    inputfolder = "/home/monalisa/JUSL_Bkg/JUSLFiles/Muons/555mData/NewRock/SiO2_norm/"

    ### get the text file
    filename = args.f

    fullFileName = inputfolder +"/"+filename

    ### filling up the histograms
    counter  = 0
    with open(fullFileName) as inFile:
        for lines in inFile.readlines():
            counter += 1
            if(counter%100==0): print("processed: ", counter)
            if args.t=="primary":
                eachLine    = lines.rstrip().split()
                energy      = float(eachLine[1])
                posX        = float(eachLine[3])
                posY        = float(eachLine[4])
                histEnergy.Fill(energy)
                histPosX.Fill(posX)
                histPosY.Fill(posY)
            else:
                eachLine    = lines.rstrip().split()
                energy      = float(eachLine[4])
                posX        = float(eachLine[7])
                posY        = float(eachLine[8])
                histEnergy.Fill(energy)
                histPosX.Fill(posX)
                histPosY.Fill(posY)

  
# Create a canvas
    canvas = ROOT.TCanvas("canvas", "Muon Histograms", 800, 600)
    canvas.SetGrid()

# --- Energy Plot ---
    histEnergy.SetLineColor(ROOT.kRed + 1)
    histEnergy.SetLineWidth(2)
    histEnergy.Draw()
    canvas.SaveAs("histEnergy_"+args.t+".png")

# --- PosX Plot ---
    canvas.Clear()
    histPosX.SetLineColor(ROOT.kBlue + 1)
    histPosX.SetLineWidth(2)
    histPosX.Draw()
    canvas.SaveAs("histPosX_"+args.t+".png")

# --- PosY Plot ---
    canvas.Clear()
    histPosY.SetLineColor(ROOT.kGreen + 2)
    histPosY.SetLineWidth(2)
    histPosY.Draw()
    canvas.SaveAs("histPosY_"+args.t+".png")

    print("Histograms plotted and saved.")


    outRoot.Write()
    outRoot.Close()


if __name__=="__main__":
    main()