

from subprocess import call
import os, sys
from matplotlib.backends.backend_pdf import PdfPages
import glob

prod_channels = ['Mu','Ele']# 'Mu','Ele',''

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
sourcedirs = ['/MuSel_new/','/EleSel_new/'] #'/MuSel_wtag_topjetcorr/'

prod_channels = ['Ele']# 'Mu','Ele',''
sourcedirs = ['/EleSel_new/']

signal_src = ['MuSigSel','EleSigSel_v1']

rootDir = 'ROOTBackgroundFit/'
prefix =''

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets*.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt*.root,QCD*.root"
#MC = "TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root"#,QCD.root"
background = 'PhotonData.root' #"SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"


signal_prefix = 'Bprime' #'X53',
production = 'T' #,'T'] #'B','T']
chirality = 'RH' #,'LH'] #['RH','LH']
masses = ['700','800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800']
witdth = '' #,'10','20','30']
cross_sec = [4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.248,0.195]
cross_sec = [x*0.5 for x in cross_sec]

cross_sec = [0.745,0.532,0.388,0.285,0.212,0.159,0.120,0.0917,0.0706,0.0541,0.042,0.0324]

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
    if '*' in MC:
        sample_list = ''
        for item in MC.split(','):
            if '*' in item:
                for name in glob.glob(dirstring+'*'+item):
                    sample_list+=name.split('.')[-2]+'.root,'
            else:
               sample_list+=item+','
        MC = sample_list[:-1]
        
    rootfile = rootDir+prefix+channel+'_background'
    #call(['./../bin/rootfilecreator', background, rootfile,dirstring,channel,"","","DATA_central"])
    #call(['./../bin/rootfilecreator', background, rootfile,dirstring,channel,"","",""])
    #continue 
    #rootfile = rootDir+prefix+channel+'_MC_central'
    #call(['./../bin/rootfilecreator', MC, rootfile,dirstring,channel,"","","central"])
    ##rootfile = rootDir+prefix+channel+'_MC_central_sig'
    ##call(['./../bin/rootfilecreator', signal_prefix+production+'-'+mass+'_'+chirality, rootfile,dirstring,channel,"","","central"])
    #rootfile = rootDir+prefix+channel+'_MC_forward'
    #call(['./../bin/rootfilecreator', MC, rootfile,dirstring,channel,"","","forward"])
 
    rootfile = rootDir+prefix+channel+'_MC'
    rootfileData = rootDir+prefix+channel+'_background.root'
    create_merged_hists(rootfile+'_central.root',rootfile+'_central_merged.root' , 'Background')
    create_merged_hists(rootfile+'_forward.root',rootfile+'_forward_merged.root' , 'DATA')
    
    call(['rm '+rootfile+'_merged.root'],shell=True) 
    print 'hadd '+rootfile+'_merged.root '+rootfile+'_*_merged.root'
    call(['hadd '+rootfile+'_merged.root '+rootfile+'_*_merged.root'],shell=True)
    call(['rm '+rootfile+'_merged_mc_unc.root'],shell=True)
    call(['hadd '+rootfile+'_merged_mc_unc.root '+rootfile+'_*_merged.root'],shell=True)
    
    add_signal_background_uncer(rootfile+'_merged.root', rootfile+'_forward_merged.root', rootfile+'_central_merged.root', rootfile+'_merged_mc_unc.root',True )
    add_signal_background_uncer(rootDir+prefix+channel+'_background.root', rootfile+'_forward_merged.root', rootfile+'_central_merged.root',  rootfileData.replace('.root','_unc.root'),True )
    

    stat_uncer = 0.15
    #binFile(stat_uncer, rootfile+'_merged_mc_unc.root', 'M_{B} [GeV/c^{2}]', ['Background'])
    #print binFile(stat_uncer, rootfileData.replace('.root','_unc.root'), 'M_{B} [GeV/c^{2}]', ['DATA'])
    #print binFile(stat_uncer, rootfileData.replace('.root','_unc_rebinned.root'), 'M_{B} [GeV/c^{2}]', ['Background'])
    #print binFile(stat_uncer, rootfileData, 'M_{B} [GeV/c^{2}]', ['Background'])
    #print binFile(stat_uncer, rootfileData.replace('.root','_rebinned.root'), 'M_{B} [GeV/c^{2}]', ['DATA'])

    print 'Rebinned sample', simpleRebin(rootfileData,stat_uncer,['Background'])
    add_signal_background_uncer(rootDir+prefix+channel+'_background_rebinned.root', rootfile+'_forward_merged.root', rootfile+'_central_merged.root',  rootfileData.replace('.root','_unc_rebinned.root'),True )

    workfile = simpleRebin(rootfileData.replace('.root','_unc_rebinned.root'),stat_uncer,['Background'],rootfileData.replace('.root','_unc_final_rebinned.root'),['Background'])
    print workfile

    print '*'*10
    print '*'*10
    print "nominal results"
    #nominal_results = background_fit(rootfile+'_merged_mc_unc_rebinned.root', channel, False, "")
    nominal_results = background_fit(rootfileData.replace('.root','_unc_final_rebinned.root'), channel, False, "")
    
    print nominal_results    
    print '*'*10
    print '*'*10
    #sys.exit(0)
    #continue
    print '+'*10

    
    scale_hists(rootfileData.replace('.root','_unc_rebinned.root'), 'Background', nominal_results)
        
    continue

    print dirstring+'/*'+signal_prefix+production+'*'+chirality+'.root'
    signal_files=find_files(dirstring+'/*'+signal_prefix+production+'*'+chirality+'.root')
    signal_rootfile = rootDir+prefix+channel+'_MC_forward_signal'
    
    call(['./../bin/rootfilecreator', signal_files, signal_rootfile,dirstring,channel,"","",""])
    
    call(['rm '+rootfile+'_nominal_with_signal_mc_unc.root'],shell=True)
    call(['rm '+rootfile+'_nominal_with_signal_mc_unc_scale.root'],shell=True)

    #Use background region MC also in the signal region 
    #create_merged_hists(rootfile+'_central.root',rootfile+'_central_merged_data.root' , 'DATA')
    #scale_hists(rootfile+'_central_merged_data.root', 'DATA', nominal_results)
    #add_signal_background_uncer(rootfile+'_merged.root', rootfile+'_forward_merged.root', rootfile+'_central_merged.root', rootfile+'_merged_mc_unc.root',True )
    
    call(['hadd '+rootfile+'_nominal_with_signal_mc_unc.root '+rootfile+'_merged_mc_unc.root '+signal_rootfile+'.root'],shell=True)
    call(['hadd '+rootfile+'_nominal_with_signal_mc_unc_scale.root '+rootfile+'_merged_mc_unc.root '+signal_rootfile+'.root'],shell=True)

    scale_hists(rootfile+'_nominal_with_signal_mc_unc_scale.root', 'Background', nominal_results)
   
    binFile(0.2, rootfile+'_nominal_with_signal_mc_unc_scale.root', 'M_{B} [GeV/c^{2}]', ['Background'])
    
    nom_exp,nom_obs = injected_signal_mc_limits(rootfile+'_nominal_with_signal_mc_unc_scale_rebinned.root', chirality, channel, signal_prefix+production , False, "nominal")
    
    nominal_file = rootfile+'_nominal_with_signal_mc_unc.root'
    
    injected_mass_cross_section_expected = []
    injected_mass_cross_section_observed = []
    
    for m,mass in enumerate(masses):
        print 'Injecting ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        rootfile = rootDir+prefix+channel+'_MC_central_sig'
        call(['./../bin/rootfilecreator', signal_prefix+production+'-'+mass+'_'+chirality, rootfile,dirstring,channel,"","","central"])
        rootfile = rootDir+prefix+channel+'_MC_forward_sig'
        call(['./../bin/rootfilecreator', signal_prefix+production+'-'+mass+'_'+chirality, rootfile,dirstring,channel,"","","forward"])
        rootfile = rootDir+prefix+channel+'_MC'
        create_merged_hists(rootfile+'_central.root',rootfile+'_central_merged_'+mass+'.root' , 'Background')
        create_merged_hists(rootfile+'_forward.root',rootfile+'_forward_merged_'+mass+'.root' , 'DATA')
        signal = signal_prefix+production+'-'+mass+'_'+chirality
        add_signal(rootfile+'_central_merged_'+mass+'.root', 'Background', rootfile+'_central_sig.root' , signal, cross_sec[m])
        add_signal(rootfile+'_forward_merged_'+mass+'.root', 'DATA', rootfile+'_forward_sig.root' , signal, cross_sec[m])
        call(['rm '+rootfile+'_merged_'+mass+'.root'],shell=True)
        call(['hadd '+rootfile+'_merged_'+mass+'.root '+rootfile+'_*_merged_'+mass+'.root'],shell=True)
        binFile(0.2, rootfile+'_merged_'+mass+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
        mass_injection.append(background_fit(rootfile+'_merged_'+mass+'_rebinned.root', channel, False, ""))
      
        injected_file= nominal_file.replace('/','/'+mass+'_')
        
        call(['cp '+nominal_file+' '+injected_file],shell=True)

        
        add_signal(injected_file,'DATA', rootfile+'_forward_sig.root' , signal, cross_sec[m])
        add_signal(injected_file,'Background', rootfile+'_forward_sig.root' , signal, cross_sec[m])
        #add_signal(injected_file, 'Background', rootfile+'_central_sig.root' , signal, cross_sec[m])
        scale_hists(injected_file, 'Background',  mass_injection[-1])
        #add_signal(injected_file,'Background', rootfile+'_forward_sig.root' , signal, cross_sec[m])
        binFile(0.2, injected_file, 'M_{B} [GeV/c^{2}]', ['Background'])
        
        rebinned_file = injected_file.replace('.root','_rebinned.root')
        print 'Injecting ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print 'limit calculated for',rebinned_file
        inj_exp,inj_obs = injected_signal_mc_limits(rebinned_file, chirality, channel, signal_prefix+production , False, mass+"_injected")
        print 'expected limit ratio for injected mass',mass
        for i in xrange(len(nom_exp.y)):
            if i == m: injected_mass_cross_section_expected.append(inj_exp.y[i]/nom_exp.y[i]) 
            #print 'Mass',nom_exp.x[i], 'nominal', nom_exp.y[i],'+-',nom_exp.yerrors[i], 'injection',inj_exp.y[i],'+-',inj_exp.yerrors[i], 'ratio', inj_exp.y[i]/nom_exp.y[i]
            print 'Mass',nom_exp.x[i], 'nom/inj', nom_exp.y[i], ' / ',inj_exp.y[i], ' = ', inj_exp.y[i]/nom_exp.y[i]
        print 'obs limit ratio for injected mass',mass
        for i in xrange(len(nom_obs.y)):
            if i == m: injected_mass_cross_section_observed.append(inj_obs.y[i]/cross_sec[m]) 
            print 'Mass',nom_obs.x[i], 'nom/inj', nom_obs.y[i], ' / ',inj_obs.y[i], ' = ', inj_obs.y[i]/nom_obs.y[i]
 
    print nominal_results
    print mass_injection

    create_injectionratio_plots("injection_back_"+channel+".pdf", nom_exp.x, injected_mass_cross_section_expected, 'cross section before/after injection', 'ratio of nominal CL/injected CL on MC' )
    create_injectionratio_plots("injection_signal_"+channel+".pdf", nom_obs.x, injected_mass_cross_section_observed,' injected/expected cross section ','ratio ')


    continue

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




                    
    
