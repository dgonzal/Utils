from subprocess import call
import os, glob, sys, time


signal_prefixes = ['X53','Bprime']#'X53',
channels = ['Mu','Ele','']#'Mu','Ele',
productions = ['B','T'] #'B','T']
chiralities = ['RH','LH'] #['RH','LH']
witdths =['','10','20','30']

signal_prefixes = ['X53']#'X53',
channels = ['']
productions = ['T']
chiralities = ['RH','LH']
witdths =['']


release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
sourcedirs = ['/MuSigSel/','/EleSigSel_v1']
rootDir = 'ROOTDataShape/'
prefix = 'Bay_'
createfiles = True 
limits = True


#not implemented yet
#signal_injection = False



if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)

execfile("histogram_rebinning.py")
execfile("calculation_datadriven.py")


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
                    if source_num <len(sourcedirs):
                        dirstring = release+sourcedirs[source_num]
                    rootfile = rootDir+prefix+signal_pre+production+'_'+channel+'_'+chirality
                    if width:
                        rootfile +='_'+width
                    if not channel:
                        dirstring = rootfile.replace('__','_*_')
                        rootfile = rootfile.replace('__','_')
                        print rootfile,dirstring
                        if createfiles:
                            pass
                            print 'Going create files for combination'
                            call(['rm '+rootfile+'_rebinned.root '],shell=True)
                            #print 'hadd '+rootfile+'_return ebinned.root '+dirstring+'_rebinned.root'
                            #sys.exit(0)
			    print dirstring+"_rebinned.root"
                            if glob.glob(dirstring+"_rebinned.root"):
                                pass
                                call(['hadd '+rootfile+'_rebinned.root '+dirstring+'_rebinned.root'],shell=True)
                    else:
                        signal = None
                        print rootfile,dirstring
			print 'Width',width,'%'
                        #print dirstring+'/*'+signal_pre+production+'*'+chirality+'*.root'
                        for name in glob.glob(dirstring+'/*'+signal_pre+production+'*'+chirality+'.root'):
                            #print width, name
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
                        if createfiles: 
               		    pass
                            print "going to create signal file with",'./../bin/rootfilecreator',signal,rootfile+'_Signal',dirstring,channel
                            call(['./../bin/rootfilecreator',signal,rootfile+'_Signal',dirstring,channel,"","",""])
                            call(['rm '+rootfile+'.root '],shell=True)
			    time.sleep(1)
                            call(['hadd '+rootfile+'.root '+rootfile+'_Signal'+'.root '+rootDir+'/'+channel+'_background_mc_uncer.root'],shell=True)
                            binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])    

                    print 'working on',rootfile+'.root'
                    #print signal_pre
		    #continue
		    #if width: sys.exit(0)
                    if limits and os.path.isfile(rootfile+"_rebinned.root"):
                        if 'X53' in signal_pre:
                            exp,obs = run_cutopt(rootfile+"_rebinned.root",chirality,channel,'X',True,"",0,prefix+width+'_')
                        else:
                            exp,obs = run_cutopt(rootfile+"_rebinned.root",chirality,channel,signal_pre+production,True,"",0,prefix+width+'_')
                        #sys.exit(0)

