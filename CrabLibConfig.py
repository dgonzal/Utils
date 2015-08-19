from CrabScript import *
from CRABClient.UserUtilities import config, getUsernameFromSiteDB

config = config()

config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_7/src/UHH2/core/python/ntuplewriter.py'
config.JobType.outputFiles = ["Ntuple.root"]

config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 10000
config.Data.outLFNDirBase = '/store/user/dagonzal/' # % (getUsernameFromSiteDB())
config.Data.publication = False
#config.Data.publishDataName = 'CRAB3_tutorial_May2015_MC_analysis'

config.Site.storageSite = 'T2_DE_DESY'

requestNames = ['MC_BprimeBToTW_M-800_RH','MC_BprimeBToTW_M-800_LH','MC_BprimeBToTW_M-1000_RH','MC_BprimeBToTW_M-1000_LH','MC_BprimeBToTW_M-1200_RH','MC_BprimeBToTW_M-1200_LH']
inputDatasets = ['/BprimeBToTW_M-800_LH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM',
                 '/BprimeBToTW_M-800_RH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM',
                 '/BprimeBToTW_M-1000_LH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM',
                 '/BprimeBToTW_M-1000_RH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM',
                 '/BprimeBToTW_M-1200_LH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM',
                 '/BprimeBToTW_M-1200_RH_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM']


work = submit_config(config)
work.submitByDatasets(inputDatasets,requestNames,'_v3')
