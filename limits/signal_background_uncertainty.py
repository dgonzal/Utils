#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

import time, array
import numpy
from scipy.optimize import minimize

from  injection_merge import *

from ROOT import TFile, TH1, TObject, TF1, TLegend

def chi2(a, back, sig):
    sig.Scale(a)
    chi2 = 0
    for i in xrange(1, back.GetNcells()-1):
        chi2 += (back.GetBinContent(i)-sig.GetBinContent(i))**2/ (sig.GetBinError(i)**2+back.GetBinError(i)**2)
    sig.Scale(1/a)
    #print chi2, a 
    return chi2

def get_scale(back,sig,guess=1, x=0):
    x0 = numpy.array([guess])
    fit_back = back.Clone()
    fit_sig = sig.Clone()

    for i in xrange(fit_back.GetXaxis().FindBin(x)):
        fit_back.SetBinContent(i,0)
        fit_back.SetBinError(i,1000000)
        fit_sig.SetBinContent(i,0)
        fit_sig.SetBinError(i,10000000)
    """
    for i in xrange(fit_back.GetMaximumBin()+4):
        #print 'setting bin',i,fit_back.GetBinContent(i),'-> 0'
        fit_back.SetBinContent(i,0)
        fit_back.SetBinError(i,1000000)
    for i in xrange(fit_sig.GetMaximumBin()+4):
        fit_sig.SetBinContent(i,0)
        fit_sig.SetBinError(i,10000000)
    """
    return minimize(chi2,x0,args=(fit_back,fit_sig), method = 'Powell').x #Nelder-Mead')
    #return 


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

        category = key.split('__')[0]
        
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
        print len(get_binning(signal_mc_h))
        print get_binning(signal_mc_h)
        #exit(0)

        
        #signal_mc_h = signal_mc_h.Rebin(3)
        #back_mc_h = back_mc_h.Rebin(3)
        #exit(0)
        new_sig_binning = update(signal_mc_h, 0.05)
        print new_sig_binning
	signal_mc_h = signal_mc_h.Rebin(len(new_sig_binning)-1,category+"__SR", array.array('d',new_sig_binning))	
	back_mc_h = back_mc_h.Rebin(len(new_sig_binning)-1,category+"__CR", array.array('d',new_sig_binning))	
        new_sig_binning = update(back_mc_h, 0.05)
        signal_mc_h = signal_mc_h.Rebin(len(new_sig_binning)-1,category+"__SR", array.array('d',new_sig_binning))	
	back_mc_h = back_mc_h.Rebin(len(new_sig_binning)-1,category+"__CR", array.array('d',new_sig_binning))
        print len(get_binning(signal_mc_h))
        print get_binning(signal_mc_h)
        #exit(0)


        
        diff_hist  =  back_mc_h.Clone(key+"diff")
        
        sig_norm = signal_mc_h.GetSumOfWeights()
        back_norm = back_mc_h.GetSumOfWeights()    

         
        #sig_fit_scale = get_scale(back_mc_h,signal_mc_h, back_norm/sig_norm)
        #print sig_fit_scale
	#exit(0)
        #signal_mc_h.Scale(sig_fit_scale)
        #signal_mc_h.Scale(1/sig_norm)
        #back_mc_h.Scale(1/back_norm)
        diff_hist.Scale(1/back_norm)
        sig_fit_scale = get_scale(back_mc_h,signal_mc_h, back_norm/sig_norm,700)
        signal_mc_h.Scale(sig_fit_scale)
        diff_hist.Add(signal_mc_h,-1)
        diff_hist.Write()
        correction_file.cd()

        
        signal_mc_h.Write()
        back_mc_h.Write()

        correction_hist = signal_mc_h.Clone(category+'__corrections')
        for mc_bin in xrange(1,signal_mc_h.GetNcells()-1):
            b = back_mc_h.GetBinContent(mc_bin)
            b_err = back_mc_h.GetBinError(mc_bin)
            a = signal_mc_h.GetBinContent(mc_bin)
            a_err = signal_mc_h.GetBinError(mc_bin)
            factor = a/b
            error = ((1/b*a_err)**2 + (a/(b*b) * b_err)**2)
            correction_hist.SetBinContent(mc_bin,factor)
            correction_hist.SetBinError(mc_bin,error)
            
        correction_hist.Write()

        draw_error_hist(correction_hist,output_fname.split('/')[0]+'/'+key+'_correction.eps',key)
        
        diff_hist.Add(signal_mc_h,-1)
        diff_hist.Scale(data_norm/back_norm)

        #data_back.Add(diff_hist,-1)        
        name = "__mcR_"+key.split("__")[0]
    
	name = name.replace('Mu','')
	name = name.replace('Ele','')

	plus  = data_back.Clone(key+name+"__plus") #back.Get(key).Clone(key+"__mcregion__plus")
	minus = data_back.Clone(key+name+"__minus") #back.Get(key).Clone(key+"__mcregion__minus")

        #plus.Add(diff_hist,-1)
        #minus.Add(diff_hist,1)

        
        for i in xrange(1,data_back.GetNcells()-1):
            content = data_back.GetBinContent(i)
            error   = data_back.GetBinError(i)**2
            bin_center = data_back.GetBinCenter(i)
            mc_bin = signal_mc_h.GetXaxis().FindBin(bin_center)
        
            b = back_mc_h.GetBinContent(mc_bin)
            a = signal_mc_h.GetBinContent(mc_bin)
            b_err = back_mc_h.GetBinError(mc_bin)
            a_err = signal_mc_h.GetBinError(mc_bin)

            print 'a',a,'+-',a_err,'b',b,'+-',b_err
            factor = a/b
            bin_err = (1/b*a_err)**2 + (a/(b*b)*b_err)**2
            #factor = 1/factor
            print 'bin',i,'center', bin_center,'content', content, 'new content', content*factor, 'correction factor', factor,'+-', math.sqrt(bin_err)
            print 'stat  unc',math.sqrt(error)        
            error = error + content**2*bin_err
            print 'stat + sys',math.sqrt(error)

            
            data_back.SetBinContent(i,content*factor)
            data_back.SetBinError(i,math.sqrt(error))
            plus.SetBinContent(i,content*factor*factor)
            #minus.SetBinContent(i,content/factor)
        #exit(0)
        #plus  = data_back.Clone(key+"__mcR__plus") #back.Get(key).Clone(key+"__mcregion__plus")
	#minus = data_back.Clone(key+"__mcR__minus") #back.Get(key).Clone(key+"__mcregion__minus")

        """
        back_mc_h_corrected = background_mc_file.Get(key).Clone(key+"_back_corrected")
        back_mc_h_corrected.Add(diff_hist,-1)
        signal_mc_h_original = signal_mc_file.Get(key.replace('Background','DATA')).Clone(key+"signalregion")
        print 'Chi2 mc fit probability',  signal_mc_h_original.Chi2Test(back_mc_h_corrected,'WW')
        """
        
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


