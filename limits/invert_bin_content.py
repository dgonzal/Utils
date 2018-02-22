#! /usr/bin/env python

ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

from ROOT import TFile, TH1, TObject
execfile('injection_merge.py')


def invert_bin_content(in_file,process="", out_file="", factor=-1.):
    if not out_file:
        out_file = in_file.replace(".root","_inverted.root")
    work_file =TFile(in_file,'READ')
    keys = work_file.GetListOfKeys()
    hist_list=[]
    for key in keys:
        key = str(key.GetName())
        if len(key.split('__'))==2:
            hist = work_file.Get(key).Clone(key.split('__')[0]+'__'+process)
        else:
            sign =''
            if 'minus' in key.split('__')[3]:sign ='plus'
            else: sign='minus'
            hist = work_file.Get(key).Clone(key.split('__')[0]+'__'+process+'__'+key.split('__')[2]+'__'+sign)
            #hist = work_file.Get(key).Clone(key.split('__')[0]+'__'+process+'__'+key.split('__')[2]+'__'+key.split('__')[3])

        for i in xrange(hist.GetNcells()):
            hist.SetBinContent(i,factor*hist.GetBinContent(i))
        hist_list.append(hist)
    print out_file
    output =TFile(out_file,'RECREATE')
    for item in hist_list:
        item.Write('',TObject.kOverwrite);
    output.Close()
    work_file.Close()

    return out_file



def add_substraction_unc(filename,signal_file,signal,values):
    work_file =TFile(filename,'UPDATE')
    signal_file =TFile(signal_file,'READ')
    signal_cross = values[0]
    uncer = values[1]
    hist_list =[]

    keys = work_file.GetListOfKeys()
    for key in keys:
        key = str(key.GetName())
        if len(key.split('__'))>2:continue
        if not 'Background' in key:continue
        category = key.split('__')[0]
        hist_up   =  work_file.Get(key).Clone(key+'__signalSub__plus')
        hist_down =  work_file.Get(key).Clone(key+'__signalSub__minus')
        signal_hist = signal_file.Get(category+'__'+signal)
        binning = get_binning(hist_up)
        if binning:
	    signal_hist = signal_hist.Rebin(len(binning)-1,key,array.array('d',binning))

        signal_hist.Scale(uncer)
        hist_up.Add(signal_hist)
        hist_down.Add(signal_hist,-1)
        hist_list.append(hist_up); hist_list.append(hist_down)
    signal_file.Close()
    work_file.cd()
    for item in hist_list:
        item.Write('',TObject.kOverwrite);
    work_file.Close()
    print 'added signal substraction uncertainty to', filename

        
def rename_process(in_file,old, new, out_file=""):
    if not out_file:
        out_file = in_file.replace(".root","_renamed.root")
    work_file =TFile(in_file,'READ')
    keys = work_file.GetListOfKeys()
    hist_list=[]
    print 'replace',old,'with',new
    for key in keys:
        key = str(key.GetName())
        new_key= key.replace(old,new)
        #print key, 'replaced by', new_key
        hist = work_file.Get(key).Clone(new_key)
    
        hist_list.append(hist)
    
    output =TFile(out_file,'RECREATE')
    for item in hist_list:
        item.Write('',TObject.kOverwrite);
    output.Close()
    work_file.Close()
    return out_file
