#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

import time, array

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
        #print key
        if not 'Background' in key:
            if(store_other_histos): other_histos.append(back.Get(key).Clone())
            continue
	print 'going to use',key,'and',key.replace('Background','DATA'),'to create signal/background mc uncertainty'
        histos.append(back.Get(key).Clone())

        binning=[]
        for i in xrange(1,histos[-1].GetNcells()-1):
            binning.append(histos[-1].GetBinLowEdge(i))
            if i+1==histos[-1].GetNcells()-1: binning.append(histos[-1].GetBinLowEdge(i)+histos[-1].GetBinWidth(i))

 	print binning
        binning_arr = array.array('d',binning)
	max_val = -99999999

	for i in binning_arr:
	  if i>max_val: max_val=i
	  else: print i

        histos[-1] = histos[-1].Rebin(len(binning)-1,key,binning_arr)           
        signal_region = signal_mc_file.Get(key.replace('Background','DATA')).Clone("signalregion")
        back_mc_h = background_mc_file.Get(key).Clone("background")

	signal_region = signal_region.Rebin(len(binning)-1,"signal_region_final_binning", binning_arr)	
	back_mc_h = back_mc_h.Rebin(len(binning)-1,"background_final_binning", binning_arr)

        diff_hist =  signal_region.Clone(key+"diff")

        #signal_region = signal_mc_file.Get(key.replace('Background','DATA')).Rebin(len(binning)-1,"signalregion",binning_arr)
        #diff_hist =  signal_mc_file.Get(key.replace('Background','DATA')).Rebin(len(binning)-1,key+"diff",binning_arr)
        #back_mc_h = background_mc_file.Get(key).Rebin(len(binning)-1,"",binning_arr)
        
        sig_norm = diff_hist.GetSumOfWeights()
        back_norm = back_mc_h.GetSumOfWeights()    
        
	diff_hist.Scale(1/sig_norm)
        back_mc_h.Scale(1/back_norm)

        #print key,diff_hist.Chi2TestX(back_mc_h,"WW")
        """	
        if diff_hist.Chi2Test(back_mc_h)<0.1:            
            diff_hist.DrawNormalized()
            back_mc_h.DrawNormalized("same")
            for i in xrange(diff_hist.GetNcells()):
	       print 'bin', i, 'pull', (diff_hist.GetBinContent(i)-back_mc_h.GetBinContent(i))/math.sqrt(diff_hist.GetBinContent(i)+back_mc_h.GetBinContent(i))
            #time.sleep(5)
        """
        
        #diff_hist.Divide(back_mc_h)
        diff_hist.Add(back_mc_h,-1)       
        
        #diff_hist.Add(back_mc_h,-1)
        #diff_hist.Scale(-1)
        diff_minus = diff_hist.Clone("diff_minus")
        diff_plus  = diff_hist.Clone("diff_plus")

        histo_up = histos[-1].Clone(key+"__mcSup__plus")
	histo_down = histos[-1].Clone(key+"__mcSup__minus")

	
        statD_up = histos[-1].Clone(key+"__mcSdown__plus")
	statD_down = histos[-1].Clone(key+"__mcSdown__minus")

        #diff_hist.Scale(back_norm/sig_norm)
        diff_hist.Scale(histos[-1].GetSumOfWeights())
       
	histos[-1].Add(diff_hist,1)

 
        bin_sum=0
        for i in xrange(diff_hist.GetNcells()):
	    #histos[-1].SetBinError(i, 100*math.sqrt(math.fabs(histos[-1].GetBinError(i)) + diff_hist.GetBinError(i)))
            #bin_sum+=diff_hist.GetBinContent(i)
	    #diff_hist.SetBinError(i,math.sqrt(math.fabs(diff_hist.GetBinContent(i))))
	    diff_minus.SetBinContent(i,diff_minus.GetBinContent(i)+diff_minus.GetBinError(i))   
	    diff_plus.SetBinContent(i,diff_plus.GetBinContent(i)-diff_plus.GetBinError(i))   
            #diff_hist.SetBinContent(i,math.fabs(diff_hist.GetBinContent(i)))
            #print 'bin',i,'content',histos[-1].GetBinContent(i),'+-',histos[-1].GetBinError(i)
            #back_mc_h.SetBinError(i,math.sqrt(back_mc_h.GetBinError(i)*back_mc_h.GetBinError(i)+diff_hist.GetBinContent(i)*diff_hist.GetBinContent(i)))
            #if histos[-1].GetBinContent(i)<0:print 'bin',i,'smaler 0 content',histos[-1].GetBinContent(i)
            #histos[-1].SetBinError(i,math.sqrt(math.fabs(histos[-1].GetBinContent(i))))
            #histos[-1].SetBinError(i,math.sqrt(histos[-1].GetBinError(i)*histos[-1].GetBinError(i)+diff_hist.GetBinContent(i)*diff_hist.GetBinContent(i)))
            #histos[-1].SetBinContent(i,histos[-1].GetBinError(i)+diff_hist.GetBinContent(i)*diff_hist.GetBinContent(i)))
            #plus.SetBinContent (i,histos[-1].GetBinContent(i)+math.sqrt(histos[-1].GetBinError(i)*histos[-1].GetBinError(i)+diff_hist.GetBinContent(i)*diff_hist.GetBinContent(i)))
            #minus.SetBinContent(i,histos[-1].GetBinContent(i)-math.sqrt(histos[-1].GetBinError(i)*histos[-1].GetBinError(i)+diff_hist.GetBinContent(i)*diff_hist.GetBinContent(i)))

	plus  = histos[-1].Clone(key+"__mcR_"+key.split("__")[0]+"__plus") #back.Get(key).Clone(key+"__mcregion__plus")
	minus = histos[-1].Clone(key+"__mcR_"+key.split("__")[0]+"__minus") #back.Get(key).Clone(key+"__mcregion__minus")

            
        if bin_sum >=0:
            #histos[-1].Add(diff_hist)
            plus.Add(diff_hist,1)
            minus.Add(diff_hist,-1)
	    histo_up.Add(diff_plus)
	    histo_down.Add(diff_plus,-1) 
	    statD_up.Add(diff_minus)
	    statD_down.Add(diff_minus,-1)
        else:
            #histos[-1].Add(diff_hist,-1)
            plus.Add(diff_hist,-1)
            minus.Add(diff_hist,1) 
	    histo_up.Add(diff_plus,-1)
	    histo_down.Add(diff_plus)       
            statD_up.Add(diff_minus,-1)
	    statD_down.Add(diff_minus)

        back_mc_h.Scale(back_norm)
        diff_hist.Scale(back_norm/histos[-1].GetSumOfWeights())
        back_mc_h.Add(diff_hist)
        print 'Chi2 hist fit probability', signal_region.Chi2Test(back_mc_h,'WW')
        #print 'Chi2 hist fit prob ', signal_region.Chi2Test(histos[-1],'WW')
        
        histos.append(plus);histos.append(minus)
 	#histos.append(histo_up);histos.append(histo_down)    
	#histos.append(statD_up);histos.append(statD_down)   
        continue
        

	#diff_hist.Divide(back_mc_h)
	diff_hist.Add(back_mc_h,-1)

        #fit_function = TF1("linear","1+[0]*x")
        #diff_hist.Fit(fit_function)
	#print fit_function.GetParameter(0)
        
	plus  = histos[-1].Clone(key+"__mcregion__plus") #back.Get(key).Clone(key+"__mcregion__plus")
	minus = histos[-1].Clone(key+"__mcregion__minus") #back.Get(key).Clone(key+"__mcregion__minus")

	binsum = 0
        for i in xrange(1,histos[-1].GetNcells()-1):
          #factor = 1.0/fit_function.Eval(histos[-1].GetXaxis().GetBinCenter(i))
	  #histos[-1].SetBinContent(i,histos[-1].GetBinContent(i)*factor)     
	  #if factor > 1: plus.SetBinContent(i,plus.GetBinContent(i)*factor*factor)
          #else:  minus.SetBinContent(i,minus.GetBinContent(i)*factor*factor)
          binsum +=diff_hist.GetBinContent(i)
          #diff_hist.SetBinContent(i, math.fabs(diff_hist.GetBinContent(i)))
          diff_hist.SetBinContent(i, diff_hist.GetBinContent(i))
   	  

        diff_hist.Scale(histos[-1].GetSumOfWeights())

	if binsum>=0:
	   plus.Add(diff_hist,1)
           minus.Add(diff_hist,-1)
        else:
	   plus.Add(diff_hist,-1)
	   minus.Add(diff_hist,1)


	histos.append(plus);histos.append(minus)



 
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
