from subprocess import call
import os, glob, sys, time


signal_prefixes = ['X53','Bprime']#'X53',
channels = ['Mu','Ele','']#'Mu','Ele',
productions = ['B','T'] #'B','T']
chiralities = ['RH','LH'] #['RH','LH']
witdths =['','10','20','30']

#signal_prefixes = ['Bprime']
signal_prefixes = ['X53']
channels = ['']
#channels = ['']
productions = ['']
chiralities = ['RH']
witdths =['']

xmldir = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/common/datasets/RunII_80X_v3/Bprime/'
release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
sourcedirs = ['/MuSigSel_v1/','/EleSigSel_v1']
#sourcedirs =['/EleSigSel_v1','/MuSigSel/']
rootDir = 'ROOTDataShape_50_0_3500_eta4/'
prefix = 'Eta4_'
createfiles = False 
limits =  True
do_discovery = False  
single_cat = True 

if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)

"""
#execfile("histogram_rebinning.py")
execfile("calculation_datadriven.py")
execfile("invert_bin_content.py")
execfile("injection_merge.py")
execfile('norm.py')
"""

sys.path.append('/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/Utils/limits/')
sys.path.append('/nfs/dust/cms/user/gonvaq/theta/utils2/theta_auto')

from signal_background_uncertainty import *    
from calculation_datadriven import *
from invert_bin_content import *
from injection_merge import *
from calc_chi2 import *
from norm import *

for source_num,channel in enumerate(channels):
    print 'channel',channel
    print '='*50
    for signal_pre in signal_prefixes:
        for count,production in enumerate(productions):
            if 'X53' in signal_pre:
                production=''
                if count>0:continue
            for chirality in chiralities:
                for width in witdths:
                    dirstring =''
                    ###Files for the discovery calculation 
                    discovery_calcs = []
                    if source_num <len(sourcedirs):
                        dirstring = release+sourcedirs[source_num]
                    rootfile = rootDir+prefix+signal_pre+production+'_'+channel+'_'+chirality
                    if width:
                        rootfile +='_'+width
                    if not channel:
                        dirstring = rootfile.replace('__','_*_')
                        rootfile = rootfile.replace('__','_')
                        print rootfile,dirstring
                        #if createfiles:
                        pass
                        print 'Going create files for combination'
                        call(['rm '+rootfile+'_rebinned.root '],shell=True)
			print dirstring+"_rebinned.root"
                        if glob.glob(dirstring+"_rebinned.root"):
                            pass
                            call(['hadd '+rootfile+'_rebinned.root '+dirstring+'_rebinned.root'],shell=True)                                
			    time.sleep(1)
                    else:
                        signal = None
                        print rootfile,dirstring
			print 'Width',width,'%'
                        print dirstring+'/*'+signal_pre+production+'*'+chirality+'*.root'
                        for name in glob.glob(dirstring+'/*'+signal_pre+production+'*'+chirality+'.root'):
                            #print width, name
			    #if '1600' in name: continue
			    #if signal: continue 
                            name = (name.split('/')[-1]).split('.')[-2]
                            if not width:
                                if not 'Width' in name:
                                    if signal:signal += ','+name
                                    else:     signal = name
                            else:
                                if width+'p' in name:
                                    if signal:signal += ','+name
                                    else:     signal = name
                        print signal
                        if not signal:
                            print 'no signal found:',signal_pre+production
                            continue


			path = prefix+width+'_output_'+signal_pre+production+'_'+chirality+'_'+channel
			if not channel : path = prefix+width+'_output_'+signal_pre+production+'_'+chirality
			#path = path.replace('X53','X')
			print path
			#continue
                        
  			if not os.path.exists(rootfile+'_Signal.root'):
                            print 'signal file does not exist',rootfile+'_Signal.root'
                        if createfiles or not os.path.isfile(rootfile+'_Signal.root'): 
                            pass
                            print "going to create signal file with",'./../bin/rootfilecreator',signal,rootfile+'_Signal',dirstring,channel 
			    call(['./../bin/rootfilecreator',signal,rootfile+'_Signal',dirstring,channel,"","",""])
			    #exit(0)
                            call(['rm '+rootfile+'_rebinned.root'],shell=True)
                            call(['hadd '+rootfile+'_rebinned.root '+rootfile+'_Signal'+'.root '+rootDir+'/'+channel+'_background_mc_uncer.root'],shell=True)
                            #For the discovery calculation with signal in the background
                            if do_discovery and createfiles: 
                                for item in signal.split(","):
                                    central_sig = rootDir+prefix+"central_signal_"+item+'.root'
                                    discovery_file  = rootfile+'_'+item+'_rebinned.root'
                                    if createfiles or not os.path.isfile(discovery_file): 
                                        call(['./../bin/rootfilecreator', item, central_sig, dirstring,channel,"","","central"])
                                        inverted_signal = invert_bin_content(central_sig,"Signal")
                                        call(['rm '+discovery_file],shell=True)
                                        call(['hadd '+discovery_file+' '+rootfile+'_Signal'+'.root '+rootDir+'/'+channel+'_background_mc_uncer.root'+' '+inverted_signal],shell=True)
                                        signal_rebin(discovery_file, signal_pre, 'Background')
                                        selfignal_rebin(discovery_file, 'Signal', 'Background')
                                        #scale_pdf_reweight(discovery_file,xmldir, signal_pre+production,chirality,width)
                                        sys_reweight(discovery_file,'PDF')
                                        sys_reweight(discovery_file,'scale')
                                    discovery_calcs.append(discovery_file)
                            
                            ##binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
                            signal_rebin(rootfile+'_rebinned.root', signal_pre, 'Background')
			    #sys_reweight(rootfile+'_rebinned.root','PDF')
                            #sys_reweight(rootfile+'_rebinned.root','scale')
                            scale_pdf_reweight(rootfile+'_rebinned.root',xmldir, (signal_pre+production),chirality,width)
			    
                    #print signal_pre
		    #continue
		    #if width: sys.exit(0)

                    workfile = rootfile+"_rebinned.root"
                    workfile = rescale_on_count(workfile,'DATA','Background','',signal_pre+production)

                    if single_cat:
                        categories = sculp_categories(workfile)
			#print categories
			#exit(0)
                        for cat in categories:
                            if limits and os.path.isfile(cat):
                                if 'X53' in signal_pre:
			            rename_hists(cat,'X53','X')
				    exp,obs = run_cat(cat, chirality, "X", True,channel)
                                else:
			            rename_hists(rootfile+"_rebinned.root",'-','_')
				    exp,obs = run_cat(cat, chirality, signal_pre+production, True,channel)
		            elif limits and not os.path.isfile(cat):
			        print cat,'does not exist'
                        continue


                    
                    if limits and os.path.isfile(workfile):
                        if 'X53' in signal_pre:
			    rename_hists(workfile,'X53','X')
                            exp,obs = run_cutopt(workfile,chirality,channel,'X',False,"",0,prefix+width+'_')
                        else:
			    rename_hists(rootfile+"_rebinned.root",'-','_')
                            exp,obs = run_cutopt(workfile,chirality,channel,signal_pre+production,False,"",0,prefix+width+'_')
		    elif limits and not os.path.isfile(workfile):
			print workfile,'does not exist'

                    if do_discovery:
                        for discovery_file in discovery_calcs:
                            rename_hists(discovery_file,'X53','X')
                            rename_hists(discovery_file,'-','_')
                            particle = signal_pre+production
                            if 'X' in signal: particle = 'X'                            
                            background_signal_discovery(discovery_file, chirality, channel, particle, False, "", 0,prefix+width+'_')
                        #sys.exit(0)

