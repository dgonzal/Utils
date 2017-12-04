#! /usr/bin/env python

import sys
sys.argv.append('-b')

import math, warnings

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

from ROOT import TCanvas, TFile, TH1, THStack, TLegend

debug = False

class hinfo:
  def __init__(self, name):
    fields = name.split('__')
    self.channel = fields[0]
    self.process = fields[1]
    self.systematic = None
    self.shift = None
    #print name
    if len(fields) > 2:
      self.systematic = fields[2]
      self.shift = fields[3]
      
def name(channel, process, systematic = None, shift = None):
  if not systematic:
    return '__'.join([channel, process])
  return '__'.join([channel, process, systematic, shift])

def merge(old,new):
  if not old:
    old = new.Clone()
  else:
    old.Add(new)
  return old

def findMaximum(histogram):
    index = 1
    maximum = 0
    for i in xrange(1,histogram.GetNbinsX()+1):
        if histogram.GetBinContent(i) > maximum:
            maximum = histogram.GetBinContent(i)
            index = i
    return index, maximum

def findLowIndex(histogram, rerror):
    value = 0.0
    error = 0.0
    index, maximum = findMaximum(histogram)
    for i in range(1,histogram.GetNbinsX()+1):
        value = value + histogram.GetBinContent(i)
        error = math.sqrt(error**2+histogram.GetBinError(i)**2)
        if value > maximum and i > 1:
          return i-1
        elif value > maximum and i < 1:
          return 1
        ratio = 1.0
        if value != 0: ratio = error/value
        if ratio < rerror:
            return i

def findBinSize(histogram, highindexes, rerror, minvalue, maxbinsize, start, stop):
    #print "minvalue %f, maxbinsize %d, start %d, stop %d : " % (minvalue, maxbinsize, start, stop), highindexes
    value = 0.0
    error = 0.0
    for i in range(start, stop, -1):
        value = value + histogram.GetBinContent(i)
        error = math.sqrt(error**2+histogram.GetBinError(i)**2)
        ratio = 1.0
        binsize = start - i + 1
        if value != 0: ratio = error/value
        # if ratio < rerror and value*(1+ratio) >= minvalue:
        if ratio < rerror:
            if binsize <= maxbinsize:
                highindexes.append(i)
                if not findBinSize(histogram, highindexes, rerror, value, binsize, i-1, stop):
                    highindexes.pop()
                    continue
                return True    
            else:
                return False
    # highindexes.append(stop+1)
    return True        

def computeBinning(histogram, rerror):
    highindexes = []
    lowindex = findLowIndex(histogram, rerror)
    maxindex, maximum = findMaximum(histogram)
    #print 'Number of bins',histogram.GetNbinsX()
    findBinSize(histogram, highindexes, rerror, 0, histogram.GetNbinsX(), histogram.GetNbinsX(), lowindex)     
    highindexes = sorted(highindexes)
    binning = [histogram.GetBinLowEdge(0), histogram.GetBinLowEdge(lowindex)+histogram.GetBinWidth(lowindex)]
    #print 'nominal binning', binning
    #print highindexes
    for i in highindexes[1:]:
        binning.append(histogram.GetBinLowEdge(i))
    binning.append(histogram.GetBinLowEdge(histogram.GetNbinsX())+histogram.GetBinWidth(histogram.GetNbinsX()))
    #print binning
    return binning   

import array

