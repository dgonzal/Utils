#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)



from ROOT import TFile, TH1, TObject

import numpy as np
import time

# merge all the histograms to one
def create_merged_hists(input_fname, output_fname,postfix):
    TH1.AddDirectory(ROOT.kFALSE);
    print '='*10
    print 'doing the merging'
    print 'input file',input_fname
    print 'output file',output_fname
    
    in_file = TFile(input_fname)
    keys = in_file.GetListOfKeys()
    
    hist_names =[]
    histos = []
    cloned = []
    
    for key in keys:
        key = str(key.GetName())
        #print key
        category = key.split("__")[0]
        #print category
        if not category in hist_names:
            hist_names.append(category)
            histos.append(in_file.Get(key).Clone(category+'__'+postfix))
            histos[-1].Sumw2()
            cloned.append(key)

    print cloned
            
    for key in keys:
        key = str(key.GetName())
        for i, name in enumerate(hist_names):
            if name in key:
                if cloned[i] not in key:
                    print 'merging histogram',key,'to',cloned[i]
                    hist = in_file.Get(key).Clone(category+'__'+postfix)
                    #hist.Draw()
                    #time.sleep(2)
                    if type(hist) is not type(histos[i]):
                        print 'can not merge',key,'. it is not of the same type'           
                    histos[i].Add(hist)
                  

    in_file.Close()    
    out_file =TFile(output_fname,"RECREATE")
    out_file.cd()
    for item in histos:
        item.Write()
    out_file.Close()

    print 'done with merging'
    print '='*10

def add_signal(fname, process, signal_file, signal, scale):
    print scale
    signal_file = TFile(signal_file,'READ')
    work_file =TFile(fname,'UPDATE')
    work_file.cd()
    keys = work_file.GetListOfKeys()
    signal_keys = signal_file.GetListOfKeys()

    print len(keys),len(signal_keys)

    #return
    original = []
    
    for key in keys:
        key = str(key.GetName())    
        category = key.split("__")[0]
        if process not in key.split("__")[1]:continue
        for signal_key in signal_keys:
            signal_key =  str(signal_key.GetName())
            print 'comparing',key, signal_key
            sig_cat = signal_key.split("__")[0]
            if signal not in signal_key.split("__")[1]:continue
            if category == sig_cat:
                sig_hist = signal_file.Get(signal_key)
                sig_hist.Scale(scale)
                original.append(work_file.Get(key))
                originalnorm = original[-1].GetSumOfWeights()
                original[-1].Add(sig_hist)
                #original[-1].Scale(originalnorm/original[-1].GetSumOfWeights())
                print '='*10
                break
            
    for item in original:
        item.Write('',TObject.kOverwrite);
    work_file.Close()
    signal_file.Close()
                
