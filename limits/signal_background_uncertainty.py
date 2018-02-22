#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

import time, array

from  injection_merge import *

from ROOT import TFile, TH1, TObject, TF1

# merge all the histograms to one
def add_signal_background_uncer(background,signal_region_mc, background_region_mc,output_fname="", store_other_histos =False, debug = None):
    TH1.AddDirectory(ROOT.kFALSE);

    if not output_fname: output_fname = background.replace('.root','_unc.root')
    correction = output_fname.replace('.root','_correction.root')
    
    print '='*10
    print 'Adding the uncertainty of the signal to background region'
    print 'background',background
    print 'signal region',signal_region_mc,'background region',background_region_mc
    print 'output file',output_fname
    print 'correction file' ,correction

    correction_file = TFile(correction,'RECREATE')

    signal_mc_file = TFile(signal_region_mc)
    background_mc_file = TFile(background_region_mc)
    back = TFile(background)

    keys = back.GetListOfKeys()

    hist_names =[]
    histos = []
    cloned = []
    other_histos =[]
    debug_histos=[]
    
    scale = False 
        
    for key in keys:
        key = str(key.GetName())
        if len(key.split('__')) >2:continue
        #print key
        if not 'Background' in key:
            if(store_other_histos): other_histos.append(back.Get(key).Clone())
            continue
	print 'going to use',key,'and',key.replace('Background','DATA'),'to create signal/background mc uncertainty'

        data_back = back.Get(key).Clone()
        data_norm = data_back.GetSumOfWeights()
        
        binning =  get_binning(data_back)

 	print binning
        binning_arr = array.array('d',binning)
	max_val = -99999999
	for i in binning_arr:
	  if i>max_val: max_val=i
	  else: print i
          
        signal_mc_h = signal_mc_file.Get(key.replace('Background','DATA')).Clone("signalregion")
        back_mc_h = background_mc_file.Get(key).Clone("background")

	signal_mc_h = signal_mc_h.Rebin(len(binning)-1,key+"_signal_region_final_binning", binning_arr)	
	back_mc_h = back_mc_h.Rebin(len(binning)-1,key+"_background_final_binning", binning_arr)

        sr_binning =binning
        #sr_binning = update(signal_mc_h,0.05)
        #print sr_binning     

        #signal_mc_h = signal_mc_h.Rebin(len(sr_binning)-1,signal_mc_h.GetName(),array.array('d',sr_binning))
	#back_mc_h = back_mc_h.Rebin(len(sr_binning)-1,back_mc_h.GetName(),array.array('d',sr_binning))

        diff_hist  =  back_mc_h.Clone(key+"diff")
        #diff_hist  =  diff_hist.Rebin(len(sr_binning)-1,diff_hist.GetName(),array.array('d',sr_binning))
        ratio_hist =  signal_mc_h.Clone(key+"ratio")

        sig_norm = signal_mc_h.GetSumOfWeights()
        back_norm = back_mc_h.GetSumOfWeights()    
   
	#exit(0)
    
        signal_mc_h.Scale(1/sig_norm)
        diff_hist.Scale(1/back_norm)
        ratio_hist.Scale(1/sig_norm)
        ratio_hist.Divide(diff_hist)
        diff_hist.Add(signal_mc_h,-1)
        sr_stat_unc = signal_mc_h.Clone(key+"_sr_stat")
        cr_stat_unc = back_mc_h.Clone(key+"_cr_stat")
        #diff_hist.Scale(data_norm/back_norm)
        correction_file.cd()
        correction_hist = diff_hist.Clone()

        correction_hist.Write()
        back_mc_h.Scale(1/back_norm)
        back_mc_h.Write()
        back_mc_h.Scale(back_norm)
        signal_mc_h.Write()
        ratio_hist.Write()
        sr_stat_unc.Write()
        cr_stat_unc.Write()

        diff_hist.Scale(data_norm)
        data_back.Add(diff_hist,-1)
	#plus  = data_back.Clone(key+"__mcR_"+key.split("__")[0]+"__plus") #back.Get(key).Clone(key+"__mcregion__plus")
	#minus = data_back.Clone(key+"__mcR_"+key.split("__")[0]+"__minus") #back.Get(key).Clone(key+"__mcregion__minus")
        name = "__mcR_"+key.split("__")[0]
	name = name.replace('Mu','')
	name = name.replace('Ele','')

	plus  = data_back.Clone(key+name+"__plus") #back.Get(key).Clone(key+"__mcregion__plus")
	minus = data_back.Clone(key+name+"__minus") #back.Get(key).Clone(key+"__mcregion__minus")
        #plus  = data_back.Clone(key+"__mcR__plus") #back.Get(key).Clone(key+"__mcregion__plus")
	#minus = data_back.Clone(key+"__mcR__minus") #back.Get(key).Clone(key+"__mcregion__minus")
        
        sr_error = signal_mc_h.Clone(key+'_sr_error')
        cr_error = back_mc_h.Rebin(len(sr_binning)-1,key+'_cr_error',array.array('d',sr_binning)) 
        for i in xrange(1,sr_error.GetNcells()-1):
            sr_error.SetBinContent(i,sr_error.GetBinContent(i)-sr_error.GetBinError(i))
            cr_error.SetBinContent(i,cr_error.GetBinContent(i)+cr_error.GetBinError(i))

        cr_error.Scale(1/back_norm)
        #sr_error.Scale(1/sig_norm)

        sr_error.Divide(cr_error)
        #sr_error.Add(ratio_hist,-1)
        #sr_error.Scale(back_norm)
        sr_error.Write()

        
        
        back_mc_h_corrected = background_mc_file.Get(key).Clone(key+"_back_corrected")
        signal_mc_h_original = signal_mc_file.Get(key.replace('Background','DATA')).Clone(key+"signalregion")
        
        for i in xrange(1,data_back.GetNcells()-1):
            if back_mc_h_corrected.GetBinError(i)<1: back_mc_h_corrected.SetBinError(i,1)
            ratio_bin   = ratio_hist.GetBinContent(ratio_hist.FindBin(back_mc_h_corrected.GetBinCenter(i)))
            ratio_error = math.fabs(ratio_hist.GetBinContent(ratio_hist.FindBin(back_mc_h_corrected.GetBinCenter(i)))-sr_error.GetBinContent(ratio_hist.FindBin(back_mc_h_corrected.GetBinCenter(i))))
            print 'bin center',data_back.GetBinCenter(i), 'bin ratio',ratio_bin,'+-',ratio_error,'back',back_mc_h_corrected.GetBinContent(i),'+-',back_mc_h_corrected.GetBinError(i)
            
	    #data_back.SetBinContent(i,data_back.GetBinContent(i)*ratio_bin)
            back_mc_h_corrected.SetBinError(i,math.sqrt((back_mc_h_corrected.GetBinError(i)*ratio_bin)**2+ (back_mc_h_corrected.GetBinContent(i)*(1-ratio_bin)*ratio_error)**2))
            #back_mc_h_corrected.SetBinError(i,back_mc_h_corrected.GetBinError(i)*(ratio_bin+ratio_error))
	    back_mc_h_corrected.SetBinContent(i,back_mc_h_corrected.GetBinContent(i)*ratio_bin)
	    #data_back.SetBinError(i,data_back.GetBinError(i)*ratio_bin)
            #plus.SetBinContent(i,data_back.GetBinContent(i)*ratio_bin*ratio_bin)



        for i in xrange(1,data_back.GetNcells()-1):
            diff_hist.SetBinContent(i,math.fabs(diff_hist.GetBinContent(i)))
            
        plus.Add(diff_hist,-1)
        minus.Add(diff_hist,1)
        
        #plus.Add(ratio_hist,1)
        #minus.Add(ratio_hist,-1)
        
        #plus.Add(scale_up,1)
        #minus.Add(scale_down,-1)

        back_mc_h_corrected.Scale(1/back_mc_h_corrected.GetSumOfWeights())
        signal_mc_h_original.Scale(1/sig_norm)
        back_mc_h_corrected.Write()
        signal_mc_h_original.Write()
        
        
        #Test the method
        #back_mc_h_corrected = back_mc_h.Clone("test_diff")
        #diff_hist.Scale(back_norm/data_norm)
        #back_mc_h_corrected.Add(diff_hist,-1)
        #back_mc_h_corrected.Scale(sig_norm/back_norm)
        print 'Chi2 mc fit probability',  signal_mc_h_original.Chi2Test(back_mc_h_corrected,'WW')
        
        
        
        #exit(0) 
        # method with shape uncertainties
        histos.append(data_back);#histos.append(diff_hist)
        histos.append(plus);histos.append(minus);
 
    out_file =TFile(output_fname,"RECREATE")
    out_file.cd()
    for item in histos:
        item.Write()

    if(store_other_histos):
        for item in other_histos:
            item.Write()
    out_file.Close()
    signal_mc_file.Close()
    background_mc_file.Close()
    back.Close()        
    debug_file = None
    if debug:
        debug_file = TFile(debug,'recreate')
        debug_file.cd()
        for item in debug_histos:
            item.Write()

        
    print 'done with uncertainty'
    print '='*10
    return output_fname
