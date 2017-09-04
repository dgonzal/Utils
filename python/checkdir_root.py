#! /usr/bin/env python

import sys
sys.argv.append('-b')
import glob
import math, warnings

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

from ROOT import TCanvas, TFile, TH1, THStack, TLegend
import matplotlib.pyplot as plt

import array

def filetreecheck(filename,tree_name,draw_command): 
    warnings.simplefilter("ignore")
    file = TFile(filename)
    tree = file.Get(tree_name)
    tree.Draw(draw_command)
    return True


if __name__=='__main__':
    file_pattern = '*QCD*'
    path = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/jec_up_MuSelUNC/'
    draw_command = 'jet_jec_up.jet_jec_up.m_pt'

    search_dir = path+'/'+file_pattern
    print 'Target dir',
    for item in glob.glob(search_dir):
        print 'checking', item
        filetreecheck(item,'AnalysisTree',draw_command)
    






  


#compute('limits/ROOTDataShape/Bay_BprimeT_RH_rebinned.root')
#binFile(0.3, 'RecoBp.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])


