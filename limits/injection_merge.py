#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)



from ROOT import TFile, TH1, TObject

import numpy as np
import time
from shutil import copyfile
import math

import array

def find_files(dirstring):
    result = ""
    print 'Searching',dirstring
    for name in glob.glob(dirstring):
        name = (name.split('/')[-1]).split('.')[-2]
        result+=name+','
    return result[0:-1]


# merge all the histograms to one
def create_merged_hists(input_fname, output_fname,postfix,region =['','']):
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
            new_name = category+'__'+postfix
            if region[0]:
                new_name = new_name.replace(region[0],region[1])
            histos.append(in_file.Get(key).Clone(new_name))
            histos[-1].Sumw2()
            cloned.append(key)

    #print cloned
            
    for key in keys:
        key = str(key.GetName())
        for i, name in enumerate(hist_names):
            #print name,key
            if name in key:
                if key not in cloned:
                    #print 'merging histogram',key,'to',histos[i].GetName()#,cloned[i]
                    hist = in_file.Get(key).Clone(category+'__'+postfix)
                    #hist.Draw()
                    #time.sleep(2)
                    if type(hist) is not type(histos[i]):
                        print 'can not merge',key,'. it is not of the same type'           
                    histos[i].Add(hist)
		    #print ''
		    break
  		else:
		    #print''
		    break
		                  

    in_file.Close()    
    out_file =TFile(output_fname,"RECREATE")
    out_file.cd()
    for item in histos:
        item.Write()
    out_file.Close()
    del hist_names
    del histos
    del cloned

    print 'done with merging'
    print '='*10

def add_signal(fname, process, signal_file, signal, scale):
    print 'adding signal', signal, ' with cross section', scale,'to',process
    print 'working file',fname,'signal file',signal_file
    signal_file = TFile(signal_file,'READ')
    work_file =TFile(fname,'UPDATE')
    keys = work_file.GetListOfKeys()
    signal_keys = signal_file.GetListOfKeys()
    original = []
    for key in keys:
        key = str(key.GetName())
        if len(key.split('__'))>2: continue
        category = key.split("__")[0]
        #print key, category
        if process not in key:continue
        for signal_key in signal_keys:
            signal_key =  str(signal_key.GetName())
            #print 'comparing',key, signal_key
            sig_cat = signal_key.split("__")[0]
            #print signal, signal_key.split('__')[1]
            if signal.replace('-','_') not in signal_key.split("__")[1].replace('-','_'):continue
	    #print category, sig_cat
            if category == sig_cat:
                print 'adding to',signal_key,'to',key 
                sig_hist = signal_file.Get(signal_key).Clone(key)
                #print sig_hist.GetSumOfWeights()
                original.append(work_file.Get(key))
                binning = get_binning(original[-1])
                if binning:
		   sig_hist = sig_hist.Rebin(len(binning)-1,key,array.array('d',binning))
                sig_hist.Scale(scale)
		print 'adding',sig_hist.GetIntegral()[0], 'to', original[-1].GetIntegral()[0] 
                original[-1].Add(sig_hist)
                break

    signal_file.Close()
    work_file.cd()
    for item in original:
        item.Write('',TObject.kOverwrite);
    work_file.Close()

    
def scale_hists(fname, process, factors):
    print 'scaling', fname,process
    work_file =TFile(fname,'UPDATE')
    keys = work_file.GetListOfKeys()
    hist_list =[]
    for key in keys:
        key = str(key.GetName())
        #if len(key.split('__'))>2: continue
        category = key.split("__")[0]
        if process not in key.split("__")[1]:continue
        #print key, category
        
        if(isinstance(factors, list)):
            for factor in factors:
                catname = factor[0]
                catname = catname.replace('Anti-b-tag_rate','AntiBTag')
                catname = catname.replace('1-b-tag_rate','1_BTag')
                catname = catname.replace('2-b-tag_rate','2_BTag')
                catname = catname.replace('W-tag_rate','WTag')
                catname = catname.replace('top-tag_rate','TopTag')
                #print category, catname
                if catname in category:
                    print 'scaling',key,'by',factor[1]
                    hist = work_file.Get(key)
                    hist.Scale(factor[1])
                    hist_list.append(hist)
                    #hist.Write('',TObject.kOverwrite);
        else:
            print 'scaling',key,'by',factors
            hist =work_file.Get(key)
            hist.Scale(factors)
            hist_list.append(hist)
        

    for item in hist_list:
        item.Write('',TObject.kOverwrite);
        item.SetDirectory(0)
    work_file.Close()
    del hist_list

            
