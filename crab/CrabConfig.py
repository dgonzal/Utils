#!/usr/bin/env python

"""
*
* Usage :
* ./CrabConfig.py [options] 
*
*
"""
import argparse
import os, glob


from CRABClient.UserUtilities import config, getUsernameFromSiteDB

from CrabScript import *
from create_dataset_xmlfile import create_dataset_xml
from readaMCatNloEntries import readEntries

#requestNames = ['MC_BprimeTToTW_M-1200_LH_25ns']
inputDatasets = ['/BprimeTToTW_M-1200_LH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',
		 '/BprimeTToTW_M-1000_LH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',
		 '/BprimeTToTW_M-800_LH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',
		 '/BprimeTToTW_M-800_RH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM',			'/BprimeTToTW_M-1000_RH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM',
		 '/BprimeTToTW_M-1200_RH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM']



if __name__ == '__main__':        
        parser = argparse.ArgumentParser(description='Scripts for a simple multicrab')
        parser.add_argument('--xml', dest='xml_create',action='store_true',
                            default=False,
                            help="create xml Files for sframe")
        parser.add_argument('--submit','-s', dest='submit_flag', action='store_true',
                            default=False,
                            help='Submit to the grid')
        parser.add_argument('--readEntries', dest='readEntries', action='store',
                            default=0,
                            help='read the Entries by contained in each file. Specify how many core you want to use.')
        #parse.add_argument('--prostfix','-p', dest="postfix", action

        args = parser.parse_args()

	requestNames = []
	for x in inputDatasets:
		name = x.split('/')
		requestNames.append(name[1])


        config = config()
        config.General.workArea = 'crab_projects'
        config.General.transferOutputs = True
        config.General.transferLogs = True
        
        config.JobType.pluginName = 'Analysis'
        config.JobType.psetName = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_15_patch1/src/UHH2/core/python/ntuplewriter.py'
        config.JobType.outputFiles = ["Ntuple.root"]
        config.JobType.inputFiles = ['Summer15_25nsV2_MC.db']
        
        config.Data.inputDBS = 'global'
        config.Data.splitting = 'EventAwareLumiBased'
        config.Data.unitsPerJob = 5000
        config.Data.outLFNDirBase = '/store/user/dagonzal/' # % (getUsernameFromSiteDB())
        config.Data.publication = False
        #config.Data.allowNonValidInputDataset = True
        #config.Data.publishDataName = 'CRAB3_tutorial_May2015_MC_analysis'

        config.Site.storageSite = 'T2_DE_DESY'
        
        if args.submit_flag:
                work = submit_config(config)
                work.submitByDatasets(inputDatasets,requestNames,'_v7')

        if args.xml_create:
                for name in requestNames:
                        dirname = '/pnfs/desy.de/cms/tier2/'+config.Data.outLFNDirBase+name+'/crab*_v7/**/**/*.root'
                        l = glob.glob(dirname)
                        xmlname = name.replace('_TuneCUETP8M1_13TeV-madgraph-pythia8','')
                        xmlname += '.xml'
                        #print xmlname, l
                        create_dataset_xml(dirname,xmlname)

        if args.readEntries > 0:
                fileList =[]
                for name in requestNames:
                        xmlname = name.replace('_TuneCUETP8M1_13TeV-madgraph-pythia8','')
                        xmlname += '.xml'
                        fileList.append(xmlname)
                readEntries(args.readEntries,fileList)
                
