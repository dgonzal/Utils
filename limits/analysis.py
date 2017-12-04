from subprocess import call

channels = [ 'Mu','Ele','']# 'Mu','Ele',''
productions = ['B','T'] #'B','T'
chiralities = ['RH','LH']

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/'
sourcedirs = 'config/MuSel_wtag_topjetcorr/'
rootDir = 'ROOTMC125/'
prefix = ''
createfiles = True
rebin = True 
limits = True
signal_injection = False
signal_pre = 'Bprime'
combine =False

background = "SingleT_s.root, SingleT_t.root, SingleTWAntitop.root, SingleTWtop.root, ZJets.root, TTbar.root, WJets_Pt.root, QCD.root" 


if not rebin:
    createfiles = False

if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)

execfile("histogram_rebinning.py")
execfile("calculation.py")

i = 0 
for channel in channels:
  for production in productions:
    for chirality in chiralities:
        dirstring = release+sourcedirs
        rootfile = rootDir+signal_pre+production+'_'+channel+'_'+chirality+"_MC"
        """
        i+=1
        print rootfile
        if i < 4:
            continue
        """
        #call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,dirstring,channel,"","","mc"])
        if not channel:
            dirstring = rootfile.replace('__','_*_')
            rootfile = rootfile.replace('__','_')
            #print return ootfile,dirstring
            #call(['rm '+rootfile+'.root '],shell=True)
            #call(['hadd '+rootfile+'.root '+dirstring+'.root'],shell=True)
        else:
            pass
            #call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,dirstring,channel,"","","mc"]) 

            
        binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt', 'SingleT_s','SingleT_t', 'SingleTWAntitop','SingleTWTop','TTbar','QCD'])    
        exp,obs = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)
        #sys.exit(0)
