#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)
ROOT.TH1.AddDirectory(0)

from ROOT import TFile, TH1, TObject
from scipy import stats
import sys
import operator

def chi2_calc(filename,obs,exp):
    print 'starting to calculate chi2 for file',filename 
    work_file = TFile(filename)
    keys = work_file.GetListOfKeys()
    categories_done=[]

    chi2 =0
    bins =0
    
    for key in keys:
        key = str(key.GetName())
        category = key.split("__")[0]
        if category in categories_done:continue
        print 'working on category',category
        obs_hist=work_file.Get(category+'__'+obs)
        exp_hist=work_file.Get(category+'__'+exp)
        for i in xrange(1,obs_hist.GetNcells()-1):
            bins+=1
            exp_content = exp_hist.GetBinContent(i)
            binchi2 = (obs_hist.GetBinContent(i)-exp_content)**2
            sigma2 = obs_hist.GetBinError(i)**2+exp_hist.GetBinError(i)**2
            for unc in keys:
                unc = str(unc.GetName())
                #print unc
                if category not in unc or len(unc.split("__"))<3 or exp not in unc:continue
                #print 'uncertatinies considered',unc, (work_file.Get(unc).GetBinContent(i)-exp_content)**2
		pass
                sigma2 = sigma2+ (work_file.Get(unc).GetBinContent(i)-exp_content)**2#/4
            #exit(0)
            #print 'bin',i,'chi2',binchi2,'sigma',sigma2
            chi2= chi2 + binchi2/sigma2
        categories_done.append(category)
    print '\033[92m'
    print 'chi2',chi2,'bins',bins
    print 'Prob.(Chi2,'+str(bins-3)+')',1-stats.chi2.cdf(chi2, bins-10)
    print 'Prob.(Chi2,'+str(bins-7)+')',1-stats.chi2.cdf(chi2, bins-10)
    print 'Prob.(Chi2,'+str(bins-10)+')',1-stats.chi2.cdf(chi2, bins-10)
    print 'Prob.(Chi2,'+str(bins-20)+')',1-stats.chi2.cdf(chi2, bins-20)
    print '\033[0m'
    
def blind_final(filename,obs,exp):
    print 'starting to calculate chi2 for file',filename 
    work_file = TFile(filename,'UPDATE')
    keys = work_file.GetListOfKeys()
    categories_done=[]
    hist_list=[]
    for key in keys:
        key = str(key.GetName())
        category = key.split("__")[0]
        if category in categories_done:continue
        exp_hist=work_file.Get(category+'__'+exp).Clone(category+'__'+obs)
        hist_list.append(exp_hist)
        work_file.Delete(key+';*')
        categories_done.append(category)
    for item in hist_list:
        item.Write('',TObject.kOverwrite)
    work_file.Close()
    
def rescale_on_count(filename,obs,exp,output='',signal=''):
    print 'starting to rescale based on the sr count',filename 
    work_file = TFile(filename,'READ')
    keys = work_file.GetListOfKeys()
    categories_done=[]
    hist_list=[]
    for key in keys:
        key = str(key.GetName())
        category = key.split("__")[0]
        if signal and signal in key:
            hist_list.append(work_file.Get(key))
            continue
        if category in categories_done:continue
        obs_hist=work_file.Get(category+'__'+obs).Clone()
        exp_hist=work_file.Get(category+'__'+exp).Clone()
        scale = obs_hist.GetSumOfWeights()/exp_hist.GetSumOfWeights()
        #scale = obs_hist.GetSumOfWeights()/exp_hist.Integral()
        print 'scaling', category+'__'+exp, 'by', scale
        exp_hist.Scale(scale)
        hist_list.append(obs_hist)
        hist_list.append(exp_hist)
        for unc in keys:
            unc = str(unc.GetName())
            if category not in unc or len(unc.split("__"))<3 or obs in unc:continue
            hist_list.append(work_file.Get(unc).Clone())
            if signal and signal in unc:continue
            hist_list[-1].Scale(scale)
            #work_file.Delete(unc+';*')
            #print 'scaling',str(work_file.Get(unc).GetName()) 
        categories_done.append(category)
    work_file.Close()
    if not output: output = filename.replace('.root','_sr_rescale.root')
    result= TFile(output,'RECREATE')
    result.cd()
    for item in hist_list:
        item.Write('',TObject.kOverwrite)
    result.Close
    return output
    #exit(0)

