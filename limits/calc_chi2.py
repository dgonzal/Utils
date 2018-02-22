#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

from ROOT import TFile, TH1, TObject
from scipy import stats

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
            sigma = obs_hist.GetBinError(i)**2+exp_hist.GetBinError(i)**2
            for unc in keys:
                unc = str(unc.GetName())
                #print unc
                if category not in unc or len(unc.split("__"))<3 or exp not in unc:continue
                #print 'uncertatinies considered',unc
                sigma =sigma+ (work_file.Get(unc).GetBinContent(i)-exp_content)**2
            #exit(0)
            #print 'bin',i,binchi2,sigma
            chi2= chi2 + binchi2/sigma
        categories_done.append(category)
    print '\033[92m'
    print 'chi2',chi2,'bins',bins
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
    
