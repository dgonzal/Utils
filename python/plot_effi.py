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

def compute(filename): 
    warnings.simplefilter("ignore")
    file = TFile(filename)
    keys = file.GetListOfKeys()

    print 'input file:',filename
    tuple_store = None

    # load all the background and data histograms
    for key in keys:
        key =  key.GetName()
        if 'Background' in key or 'plus' in key or 'minus' in key or 'DATA' in key:
          continue
       
        signalname = key.split('__')[1]
        histogram = file.Get(key)
        entries = float(histogram.GetEntries())
        found_sig = False
        if tuple_store:
          for item in tuple_store:
            if item[0] == signalname:
              item[1] += entries
              found_sig = True
              break
        if not found_sig:
          if not tuple_store: tuple_store=[]
          tuple_store.append([signalname,entries])
 
    for item in tuple_store:
      print item[0],item[1]
      item.append(item[0])
      item[0] = int((item[0].split('-')[1]).split('_')[0])
    tuple_store.sort(key=lambda x: x[0])
    return  tuple_store



execfile("lumi_signal.py")
for res_file in glob.glob("../limits/ROOTDataShape/*LH_rebinned.root"):
    if 'Mu' in res_file or 'Ele' in res_file:
        continue
    print res_file
    result = compute(res_file)
    x_range =[]
    y_range =[]
    legend = (result[0][2].split('-'))[0]+' '+(result[0][2].split('_'))[1]
    if len((result[0][2].split('_'))[1])>2: legend += ' '+(result[0][2].split('_'))[2]
    
    for item in result:
        divisor = 1
        for lumi in lumi_list:
            if lumi[0].replace('53','')==item[2]:
                divisor = lumi[1]/100
                print lumi
                break
        
        x_range.append(item[0])
        y_range.append(item[1]/divisor)

    legend = legend.replace('BprimeB','B+b')
    legend = legend.replace('BprimeT','B+t')
    legend = legend.replace('X','$X_{5/3}$')
    legend = legend.replace('Width-','')
    legend = legend.replace('0p','0%')
    print legend
    plt.plot(x_range, y_range,label=str(legend))

plt.legend(loc='lower right',frameon=False)
plt.xlabel('VLQ Mass')
plt.ylabel('$\epsilon_{eff}$')
plt.title('CMS $\it{Preliminary}$', loc='left')
plt.title('Signal efficiency', loc='right')

plt.savefig("test.pdf")
#plt.show()
  


#compute('limits/ROOTDataShape/Bay_BprimeT_RH_rebinned.root')
#binFile(0.3, 'RecoBp.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])


