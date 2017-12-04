#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)



from ROOT import TFile, TH1, TObject, TF1

# merge all the histograms to one
def add_signal_background_uncer(background,signal_region_mc, background_region_mc,output_fname, store_other_histos =False, debug = None):
    TH1.AddDirectory(ROOT.kFALSE);
    print '='*10
    print 'Adding the uncertainty of the signal to background region'
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
    other_histos =[]
    debug_histos=[]
    
    
    for key in keys:
        key = str(key.GetName())
        if not 'Background' in key:
            if(store_other_histos): other_histos.append(back.Get(key).Clone())
            continue
	print 'going to use',key,'and',key.replace('Background','DATA'),'to create signal/background mc uncertainty'
        histos.append(back.Get(key).Clone())
        diff_hist =  signal_mc_file.Get(key.replace('Background','DATA')).Clone(key+"diff")
        sig_norm = diff_hist.GetSumOfWeights()
	diff_hist.Scale(1/sig_norm)#Integral())
	back_mc_h = background_mc_file.Get(key).Clone()
        back_norm = back_mc_h.GetSumOfWeights()
        back_mc_h.Scale(1/back_norm)#Integral())
	#diff_hist.Divide(back_mc_h)
	diff_hist.Add(back_mc_h,-1)

        #fit_function = TF1("linear","1+[0]*x")
        #diff_hist.Fit(fit_function)
	#print fit_function.GetParameter(0)
        
	plus  = histos[-1].Clone() #back.Get(key).Clone(key+"__mcregion__plus")
	minus = histos[-1].Clone() #back.Get(key).Clone(key+"__mcregion__minus")

	binsum = 0
        for i in xrange(1,histos[-1].GetNcells()-1):
          #factor = 1.0/fit_function.Eval(histos[-1].GetXaxis().GetBinCenter(i))
	  #histos[-1].SetBinContent(i,histos[-1].GetBinContent(i)*factor)     
	  #if factor > 1: plus.SetBinContent(i,plus.GetBinContent(i)*factor*factor)
          #else:  minus.SetBinContent(i,minus.GetBinContent(i)*factor*factor)
          binsum +=diff_hist.GetBinContent(i)
          #diff_hist.SetBinContent(i, math.fabs(diff_hist.GetBinContent(i)))
          #diff_hist.SetBinContent(i, diff_hist.GetBinContent(i)*-1)
   	  

        diff_hist.Scale(histos[-1].GetSumOfWeights())

	if binsum>=0:
	   plus.Add(diff_hist)
           minus.Add(diff_hist,-1)
        else:
	   plus.Add(diff_hist,-1)
	   minus.Add(diff_hist)


	histos.append(plus);histos.append(minus)

        continue 


        #diff_hist.Add(back_mc_h,-1)
	print 'entries background',back.Get(key).GetSumOfWeights(),'signal mc',signal_mc_file.Get(key.replace('Background','DATA')).GetSumOfWeights(),'back mc',background_mc_file.Get(key).GetSumOfWeights()
        if debug:
            debug_histos.append(diff_hist.Clone())
            debug_histos.append(back_mc_h.Clone())
            #debug_histos[-1].Scale(back_norm )
            #debug_histos[-2].Scale(back_norm )
            debug_histos.append(signal_mc_file.Get(key.replace('Background','DATA')).Clone())
            debug_histos[-1].Scale(1/sig_norm)# back_norm/sig_norm)
            #debug_histos[-1].Chi2Test(debug_histos[-2],"P")

	  #print diff_hist.GetBinContent(i)
        diff_hist.Scale(histos[-1].GetSumOfWeights() )#Integral())
        
        #histos[-1].Add(diff_hist)
	plus  = histos[-1].Clone() #back.Get(key).Clone(key+"__mcregion__plus")
	minus = histos[-1].Clone() #back.Get(key).Clone(key+"__mcregion__minus")

        #plus.Add(diff_hist)
        #minus.Add(diff_hist,-1)
    
        binsum = 0.0;
        for i in xrange(1,diff_hist.GetNcells()-1):
	    binsum +=diff_hist.GetBinContent(i)
            
        if binsum >= 0:
            plus.Add(diff_hist,2)
            minus.Add(diff_hist,-2)
        else:
            minus.Add(diff_hist,2)
            plus.Add(diff_hist,-2)
                
        """
        binsum = 0.0;
        for i in xrange(1,diff_hist.GetNcells()-1):
	    binsum +=diff_hist.GetBinContent(i)
            #diff_hist.SetBinContent(i, math.fabs(diff_hist.GetBinContent(i)))
        """
        #print binsum
        #if binsum>=0:
        
        
	#diff_hist.Scale(0.0001)
        #minus.Add(diff_hist,-1)
        #else:
        #   plus.Add(diff_hist,-1)	
        #   minus.Add(diff_hist)

        #histos.append(diff_hist)
	histos.append(plus);histos.append(minus)
        if debug:
            debug_histos.append(plus.Clone());debug_histos.append(minus.Clone())


 
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
