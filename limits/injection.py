

from subprocess import call
import os, sys
from matplotlib.backends.backend_pdf import PdfPages
import glob
from shutil import copyfile

prod_channels = ['Mu','Ele']# 'Mu','Ele',''

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
sourcedirs = ['/MuSel_new/','/EleSel_new/'] #'/MuSel_wtag_topjetcorr/'

#prod_channels = ['Ele']# 'Mu','Ele',''
#sourcedirs = ['/EleSel_cross/']

signal_src = ['MuSigSel','EleSigSel_v1']

rootDir = 'ROOT_Injection/'
prefix =''

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets*.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt*.root,QCD*.root"
#MC = "TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root"#,QCD.root"
background = 'Data.root,DATA.root' #"SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"


signal_prefix = 'Bprime' #'X53',
production = 'B' #,'T'] #'B','T']
chirality = 'RH' #,'LH'] #['RH','LH']
masses = ['700','800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800']
witdth = '' #,'10','20','30']
cross_sec = [4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.248,0.195]
cross_sec = [x*0.5 for x in cross_sec]

cross_sec = [1.5, 0.9, 0.4, 0.2, 0.12, 0.11, 0.08, 0.09, 0.08, 0.05, 0.05, 0.07]
create = False 


if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)


execfile("signal_background_uncertainty.py")    
execfile("backgroundfit.py")
execfile("injection_merge.py")
#execfile("histogram_rebinning.py")


sigma1_color = 'gray' #"green"
x = [float(i) for i in masses]