def binFile(rerror, filename, xtitle, backgrounds): 
    warnings.simplefilter("ignore")
    file = TFile(filename)
    keys = file.GetListOfKeys()

    print 'input file:',filename
    print 'background:',backgrounds
    h_bkg = {}
    h_data = {}

    resultfile = filename.split('.')[0]+'_rebinned.root'
    
    # load all the background and data histograms
    for key in keys:
        key = key.GetName()
        info = hinfo(key)
        if not info.systematic:
            if info.process in backgrounds:
                if info.channel in h_bkg:
                    print 'merging',info.channel
                    h_bkg[info.channel] = merge(h_bkg[info.channel], file.Get(key).Clone())
                else:
                    print 'adding',info.channel
                    h_bkg[info.channel] = file.Get(key).Clone()
            elif info.process == 'data' or info.process == 'DATA':
                if info.channel in h_data:
                    h_data[info.channel] = merge(h_data[info.channel], file.Get(key).Clone())
                else:
                    h_data[info.channel] = file.Get(key).Clone()

                    
    canvas = TCanvas()
    canvas.SetLogy()

    #keys = file.GetListOfKeys()
    output = TFile(resultfile, 'RECREATE')
    #print h_bkg[1]	
    # print all the histograms for all the channels
    for key in h_bkg:
        binning = array.array('d', computeBinning(h_bkg[key], rerror))
        #print binning
        h_bkg[key] = h_bkg[key].Rebin(len(binning)-1, h_bkg[key].GetName(), binning)
       
        h_bkg[key].SetLineColor(ROOT.kGray+1)
        h_bkg[key].SetFillColor(ROOT.kGray+1)

        maxs=h_bkg[key].GetMaximum()
        
        if len(h_data)>0:
          h_data[key] = h_data[key].Rebin(len(binning)-1, h_data[key].GetName(), binning)
          h_data[key].SetLineColor(ROOT.kBlack)  
          h_data[key].SetMarkerStyle(20)
          maxs = [h_data[key].GetMaximum(), h_bkg[key].GetMaximum()]
          
        pad = canvas.cd(1)
        pad.SetLeftMargin(0.15)
        pad.SetBottomMargin(0.15)

        min = h_bkg[key].GetBinContent(h_bkg[key].GetMinimumBin())


        if len(h_data)>0:
          h_data[key].GetYaxis().SetRangeUser(0.5*min,max(maxs)*1.8)

          h_data[key].GetYaxis().SetLabelSize(0.05)
          h_data[key].GetYaxis().SetTitleSize(0.05)
          h_data[key].GetYaxis().SetTitle('event yield')
          h_data[key].GetXaxis().SetLabelSize(0.05)
          h_data[key].GetXaxis().SetTitleSize(0.05)
          h_data[key].GetXaxis().SetTitle(xtitle)

          h_data[key].Draw('e')
          
        h_bkg[key].Draw('samehist')
        
        if len(h_data)>0:
          h_data[key].Draw('samee')

        legend = TLegend(.67, .78, .89, .88)
        legend.SetMargin(0.12);
        legend.SetTextSize(0.03);
        legend.SetFillColor(10);
        legend.SetBorderSize(0);
        legend.AddEntry(h_bkg[key], "Background", "f")
        if len(h_data)>0:legend.AddEntry(h_data[key], "CMS Data 2016", "lp")
        legend.Draw()

        labelcms = TLegend(.15, .91, 1, .96)
        labelcms.SetTextSize(0.04)
        labelcms.SetMargin(0.12);
        labelcms.SetFillColor(10);
        labelcms.SetBorderSize(0);
        labelcms.SetHeader('CMS Preliminary #sqrt{s} = 13 TeV')
        labelcms.Draw()

        labellumi = TLegend(.73, .91, 1, .96)
        labellumi.SetTextSize(0.04)
        labellumi.SetMargin(0.12);
        labellumi.SetFillColor(10);
        labellumi.SetBorderSize(0);
        labellumi.SetHeader('L = 35.8 fb^{-1}')
        labellumi.Draw()

        labellumi2 = TLegend(.67, .70, .89, .75)
        labellumi2.SetTextSize(0.03)
        labellumi2.SetMargin(0.12);
        labellumi2.SetFillColor(10);
        labellumi2.SetBorderSize(0);
        labellumi2.SetHeader(key)
        labellumi2.Draw()

        canvas.SaveAs('h_'+filename.split('.')[0]+'_'+key+'.pdf')
      
        for hkey in keys:
            hkey = hkey.GetName()
            info = hinfo(hkey)
           
            if info.channel == key:
                #print hkey
                #print info.channel,key
                #print hkey
                histogram = file.Get(hkey).Clone()
		
                # Hack to fix the data lowercase names
                #if info.systematic == 'pdf':
                #    continue

                #if (info.systematic == 'scale' or info.systematic == 'matching') and 'light' in info.process and '1btag' in info.channel:
                #    print "Excluding : ", info.systematic, info.process, info.channel
                #    continue
                if 'data' in info.process:
                    histogram.SetName(histogram.GetName().replace('data','DATA'))

                if info.systematic:
                  if ('ttjets' in info.process.lower() or 'ttbar' in info.process.lower()) and 'scale' in info.systematic:
                    orig = histogram.GetName()
                    histogram.SetName(histogram.GetName().replace('scaleWeight','scale_ttbar'))
                    if debug: print "Renaming: %s to %s" % (orig, histogram.GetName())
                  elif ('wjets' in info.process.lower() or 'zjets' in info.process.lower()) and 'scale' in info.systematic:
                    orig = histogram.GetName()
                    histogram.SetName(histogram.GetName().replace('scaleWeight','scale_vjets'))
                    if debug:print "Renaming: %s to %s" % (orig, histogram.GetName())
                  elif 'singlet' in info.process.lower() and 'scale' in info.systematic:
                    orig = histogram.GetName()
                    histogram.SetName(histogram.GetName().replace('scaleWeight','scale_singletop'))
                    if debug:print "Renaming: %s to %s" % (orig, histogram.GetName())
                  elif ('bpb' in info.process.lower() or 'bpt' in info.process.lower()) and 'scale' in info.systematic:
                    orig = histogram.GetName()
                    histogram.SetName(histogram.GetName().replace('scaleWeight','scale_bp'))
                    if debug:print "Renaming: %s to %s" % (orig, histogram.GetName())
                  elif 'qcd' in  info.process.lower() and 'scale' in info.systematic:
                    if debug: print "ignoring QCD scale uncertainties"
                    continue
                  
                #following not used at the moment!
                if ('wlight' in info.process or 'zlight' in info.process) and info.systematic == 'matching':
                    orig = histogram.GetName()
                    histogram.SetName(histogram.GetName().replace('matching','matching_vjets'))
                    print "Renaming: %s to %s" % (orig, histogram.GetName())


                if 'mcregion' in hkey:
                  print key
                  channel = info.channel.replace('Mu','')
                  channel = channel.replace('Ele','')
                  histogram.SetName(histogram.GetName().replace('mcregion','mcregion_'+channel))

                    
		#Rename the hists to make it easier for theta to find the mass
		if 'X53' in info.process:
		  histogram.SetName(histogram.GetName().replace('X53','X'))
                    
                #norm signal, just pure shape
                if ('Bprime' in info.process or 'X53' in info.process) and info.systematic:
                  for h_hkey in keys:
                    h_hkey = h_hkey.GetName()
                    h_info = hinfo(h_hkey)
                    if not h_info.systematic and info.process == h_info.process and h_info.channel == info.channel:
                      h_norm = file.Get(hkey).Clone()
                      events_unc = 0
                      events_nom = 0
                      for i in xrange(1,histogram.GetNbinsX()+1):
                        events_nom += h_norm.GetBinContent(i) 
                        events_unc += histogram.GetBinContent(i)
                      #print 'Going to normalize to the same value', h_info.process,h_info.channel,info.systematic,'factor',events_nom/events_unc
                      #histogram.Scale(events_nom/events_unc)
                      
                #print 'rebinning',hkey 
                histogram = histogram.Rebin(len(binning)-1, histogram.GetName(), binning)
                for i in xrange(1,histogram.GetNbinsX()+1):
                  if histogram.GetBinContent(i)<=0. : histogram.SetBinContent(i,0.0001)
                  
                output.cd()
		histogram.Sumw2()
                histogram.Write()
    return resultfile


#binFile(0.3, 'RecoBp.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])