def create_significance_files(filename,signal):
    print 'starting to create significance file from',filename
    work_file = TFile(filename,'READ')
    keys = work_file.GetListOfKeys()
    categories_done=[]
    hist_sig=[]
    hist_other=[]
    masses=[]
    for key in keys:
        key = str(key.GetName())
 	if signal in key: hist_sig.append(work_file.Get(key).Clone())
	else: hist_other.append(work_file.Get(key).Clone())

    for h in hist_sig:
        name = h.GetName()
        name = name.replace('10p','') 
        name = name.replace('20p','') 
        name = name.replace('30p','')
        name = ''.join(x for x in name if x.isdigit()) 
	masses.append(name)
        output = TFile(filename.replace('.root',name+'_'+signal+'.root'),'RECREATE')
	output.Cd()
	h.Write()
	for o in hist_other:
	    o.Write()

	output.Close()
    work_file.Close()
    return masses  

class cat_help:
    def __init__(self,name, bins):
        self.name = name
        self.bins = []
        self.bins.append(bins)
        

def inspect(filename):
    work_file = TFile(filename,'READ')
    keys = work_file.GetListOfKeys()

    hist_cat = []
    for key in keys:
        key = str(key.GetName())
        exits = -1
        for i,val in enumerate(hist_cat):
            if key.split("__")[0] in val.name:
                exits=i
        hist = work_file.Get(key)
        print key
        if exits>-1:
            hist_cat[exits].bins.append(hist.GetNcells())
            if hist_cat[exits].bins[-1] !=  hist_cat[exits].bins[0]:
                print key, 'has a different number of bins'
        else:
            hist_cat.append(cat_help(key.split("__")[0],hist.GetNcells()))
        
            
        #print key, 'n bins', hist.GetNcells()
    for cat in hist_cat:
        print cat.name,cat.bins
    work_file.Close()


class sort_h:
    def __init__(self,name,hist):
        self.name = name
        self.hist = hist
    
    
def sort_hists(filename):
    work_file = TFile(filename,'UPDATE')
    keys = work_file.GetListOfKeys()

    hist_s = []
    for key in keys:
        key = str(key.GetName())
        hist_s.append(sort_h(key,work_file.Get(key)))

    
    hist_s = sorted(hist_s,key=lambda x : x.name)

    print 'sort', '+'*10 
    for i in hist_s:
        print i.name
        i.hist.Write('',TObject.kOverwrite)
    print 'sort', '+'*10


def get_masses(filename, signal):
    work_file = TFile(filename,'READ')
    keys = work_file.GetListOfKeys()

    masses =  [] 
    for key in keys:
        key = str(key.GetName())
        if signal in key:
            m = key.split("__")[1]
            m = m.split("_")[1]
            if m not in masses: masses.append(m)

    #masses = map(int,masses)
    masses.sort(key=int)
    return masses

def sculp_root_file(filename, signal, mass, unc = False, out = ''):
    work_file = TFile(filename,'READ')
    keys = work_file.GetListOfKeys()
    hists =  []
    print 'signal', signal, 'mass', mass
    for key in keys:
        key = str(key.GetName())
        if signal in key and mass not in key: continue
        if signal in key and not unc and len(key.split('__'))>2 : continue
        hists.append(work_file.Get(key).Clone())
    if not out:
        out = filename.replace('.root','_'+mass.replace('_','')+'.root')
    output = TFile(out,'RECREATE')
    output.cd()
    for i in hists:
        i.Write()
    output.Close()
    work_file.Close()
        
    return out


def sculp_categories(filename, category=[]):
    print 'working on',filename
    work_file = TFile(filename,'READ')
    keys = work_file.GetListOfKeys()
    out=[]
    if not category:
        for key in keys:
            key = str(key.GetName())
            cat = key.split('__')[0]
            cat = cat.replace('Ele','')
            cat = cat.replace('Mu','')
            if cat not in category:
                category.append(cat)
    print category
    for cat in category:
        hists = []
        for key in keys:
            key = str(key.GetName())
            if cat not in key: continue
            hists.append(work_file.Get(key).Clone())
        out_name = filename.replace('.root','_'+cat+'.root')
        out.append(out_name)
        output = TFile(out_name,'RECREATE')
        output.cd()
        for i in hists:
            i.Write()
        output.Close()
    work_file.Close()
    return out 


    

if __name__ == '__main__':
    m_file =  sys.argv[1]
    inspect(m_file)
    sort_hists(m_file)
    inspect(m_file)