def create_injectionratio_plots(outputname, xaxis, yaxis, ytitle, legend):
    pp = PdfPages(outputname)
    plt.clf()
    plt.title("CMS $\it{Preliminary}$", fontsize=10)# , loc='right')
    
    ax=plt.gca()
    #ax.set_ylim([.5,1.50])
    ax.set_xlim([650,1850])
    plt.plot(xaxis,yaxis,color='red', label=legend)
    plt.axhline(1, color='black')
    plt.legend(loc="upper center",prop={'size':12},frameon=False,numpoints=1) 
    plt.xlabel('injected VLQ mass (GeV)')
    plt.ylabel(ytitle)
    
    plt.savefig(pp, format='pdf')
    pp.close()
    


def get_binning(histogram):
    binning=[]
    for i in xrange(1,histogram.GetNcells()-1):
      binning.append(histogram.GetBinLowEdge(i))
    binning.append(histogram.GetBinLowEdge(histogram.GetNcells()-2)+histogram.GetBinWidth(histogram.GetNcells()-2))
    return binning

def update(hist_original, max_unc, error='poisson'):
    binning = get_binning(hist_original)
    update=True
    loop_counter=0
    hist = hist_original.Clone()
    hist.Sumw2(ROOT.kFALSE)
    hist.SetBinErrorOption(1)
    hist.Sumw2()
    print 'Working on histogram',hist.GetName(),'number of events',hist.GetSumOfWeights()
    while update and loop_counter < 500:
        #print loop_counter
        #print '#=*'*15 
        update = False
        #print 'binning', binning
        for i in xrange(1,hist.GetNcells()-1):
            #if hist.GetBinContent(i)>0: 
            #    print 'bin',i,'uncer',hist.GetBinError(i)/hist.GetBinContent(i),'content',hist.GetBinContent(i)
            if hist.GetBinContent(i)==0 or hist.GetBinError(i)/hist.GetBinContent(i) > max_unc:
                #print hist.GetBinError(i)
	        up = -1; down =-1;
                if(i<hist.GetNcells()-2): up = hist.GetBinError(i+1)
	        if(i>1): down = hist.GetBinError(i-1)           
                if up > down:
                    del binning[i]
	        else:
                    del binning[i-1]
                hist = hist.Rebin(len(binning)-1,hist.GetName(),array.array('d',binning))
                loop_counter+=1	
                update=True
	        if error == 'poisson': 
                  for i in xrange(1,hist.GetNcells()-1):
	             hist.SetBinError(i,math.sqrt(hist.GetBinContent(i)))
                break 
    #print hist.GetName()
    #print binning
    return binning 
	 
class cat_cont(object):
   def __init__(self, cat_, process_, hist, err_ = 'poisson'):
      self.category = cat_
      self.hist_list = [hist]
      self.process = process_
      self.error = err_ 
   def add(self, hist):
	name = hist.GetName()
 	split = name.split('__')
	if self.category == split[0] and self.process == split[1]:
	   self.hist_list.append(hist)
	   return True
	return False

   def rebin(self, binning):
        print 'Rebinning everything with',binning
	for i,item in enumerate(self.hist_list):
   	  self.hist_list[i] = item.Rebin(len(binning)-1,item.GetName(),array.array('d',binning))
	  if self.error == 'poisson':
             for i in xrange(1,item.GetNcells()-1):
	        item.SetBinError(i,math.sqrt(item.GetBinContent(i)))
          #print 'New binning'
          #print get_binning(item)
   def main(self):
	for item in self.hist_list:
	   if len(item.GetName().split('__'))==2:
		return item
	raise ValueError('No main histogram was found for '+self.category+'__'+self.process)
	sys.exit(-1)

