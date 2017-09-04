#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)



from ROOT import TFile, TH1, TObject

# merge all the histograms to one
def add_signal_background_uncer(background,signal_region_mc, background_region_mc,output_fname):
    TH1.AddDirectory(ROOT.kFALSE);
    print '='*10
    print 'Adding the signal to background region uncertainty'
    print 'background',background
    print 'signal region',signal_region_mc,'background region',background_region_mc
    print 'output file',output_fname
    
    signal_mc_file = TFile(signal_region_mc)
    background_mc_file = TFile(background_region_mc)
    back = TFile(background)

    
    keys = back.GetListOfKeys()
 
    hist_names =[]
    histos = []
    cloned = []
    
    for key in keys:
        key = str(key.GetName())
	print key
        histos.append(back.Get(key).Clone())
        diff_hist =  signal_mc_file.Get(key.replace('Background','DATA')).Clone()
	nom_entries = diff_hist.GetEntries()
	diff_hist.Scale(1/nom_entries)
	back_mc_h = background_mc_file.Get(key).Clone()
        back_mc_h.Scale(1/back_mc_h.GetEntries())
        diff_hist.Add(back_mc_h,-1)
	diff_hist.Scale(nom_entries)
	for i in xrange(diff_hist.GetNcells()):
	  diff_hist.SetBinContent(i, math.fabs(diff_hist.GetBinContent(i)))

	plus = back.Get(key).Clone(key+"__mcregion__plus")
        plus.Add(diff_hist)
	minus = back.Get(key).Clone(key+"__mcregion__minus")
        minus.Add(diff_hist,-1)
	histos.append(plus);histos.append(minus)
         
    signal_mc_file.Close()
    background_mc_file.Close()
    back.Close()        

    out_file =TFile(output_fname,"RECREATE")
    out_file.cd()
    for item in histos:
        item.Write()
    out_file.Close()

    print 'done with uncertainty'
    print '='*10