def draw_error_hist(histogram,hist_file, text):
    leg_text= text.replace('_',' ')
    leg_text= leg_text.replace('Chi2','')
    leg_text= leg_text.replace('Background','')
    leg_text= leg_text.replace('Mu',' #mu channel ')
    leg_text= leg_text.replace('Ele',' e channel ')

    leg_text= leg_text.replace('BT','b-t')
    leg_text= leg_text.replace('WT','W-t')
    leg_text= leg_text.replace('TopT','t-t')
    leg_text= leg_text.replace('Anti','0 ')
    
    can = ROOT.gROOT.MakeDefCanvas();
    root_style()
    histogram.SetAxisRange(0.49, 1.79,"Y");
    histogram.GetXaxis().SetTitle("m_{reco} [GeV]");
    histogram.GetYaxis().SetTitle("SR/CR ratio");
    leg = TLegend(0.5,0.7,0.8,0.8)
    leg.SetFillColor(0);
    leg.SetLineColor(1);
    leg.SetBorderSize(0);
    leg.SetTextFont(42);
    leg.SetTextSize(.05);
    leg.SetFillStyle(0);

    leg.AddEntry(histogram,leg_text)
    histogram.Draw()
    leg.Draw()
    can.Print(hist_file)
    
def root_style():
    
    ROOT.gROOT.SetStyle("Plain");
    ROOT.gStyle.SetOptStat(0); 
    ROOT.gStyle.SetPadTickX(1); 
    ROOT.gStyle.SetPadTickY(1); 
    ROOT.gStyle.SetPadBorderMode(0); 
    #ROOT.gStyle.SetPadColor(kWhite); 
    ROOT.gStyle.SetPadGridX(False); 
    ROOT.gStyle.SetPadGridY(False); 
    ROOT.gStyle.SetGridColor(0); 
    ROOT.gStyle.SetGridStyle(3); 
    ROOT.gStyle.SetGridWidth(1); 
    ROOT.gStyle.SetFrameBorderMode(0); 
    ROOT.gStyle.SetFrameBorderSize(1); 
    ROOT.gStyle.SetFrameFillColor(0); 
    ROOT.gStyle.SetFrameFillStyle(0); 
    ROOT.gStyle.SetFrameLineColor(1); 
    ROOT.gStyle.SetFrameLineStyle(1); 
    ROOT.gStyle.SetFrameLineWidth(1); 
    ROOT.gStyle.SetTitleStyle(0);  
    ROOT.gStyle.SetTitleFont(42, "XYZ"); 
    ROOT.gStyle.SetTitleBorderSize(0); 
    ROOT.gStyle.SetLabelFont(42, "XYZ"); 
    ROOT.gStyle.SetAxisColor(1, "XYZ"); 
    #ROOT.gStyle.SetStripDecimals(kTRUE); 
    ROOT.gStyle.SetTickLength(0.03, "XYZ"); 
    ROOT.gStyle.SetNdivisions(510, "XYZ"); 
    ROOT.gStyle.UseCurrentStyle(); 
