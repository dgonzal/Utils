#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

from ROOT import TFile

def graphread(filename, graphname, variable,output):
    work_file = TFile(filename)
    graph = work_file.Get(graphname)
    graph.Print()
    
    
    xvals = graph.GetX()
    xerr_up = graph.GetEXhigh()
    xerr_down = graph.GetEXlow()
    yvals = graph.GetY()
    yerr_up = graph.GetEYhigh()
    yerr_down = graph.GetEYlow()    

    factor ='('
    factor_up ='('
    factor_down = '('
    for i in xrange(graph.GetN()):
        low_x  = xvals[i]-xerr_down[i]
        high_x = xvals[i]+xerr_up[i]
        if i==0:
           factor += '(('+variable+' >= '+str(low_x) +' && '+variable+' <= '+str(high_x) +')*'+ str(yvals[i])+')+'
           factor_up   += '(('+variable+' >= '+str(low_x) +' && '+variable+' <= '+str(high_x) +')*'+ str(yvals[i]+yerr_up[i])+')+'
           factor_down += '(('+variable+' >= '+str(low_x) +' && '+variable+' <= '+str(high_x) +')*'+ str(yvals[i]+yerr_down[i])+')+'
        else:
           factor += '(('+variable+' > '+str(low_x) +' && '+variable+' <= '+str(high_x) +')*'+ str(yvals[i])+')+'
           factor_up   += '(('+variable+' > '+str(low_x) +' && '+variable+' <= '+str(high_x) +')*'+ str(yvals[i]+yerr_up[i])+')+'
           factor_down += '(('+variable+' > '+str(low_x) +' && '+variable+' <= '+str(high_x) +')*'+ str(yvals[i]+yerr_down[i])+')+'
    factor = factor[:-2]+')'
    factor_up = factor[:-2]+')'
    factor_down = factor[:-2]+')'
    print factor
    print factor_down
    print factor_up

    result = open(output,'w+')
    result.write('#pragma once\n#include <string>\n')
    result.write('\n\n\n\n')
    result.write('std::string muon_trk_factors(int i=0){\n')
    result.write('\tif(i==0)\n\t\treturn \"'+factor+'\";\n')
    result.write('\tif(i==1)\n\t\treturn \"'+factor_up+'\";\n')
    result.write('\tif(i==-1)\n\t\treturn \"'+factor_down+'\";\n')
    result.write('\treturn \"something went wrong with the muon tracking factors pls check!\";\n')
    result.write('}')
    




    

if __name__ == '__main__':
    graphread('/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/common/data/Tracking_EfficienciesAndSF_BCDEFGH.root','eff_aeta_tk0_dr030e030_corr','fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta)','test.h')



    
