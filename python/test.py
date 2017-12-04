#! /usr/bin/env python

import sys, time, glob 
#sys.argv.append('-b')
import math, warnings

import ROOT
#ROOT.gROOT.SetStyle("Plain")
#ROOT.gStyle.SetOptStat(000000000)
#ROOT.gStyle.SetOptTitle(0)

from ROOT import TCanvas, TFile, TH1

import numpy as np

def test():
    events = 10000
    variations = 10
    gaus_distro = np.random.normal(5, 1, events)        
    nominal = ROOT.TH1F('nominal','',50,0,10)
    for i in gaus_distro:
        nominal.Fill(i)
       
    weights =[]
    variations_h =[]
    for i in xrange(variations):
        weights.append(np.random.random(events))
        variations_h.append(ROOT.TH1F('var_'+str(i+1),'',50,0,10))
        
    for i in xrange(variations):
        for m in xrange(events):
            variations_h[i].Fill(gaus_distro[m], 2*weights[i][m])
            
    nominal.Draw("same")
    #for i in xrange(variations):
    #    variations_h[i].SetLineColor(3+i)
    #    variations_h[i].Draw("same hist")
    
    envelop_up = ROOT.TH1F('env_up','',50,0,10)
    envelop_up.SetLineColor(2)
    envelop_down = ROOT.TH1F('env_down','',50,0,10)
    envelop_down.SetLineColor(3)
    
    for i in xrange(nominal.GetNcells()):
        up =-1
        down = 99999999999
        for m in xrange(variations):
            bincont = variations_h[m].GetBinContent(i)
            if bincont > up: up = bincont
            if bincont < down: down =bincont
        envelop_up.SetBinContent(i,up)
        envelop_down.SetBinContent(i,down)
    envelop_down.Draw('same hist')
    envelop_up.Draw('same hist')

    time.sleep(5)
    
    weights_env_up = []
    weights_env_down = []

    for i in xrange(events):
        up =-1
        down = 2
        for m in xrange(variations):
            event_weight = weights[m][i]
            if event_weight > up: up = event_weight
            if event_weight< down: down = event_weight
        weights_env_up.append(up)
        weights_env_down.append(down)

    envelop_up_w = ROOT.TH1F('env_up_w','',50,0,10)
    envelop_up.SetLineColor(4)
    envelop_down_w = ROOT.TH1F('env_down_w','',50,0,10)
    envelop_down.SetLineColor(5)

    for i in xrange(events):
        envelop_up_w.Fill(gaus_distro[i],2*weights_env_up[i])
        envelop_down_w.Fill(gaus_distro[i],2*weights_env_down[i])
    envelop_up_w.Draw('same hist')
    envelop_down_w.Draw('same hist')



    
    time.sleep(15)


if __name__ == "__main__":
    test()
