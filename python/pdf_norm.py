#! /usr/bin/env python

import sys, time, glob 
import math, warnings

import ROOT

def read_xml(xmlFileDir):
    #try:
    xmlFile = open(xmlFileDir)
    rootFileStore = []
    for line in xmlFile:
        if '<!--' in line:continue
        rootFileStore.append(line.split('"')[1])
    xmlFile.close()
    return rootFileStore

def pdf_norm(filename, treename="AnalysisTree"):
    work_file = ROOT.TFile(filename)
    tree = work_file.Get(treename)
    
    cross_section = 0
    pdf_sum = 0
    for event in tree:
        original = event.genInfo.originalXWGTUP()
        cross_section += 1 #event.genInfo.weights()[0]
        sys = 0
        counter=-1        
        for weight in event.genInfo.systweights():
            counter +=1
            if counter > 108:break
            if counter > 8:
                sys += (weight/original-1)*(weight/original-1)
        pdf_sum += math.sqrt(sys/99)* original/math.fabs(original)
        #print 'event weight', original, 'pdf', 1./99.*math.sqrt(sys)* original/math.fabs(original)
    print 'plus', 1+pdf_sum/cross_section, 'minus', 1-pdf_sum/cross_section, 'yield pdf + nominal',  pdf_sum+cross_section,  'yield pdf - nominal', pdf_sum+cross_section
    return pdf_sum,cross_section

    
if __name__ == "__main__":
    filename = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/MuSigSel/uhh2.AnalysisModuleRunner.MC.BprimeT-1800_LH.root'

    #filename = '/pnfs/desy.de/cms/tier2//store/user/dagonzal/80X_v3/MC_v1/BprimeBToTW_M-1200_RH_TuneCUETP8M1_13TeV-madgraph-pythia8/crab_BprimeBToTW_M-1200_RH/170112_125120/0000/Ntuple_1.root'
    
    pdf_norm(filename)
    
    exit(0) 
    masses = ['800','1000','1200','1400','1600','1800'] 

    for mass in masses:
        xmldir = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/common/datasets/RunII_80X_v3/Bprime/BprimeBToTW_M-'+mass+'_RH.xml'
        sel = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/MuSigSel/uhh2.AnalysisModuleRunner.MC.BprimeB-'+mass+'_LH.root'

        print xmldir
        file_store = read_xml(xmldir)
        pdf_norm(file_store[0])
        pdf_norm(sel)
        #for filename in file_store: 
        #    pdf_norm(filename)
