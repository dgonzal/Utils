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

import matplotlib
matplotlib.use('PDF')
import matplotlib.pyplot as plt


import array

class signal_store:
   def __init__(self,entries,name,hist):
	self.entries = entries
	self.mass = int(name.split('_')[1])
	self.name = name
	self.hist = hist

   def __repr__(self):
	return self.name+'_'+str(self.entries)

   def Add(self,entries,hist):
	self.entries += entries
	#self.hist.Add(hist)


def compute(filename): 
    warnings.simplefilter("ignore")
    file = TFile(filename)
    keys = file.GetListOfKeys()

    print 'input file:',filename
    tuple_store = []

    for key in keys:
        key =  key.GetName()
        if 'Background' in key or 'plus' in key or 'minus' in key or 'DATA' in key:
          continue
        signalname = key.split('__')[1]
        histogram = file.Get(key)
        #entries = float(histogram.GetSumOfWeights()) 
        entries = float(histogram.GetEntries())
        found_sig = False
        if tuple_store:
          for item in tuple_store:
            if item.name == signalname:
		#print 'add',key,entries, 'to', item.name, 'sum',item.entries+entries
		item.Add(entries,histogram)
                found_sig = True
                break
        if not found_sig:
	  #print 'creating',signalname,entries,'with', key
          tuple_store.append(signal_store(entries,signalname,histogram))
    #print tuple_store 
    tuple_store.sort(key=lambda x: x.mass)
    return  tuple_store

"""
def plot(signal,out):
	mass = []
	for sig in signal:
		if sig.mass not in mass: mass.append(sig.mass)
	for i in mass:
		for sig in signal:
			if i == sig.mass:
"""
		

execfile("lumi_signal.py")
plot_handles =[]


input_path = r'../limits/ROOTDataShape_50_0_3500_eta4/*BprimeT*_rebinned.root' 
result_path =  r'Bt.pdf'

for res_file in sorted(glob.iglob(input_path),key=lambda x: x[::-1]):
    if 'Mu' in res_file or 'Ele' in res_file:
        continue
    print res_file
    result = compute(res_file)
    x_range =[]
    y_range =[]
    print result
    #continue
    legend = result[0].name.replace('_'+str(result[0].mass),'') # (result[0][2].split('_'))[0]+' '+(result[0][2].split('_'))[-1]

    #if len((result[0][2].split('_'))[1])>3: legend += ' '+(result[0][2].split('_'))[3]
    #continue
    #print lumi_list 
    for item in result:
	#print item[2] 
        divisor = 1
        for lumi in lumi_list:
            if item.name in lumi[0]:
                divisor = lumi[1]/100
	        print lumi[0],item.entries
                #print lumi
                break

	#divisor = 1000
        print 'mass', item.mass, 'events',item.entries,'/',divisor*100, '[%]', item.entries/divisor    
        x_range.append(item.mass)
        y_range.append(item.entries/divisor)

    legend = legend.replace('BprimeB','B+b')
    legend = legend.replace('BprimeT','B+t')
    legend = legend.replace('_Width','')
    legend = legend.replace('_',' ')
    legend = legend.replace('X53',r'$X_{5/3}$')
    legend = legend.replace('0p','0%')
    legend = legend.replace('RH','$\Gamma / \mathrm{m_{VLQ}}$ RH')
    legend = legend.replace('LH','$\Gamma / \mathrm{m_{VLQ}}$ LH')


    if '0%' not in legend:
	legend = legend.replace('$\G','1% $\G') 

    print result[0],'->',legend
    plot_handles.append(plt.plot(x_range, y_range,label=str(legend)))

    
ax = plt.subplot() # Defines ax variable by creating an empty plot

# Set the tick labels font
for label in (ax.get_xticklabels() + ax.get_yticklabels()):
    label.set_fontsize(15)
plt.ylim(0.1,12.5)
plt.xlim(700,1800)
    
fontsize=15
plt.legend(loc='lower right',frameon=False,fontsize=fontsize,ncol=2)
plt.xlabel('VLQ mass [GeV]',fontsize=fontsize)#, loc='right')
plt.ylabel('$\epsilon_{signal} [\%]$',fontsize=fontsize)
#plt.title('CMS $\it{Preliminary}$', loc='left',fontsize=fontsize)
#plt.title('Signal efficiency', loc='right',fontsize=fontsize)

print 'result file',result_path
plt.savefig(result_path)
#plt.show()
  


#compute('limits/ROOTDataShape/Bay_BprimeT_RH_rebinned.root')
#binFile(0.3, 'RecoBp.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])


