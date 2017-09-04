

from subprocess import call
import os, sys
from matplotlib.backends.backend_pdf import PdfPages


prod_channels = ['Mu','Ele']# 'Mu','Ele',''

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
sourcedirs = ['/MuSel_wtag_topjetcorr/','/EleSel_cross/']
signal_src = ['MuSigSel','EleSigSel_v1']

rootDir = 'ROOTBackgroundFit/'
prefix =''

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"
#MC = "TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root"#,QCD.root"
background = 'Data.root,DATA.root' #"SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"


signal_prefix = 'Bprime' #'X53',
production = 'B' #,'T'] #'B','T']
chirality = 'RH' #,'LH'] #['RH','LH']
masses = ['700','800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800']
witdth = '' #,'10','20','30']
cross_sec = [4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.248,0.195]
cross_sec = [x*0.5 for x in cross_sec]

#cross_sec = [0.745,0.532,0.388,0.285,0.212,0.159,0.120,0.0917,0.0706,0.0541,0.042,0.324]

if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)


execfile("signal_background_uncertainty.py")    
execfile("backgroundfit.py")
execfile("injection_merge.py")
execfile("histogram_rebinning.py")


sigma1_color = 'gray' #"green"
x = [float(i) for i in masses]

for i,channel in enumerate(prod_channels):
    nominal_results = None
    mass_injection = []
  
    dirstring = release+sourcedirs[i]
    rootfile = rootDir+prefix+channel+'_background'
    call(['./../bin/rootfilecreator', background, rootfile,dirstring,channel,"","","DATA_central"])
    rootfile = rootDir+prefix+channel+'_MC_central'
    call(['./../bin/rootfilecreator', MC, rootfile,dirstring,channel,"","","central"])
    #rootfile = rootDir+prefix+channel+'_MC_central_sig'
    #call(['./../bin/rootfilecreator', signal_prefix+production+'-'+mass+'_'+chirality, rootfile,dirstring,channel,"","","central"])
    rootfile = rootDir+prefix+channel+'_MC_forward'
    call(['./../bin/rootfilecreator', MC, rootfile,dirstring,channel,"","","forward"])
    
    rootfile = rootDir+prefix+channel+'_MC'
    rootfileData = rootDir+prefix+channel+'_background.root'
    create_merged_hists(rootfile+'_central.root',rootfile+'_central_merged.root' , 'Background')
    create_merged_hists(rootfile+'_forward.root',rootfile+'_forward_merged.root' , 'DATA')
    call(['rm '+rootfile+'_merged.root'],shell=True)
    add_signal_background_uncer(rootfileData, rootfile+'_forward_merged.root', rootfile+'_central_merged.root', rootDir+prefix+channel+'_background_mc_uncer.root' )

    print 'hadd '+rootfile+'_merged.root '+rootfile+'_*_merged.root'
    call(['hadd '+rootfile+'_merged.root '+rootfile+'_*_merged.root'],shell=True)
    binFile(0.2, rootfile+'_merged.root', 'M_{B} [GeV/c^{2}]', ['Background'])
    nominal_results = background_fit(rootfile+'_merged_rebinned.root', channel, False, "")

    #sys.exit(0)
    continue
    

    for m,mass in enumerate(masses):
        print mass
        rootfile = rootDir+prefix+channel+'_MC_central_sig'
        call(['./../bin/rootfilecreator', signal_prefix+production+'-'+mass+'_'+chirality, rootfile,dirstring,channel,"","","central"])
        rootfile = rootDir+prefix+channel+'_MC'
        create_merged_hists(rootfile+'_central.root',rootfile+'_central_merged_'+mass+'.root' , 'Background')
        create_merged_hists(rootfile+'_forward.root',rootfile+'_forward_merged_'+mass+'.root' , 'DATA')
        signal = signal_prefix+production+'-'+mass+'_'+chirality
        add_signal(rootfile+'_central_merged_'+mass+'.root', 'Background', rootfile+'_central_sig.root' , signal, cross_sec[m])
        call(['rm '+rootfile+'_merged_'+mass+'.root'],shell=True)
        call(['hadd '+rootfile+'_merged_'+mass+'.root '+rootfile+'_*_merged_'+mass+'.root'],shell=True)
        binFile(0.2, rootfile+'_merged_'+mass+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
        mass_injection.append(background_fit(rootfile+'_merged_'+mass+'_rebinned.root', channel, False, ""))
       
    print nominal_results
    print mass_injection
    
    for m,res in enumerate(nominal_results):
        nominal_bands_up   = [res[1]+res[2]]*len(masses)
        nominal_bands_down = [res[1]-res[2]]*len(masses)

        

        pp = PdfPages('Signal_Injection_'+signal_prefix+'_'+production+'_'+chirality+'_'+channel+'_'+res[0]+'.pdf')
        plt.clf()
        plt.title("CMS $\it{Preliminary}$  "+res[0].replace('_',' '), fontsize=10)# , loc='right')

        ax=plt.gca()
        ax.set_ylim([.3,.80])
        ax.set_xlim([650,1850])
        y_injection =[]
        y_error =[]
        x_error =[]
        for p in xrange(len(masses)):
            y_injection.append(mass_injection[p][m][1])
            y_error.append(mass_injection[p][m][2])
            x_error.append(0)
            #plt.errorbar(x[p], mass_injection[p][m][1], yerr=mass_injection[p][m][2],xerr=0, ecolor='g', fmt='-o')
        plt.errorbar(x, y_injection, yerr=y_error,xerr=x_error, color='red', fmt='o', label='Fit with injected signal')
        #plt.plot(x,y_injection,color='g')
       
        """
        plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
        alpha=0.6, facecolor=sigma2_color, edgecolor=sigma2_color,
        linewidth=0, label="$\pm$ 2 std. deviation")
        """
        plt.fill_between(x,  nominal_bands_up, nominal_bands_down,
                         alpha=0.8, facecolor=sigma1_color, edgecolor=sigma1_color, # exp_LH.bands[1][2],
                         linewidth=0, label="nominal fit $\pm$ 1 std. deviation")
       

        plt.plot([],[],label="nominal fit with $\pm$ 1 std. deviation",color=sigma1_color,linewidth=10) 
        plt.legend(loc="upper center",prop={'size':12},frameon=False,numpoints=1) 
        #plt.plot(theory13TeV_x,upper,linestyle='-.',color='cyan')
        plt.xlabel('injected VLQ mass (GeV)')
        plt.ylabel('signal/control region ratio')
   
        plt.savefig(pp, format='pdf')
        pp.close()