def simpleRebin(inputfile,max_uncer,processes=[],output_name=None, gaus_error = []):
    if not output_name:
       output_name = inputfile.replace('.root','_rebinned.root')
       
    work_file = TFile(inputfile,"READ")    
    keys = work_file.GetListOfKeys()

    print 'rebinning ',inputfile

    histogramlist = []
    for key in keys:  
        key = str(key.GetName())
        append = False
        hist = work_file.Get(key).Clone()
        for item in histogramlist:
 	   append = item.add(hist)
 	   if append: break	  
	if not append:
           #print 'new category',key
           if key.split('__')[1] in gaus_error or 'All' in gaus_error:
	      histogramlist.append(cat_cont(key.split('__')[0], key.split('__')[1], hist,'gaus')) 
	   else:
	      histogramlist.append(cat_cont(key.split('__')[0], key.split('__')[1], hist)) 
	      	 

    for process in processes:
      binning = []
      for item in histogramlist:	
 	if item.process == process:
	  binning = update(item.main(),max_uncer,item.error)
 	  if binning:
	      for i in histogramlist:
                #print 'Check that the binning corresponds to the correct category:',i.category, item.category
                if i.category == item.category:
                    i.rebin(binning)
    work_file.Close()
    output_file = TFile(output_name,'RECREATE')
    for item in histogramlist:
        for hist in item.hist_list:
            #print 'Writing histogram', hist.GetName()
            #print 'binning',get_binning(hist)
            #hist.Sumw2()
            hist.Write('',TObject.kOverwrite)
         
    output_file.Close()
    del histogramlist
    return output_name


def sys_reweight(filename, weight = ''):
    work_file = TFile(filename,"UPDATE")
    keys = work_file.GetListOfKeys()
    nominal_hists=[]
    uncer_list = [] 
    for key in keys:  
        key = str(key.GetName())
        if 'DATA' in key or 'Data' in key or 'Background' in key: continue
        if 'minus' in key or 'plus' in key:
            uncer = key.split('__')[2]
            if uncer not in uncer_list: uncer_list.append(uncer)
            continue
        if key not in nominal_hists:
            nominal_hists.append(key)

    for hist in nominal_hists:
        #print hist
        nominal_number = (work_file.Get(hist)).Integral()
        for uncer in uncer_list:
            if weight not in uncer: continue
            up = work_file.Get(hist+'__'+uncer+'__plus')
            down = work_file.Get(hist+'__'+uncer+'__minus')
            #print 'scale plus:',nominal_number/up.Integral(),'minus',nominal_number/down.Integral()
            up.Scale(nominal_number/up.Integral())
            down.Scale(nominal_number/down.Integral())
            up.Write('',TObject.kOverwrite);
            down.Write('',TObject.kOverwrite);
        
    work_file.Close()
    del nominal_hists
        
def signal_rebin(rebin_file, signal = "Bprime", process='DATA'):
    print 'starting the rebinning of',rebin_file
    work_file = TFile(rebin_file,"UPDATE")
    keys = work_file.GetListOfKeys()
    hist_list = []
    for key in keys:  
        key = str(key.GetName())
        if process in key:
            hist = work_file.Get(key)
            binning = get_binning(hist)
            for other in keys:
                other = str(other.GetName())
                if not key.split('__')[0] in other: continue
                if not signal in other: continue
                #if len(other.split('__'))>2:continue
                if not process in other:
                    #print other
                    work_hist = work_file.Get(other)
                    hist_list.append(work_hist.Rebin(len(binning)-1,other,array.array('d',binning)))
                    #work_hist.Write('',TObject.kOverwrite)
                    
    for item in hist_list:
        item.Write('',TObject.kOverwrite);    
    work_file.Close()
    del hist_list            
 

    
def rename_hists(filename, from_s, to_s):
    print 'rename hists in',filename,'from',from_s,'to',to_s
    work_file = TFile(filename,"UPDATE")
    keys = work_file.GetListOfKeys()
    for key in keys:
        
        key = str(key.GetName())
        if from_s in key:
            #print key
            hist = work_file.Get(key).Clone(key.replace(from_s,to_s))
            work_file.Delete(key+';*')
            hist.Write()