for i,channel in enumerate(prod_channels):
    nominal_results = None
    mass_injection = []
    back_signal_fit =[]
    
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
    
    rootfile_MC_central = rootDir+prefix+channel+'_MC_central.root'
    rootfile_MC_forward = rootDir+prefix+channel+'_MC_forward.root'

    # create rootfile with hists for theta
    if create:
      call(['./../bin/rootfilecreator', MC, rootfile_MC_central,dirstring,channel,"","","central"])
      call(['./../bin/rootfilecreator', MC, rootfile_MC_forward,dirstring,channel,"","","forward"])

    #merge histos into one
    rootfile_MC_central_merged = rootfile_MC_central.replace('.root','_h_merged.root')
    rootfile_MC_forward_merged = rootfile_MC_forward.replace('.root','_h_merged.root')
    create_merged_hists(rootfile_MC_central, rootfile_MC_central_merged, 'Background')
    create_merged_hists(rootfile_MC_forward, rootfile_MC_forward_merged, 'DATA')


    #merge root files into one 
    rootfile_MC_merged = rootDir+prefix+channel+'_merged.root'
    call(['rm '+rootfile_MC_merged],shell=True) 
    call(['hadd '+rootfile_MC_merged+' '+rootfile_MC_central_merged+' '+rootfile_MC_forward_merged],shell=True)

    #add mc region uncertainty
    rootfile_MC_merged_mc_unc = rootfile_MC_merged.replace('.root','_mc_unc.root')
    call(['rm '+rootfile_MC_merged_mc_unc],shell=True)
    unc = 0.2
    first_rebin = simpleRebin(rootfile_MC_merged, unc,['Background'],'first_rebin.root',['Background','DATA'])    
    add_signal_background_uncer(first_rebin, rootfile_MC_forward_merged, rootfile_MC_central_merged, rootfile_MC_merged_mc_unc,True)
    #rootfile_MC_merged_mc_unc = first_rebin
    rootfile_MC_merged_mc_unc_rebin =  simpleRebin(rootfile_MC_merged_mc_unc, unc, ['Background'],rootfile_MC_merged_mc_unc.replace('.root','_rebinned.root'),['Background','DATA'])    


    #calculate normalisation
    print '*'*10
    print '*'*10
    print "nominal results"
    nominal_results = background_fit(rootfile_MC_merged_mc_unc_rebin, channel, False, "")
    print nominal_results    
    print '*'*10
    print '*'*10
    #continue
    print '+'*10
      
    signal_files = find_files(dirstring+'/*'+signal_prefix+production+'*'+chirality+'.root')
    signal_rootfile = rootDir+prefix+channel+'_MC_forward_signal.root'

    if create:
      call(['./../bin/rootfilecreator', signal_files, signal_rootfile,dirstring,channel,"","",""])
    
    model_file = rootfile+'_model_file.root'
    model_file_scale = model_file.replace('.root','_scale.root')
    call(['rm '+model_file],shell=True)
    call(['rm '+model_file_scale],shell=True)

    call(['hadd '+model_file+' '+rootfile_MC_merged_mc_unc_rebin+' '+signal_rootfile],shell=True)
    call(['hadd '+model_file_scale+' '+model_file],shell=True)

    #model_file_rebin = simpleRebin(model_file, unc,['Background'],model_file.replace('.root','_rebinned.root'),['All']) 
    scale_hists(model_file_scale, 'Background', nominal_results)
    signal_rebin(model_file_scale,signal_prefix)
    sys_reweight(model_file_scale,'PDF')
    sys_reweight(model_file_scale,'scale')
    #model_file_rebinned = binFile(0.2, model_file_scale, 'M_{B} [GeV/c^{2}]', ['Background'])
    nom_exp,nom_obs, zvalue = injected_signal_mc_limits(model_file_scale, chirality, channel, signal_prefix+production , True)#, "nominal")

    #continue
    #exit(0)
    injected_mass_cross_section_expected = []
    injected_mass_cross_section_observed = []

    result_file = open(channel+'_injection_result.txt','w+')
    print >> result_file, 'nominal result'
    print >> result_file, 'nominal exp'
    print >> result_file, nom_exp
    print >> result_file, 'nominal obs'
    print >> result_file, nom_obs    
    
    for m,mass in enumerate(masses):
        #signal to inject
        signal = signal_prefix+production+'-'+mass+'_'+chirality
        print 'Injecting ', signal, ' with cross section',  cross_sec[m]
        
        #merge this histograms 
        central_mass_h_merged = rootfile_MC_central.replace('.root','_'+mass+'_h_merged.root')
        forward_mass_h_merged = rootfile_MC_forward.replace('.root','_'+mass+'_h_merged.root')
        create_merged_hists(rootfile_MC_central, central_mass_h_merged, 'Background')
        create_merged_hists(rootfile_MC_forward, forward_mass_h_merged, 'DATA')

        central_mass_h_merged_mc_unc_rebinned = central_mass_h_merged.replace('.root','_mc_unc_rebinned.root')
        region_unc_input = simpleRebin(central_mass_h_merged,unc,['Background'],central_mass_h_merged.replace('.root','_rebinned.root'),['All'])
        add_signal_background_uncer(region_unc_input, rootfile_MC_forward_merged, rootfile_MC_central_merged, central_mass_h_merged_mc_unc_rebinned,True)
        
        #create signal histos to add to the CR & SR
        central_sig = rootDir+prefix+channel+'_MC_central_sig_'+mass+'.root'
        forward_sig = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'.root'
        #forward_sig_unc = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'_unc.root'
        call(['./../bin/rootfilecreator', signal, central_sig, dirstring,channel,"","","central"])
        call(['./../bin/rootfilecreator', signal, forward_sig, dirstring,channel,"","","forward"])
        #call(['./../bin/rootfilecreator', signal, forward_sig_unc, dirstring,channel,"","",""])
        #call(['./../bin/rootfilecreator', signal, forward_sig.replace(mass,masses[m+1]), dirstring,channel,"","","forward"])

        #add signal 
        add_signal(central_mass_h_merged_mc_unc_rebinned, 'Background', central_sig, signal, cross_sec[m])
        add_signal(forward_mass_h_merged,                 'DATA'      , forward_sig, signal, cross_sec[m])
       
        #exit(0) 
        #merge central and forward region
        mass_merged = central_mass_h_merged.replace('_central','')
        backPlusSignalFile = mass_merged.replace('.root','_backSig.root')
        call(['rm '+mass_merged],shell=True)
        call(['rm '+backPlusSignalFile],shell=True)
        
        call(['hadd '+backPlusSignalFile+' '+central_mass_h_merged_mc_unc_rebinned+' '+forward_mass_h_merged+' '+forward_sig],shell=True)
        call(['hadd '+mass_merged+' '+central_mass_h_merged_mc_unc_rebinned+' '+forward_mass_h_merged],shell=True)
        
        #rebin with 20% stat unc.
        mass_merged_rebinned = simpleRebin(mass_merged, unc, ['Background'],mass_merged.replace('.root','_rebinned.root'),['All'])
        backPlusSignalFile_rebinned = simpleRebin(backPlusSignalFile, unc, ['Background'],backPlusSignalFile.replace('.root','_rebinned.root'),['All'])
        #calculate the SR/CR fit
        print '#'*5
        print 'file for background fit',mass_merged_rebinned
        mass_injection.append(background_fit(mass_merged_rebinned, channel, False, ""))#,signal))
        print '*'*5
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        #back_signal_fit.append(background_fit(backPlusSignalFile_rebinned, channel, False, "",signal))
        print '#'*5

        mass_merged_limit = mass_merged.replace('.root','_limit.root')
        call(['rm '+mass_merged_limit],shell=True)
        call(['hadd '+mass_merged_limit+' '+mass_merged_rebinned+' '+signal_rootfile],shell=True)

        #scale hists according to the background mle fit 
        #scale_hists(mass_merged_limit, 'Background',  mass_injection[-1])
        scale_hists(mass_merged_limit, 'Background',mass_injection[-1])#nominal_results)
        signal_rebin(mass_merged_limit,signal_prefix)
        sys_reweight(mass_merged_limit,'PDF')
        sys_reweight(mass_merged_limit,'scale')
        
        #calculate limits
        inj_exp,inj_obs, zvalue = injected_signal_mc_limits(mass_merged_limit, chirality, channel, signal_prefix+production , False)#, mass+"_injected")
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print 'expected limit ratio for injected mass',mass

        bands = list(inj_exp.bands[1])
	print bands[0]
	print bands[1]
        for i in xrange(len(nom_exp.y)):
	    error = bands[1][i]-inj_exp.y[i]
 	    if (inj_exp.y[i]-nom_exp.y[i]) < 0: error = bands[0][i]-inj_exp.y[i]
            #if i == m: injected_mass_cross_section_expected.append((inj_exp.y[i]-nom_exp.y[i])/math.sqrt(bands[0][i]*bands[0][i]+bands[1][i]*bands[1][i])) 
            if i == m: injected_mass_cross_section_expected.append((inj_exp.y[i]-nom_exp.y[i])/math.sqrt(error*error)) 
            #print 'Mass',nom_exp.x[i], 'nominal', nom_exp.y[i],'+-',nom_exp.yerrors[i], 'injection',inj_exp.y[i],'+-',inj_exp.yerrors[i], 'ratio', inj_exp.y[i]/nom_exp.y[i]
            print 'Mass',nom_exp.x[i], 'nom/inj', nom_exp.y[i], ' / ',inj_exp.y[i], ' = ', inj_exp.y[i]/nom_exp.y[i]
        print 'obs limit ratio for injected mass',mass
        for i in xrange(len(nom_obs.y)):
            if i == m: injected_mass_cross_section_observed.append(inj_obs.y[i]-cross_sec[m]) 
            print 'Mass',nom_obs.x[i], 'nom/inj', nom_obs.y[i], ' / ',inj_obs.y[i], ' = ', inj_obs.y[i]/nom_obs.y[i]


        print >> result_file, "++"*10
        print >> result_file, 'injected result', signal, ' with cross section',  cross_sec[m]
        print >> result_file, 'injected exp'
        print >> result_file, inj_exp
        print >> result_file, 'injected obs'
        print >> result_file, inj_obs
        print >> result_file, 'z values'
        print zvalue
            
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




                    
    
