

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

rootDir = 'ROOT_Injection_z/'
prefix ='LH_'

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets*.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt*.root,QCD*.root"
#MC = "TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root"#,QCD.root"
#background = 'Data.root,DATA.root' #"SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"


signal_prefix = 'Bprime' #'X53',
production = 'B' #,'T'] #'B','T']
chirality = 'LH' #,'LH'] #['RH','LH']
masses = ['700','800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800']
witdth = '' #,'10','20','30']
cross_sec = [4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.248,0.195]
cross_sec = [x*0.25 for x in cross_sec]

#cross_sec = [1.5, 0.9, 0.4, 0.2, 0.12, 0.11, 0.08, 0.09, 0.08, 0.05, 0.05, 0.07]
create = False 



print cross_sec

exit(0)

if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)


execfile("signal_background_uncertainty.py")
execfile("invert_bin_content.py")
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

    if i == 1:continue

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
    nominal_results,resultfile = background_fit(rootfile_MC_merged_mc_unc_rebin, channel, False, "")
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
    
    #result_file = open(prefix+channel+'_half_injection_result_signal_CRSR_nosubstraction.txt','w+')
    result_file = open(prefix+channel+'_half_injection_result_signal.txt','w+')
    #result_file = open(prefix+channel+'_half_injection_result_signal_SR.txt','w+')
    print >> result_file, 'nominal result'
    print >> result_file, 'nominal exp'
    print >> result_file, nom_exp
    print >> result_file, 'nominal obs'
    print >> result_file, nom_obs    
    
    for m,mass in enumerate(masses):
 
	#if m ==2 or m==:continue 
        #signal to inject
        signal = signal_prefix+production+'-'+mass+'_'+chirality
        print 'Injecting ', signal, ' with cross section',  cross_sec[m]
        
        #merge this histograms 
        central_mass_h_merged = rootfile_MC_central.replace('.root','_'+mass+'_h_merged.root')
        forward_mass_h_merged = rootfile_MC_forward.replace('.root','_'+mass+'_h_merged.root')
        if os.path.exists(central_mass_h_merged):
            print 'removing file',central_mass_h_merged
            os.remove(central_mass_h_merged)
        if os.path.exists(forward_mass_h_merged):
            print 'removing file',forward_mass_h_merged
            os.remove(forward_mass_h_merged)    

        create_merged_hists(rootfile_MC_central, central_mass_h_merged, 'Background')
        create_merged_hists(rootfile_MC_forward, forward_mass_h_merged, 'DATA')

        #rootfile_MC_central = rename_process(rootfile_MC_central,'central','')
        #rootfile_MC_forward = rename_process(rootfile_MC_forward,'forward','')
        
        #create signal histos to add to the CR & SR
        central_sig = rootDir+prefix+channel+'_MC_central_sig_'+mass+'.root'
        forward_sig = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'.root'
        #forward_sig_unc = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'_unc.root'

        print 'Creating signal histograms for mass',mass
        #call(['./../bin/rootfilecreator', signal, central_sig, dirstring,channel,"","","central"])
        #call(['./../bin/rootfilecreator', signal, forward_sig, dirstring,channel,"","","forward"])

        #add signal
        print '*'*10
        print signal,'files produced'
        central_signal = rename_process(central_sig,'central','')
        forward_signal = rename_process(forward_sig,'forward','')
        
        add_signal(central_mass_h_merged, 'Background', central_signal, signal, cross_sec[m])
        add_signal(forward_mass_h_merged, 'DATA'      , forward_signal, signal, cross_sec[m])

        #merge central and forward region
        mass_merged = central_mass_h_merged.replace('_central','')
        call(['rm '+mass_merged],shell=True)        
        call(['hadd '+mass_merged+' '+central_mass_h_merged+' '+forward_mass_h_merged],shell=True)

        mass_merged_rebin     = simpleRebin(mass_merged,unc,['Background','Data'],mass_merged.replace('.root','_rebinned.root'),['Background','Data'])
        mass_merged_rebin_unc = mass_merged_rebin.replace('.root','_unc.root')
        add_signal_background_uncer(mass_merged_rebin, rootfile_MC_forward_merged, rootfile_MC_central_merged, mass_merged_rebin_unc,True)

        #calculate the SR/CR fit
        print '#'*5
        mass_injection.append(background_fit(mass_merged_rebin_unc, channel, False, "")[0])#,signal))
        print '*'*5
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print '#'*5
        
        mass_merged_signalfit = mass_merged.replace('.root','_signalfit.root')
       
        central_signal = rename_process(central_signal,signal.replace('-','_'),"Signal_central_"+mass)
        forward_signal = rename_process(forward_signal,signal.replace('-','_'),"Signal_forward_"+mass)
        inverted_central_signal = invert_bin_content(central_signal,"Signal_central_"+mass,'',-1)
        
        call(['rm '+mass_merged_signalfit],shell=True)
        call(['hadd '+mass_merged_signalfit+' '+mass_merged_rebin_unc+' '+forward_signal+' '+inverted_central_signal],shell=True)

        #scale hists according to the background mle fit 
        scale_hists(mass_merged_signalfit, 'Background',mass_injection[-1])#nominal_results)
        scale_hists(mass_merged_signalfit, 'Signal_central_'+mass,mass_injection[-1])#nominal_results)
        signal_rebin(mass_merged_signalfit,'Signal_forward_'+mass)
        signal_rebin(mass_merged_signalfit,'Signal_central_'+mass)
        sys_reweight(mass_merged_signalfit,'PDF')
        sys_reweight(mass_merged_signalfit,'scale')
        #fitted_signal =0
        fitted_signal = best_signalfit(mass_merged_signalfit,mass) 
	print '\033[92m'
        print 'injected cross section:',cross_sec[m],'fitted cross section:',fitted_signal
        print '\033[0m'
        #continue
        #signal_rebin(inverted_central_signal,'Signal_central')
        
        #continue
        
        forward_sig_err = forward_sig.replace('.root','_err.root')
        #call(['./../bin/rootfilecreator', signal, forward_sig_err, dirstring,channel,"","","forward_errors"])
        forward_signal_err = rename_process(forward_sig_err,'forward','')
        forward_signal_err = rename_process(forward_signal,signal.replace('-','_'),"Signal_forward_"+mass)
        
        mass_merged_z = mass_merged_signalfit.replace('.root','_SRCR.root')
        call(['rm '+mass_merged_z],shell=True)
        call(['hadd '+mass_merged_z+' '+mass_merged_rebin_unc+' '+forward_signal_err],shell=True)
        scale_hists(mass_merged_z, 'Background',mass_injection[-1])#nominal_results)
        signal_rebin(mass_merged_z,'Signal_forward')
        sys_reweight(mass_merged_z,'PDF')
        sys_reweight(mass_merged_z,'scale')

        #calculate zscore
        inj_exp,inj_obs, zvalue_nosub = injected_signal_mc_limits(mass_merged_z, chirality, channel, signal_prefix+production , False, mass+"_injected",mass)
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print 'expected limit ratio for injected mass',mass

        print >> result_file, "++"*10
        print >> result_file, 'injected result', signal, ' with cross section',  cross_sec[m]
        print >> result_file, 'no signal substraction'
        print >> result_file, 'scale values'
        print >> result_file, mass_injection[-1]
        print >> result_file, 'fitted cross section',fitted_signal
        print >> result_file, 'z values'
        print >> result_file, zvalue_nosub
        
        mass_merged_z = mass_merged_signalfit.replace('.root','_signalSub.root')
        call(['rm '+mass_merged_z],shell=True)
        call(['hadd '+mass_merged_z+' '+mass_merged_rebin_unc+' '+forward_signal_err],shell=True)
        add_signal( mass_merged_z, 'Background', inverted_central_signal, 'Signal_central_'+mass,fitted_signal[0])
        scale_hists(mass_merged_z, 'Background',mass_injection[-1])#nominal_results)
        signal_rebin(mass_merged_z,'Signal_forward')
        sys_reweight(mass_merged_z,'PDF')
        sys_reweight(mass_merged_z,'scale')
        add_substraction_unc(mass_merged_z,central_signal,'Signal_central_'+mass,fitted_signal)
        
        #print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
                 
        #calculate zscore
        inj_exp,inj_obs, zvalue_sub = injected_signal_mc_limits(mass_merged_z, chirality, channel, signal_prefix+production , False, mass+"_injected",mass)
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print 'expected limit ratio for injected mass',mass
    
        #print >> result_file, "++"*10
        #print >> result_file, 'injected result', signal, ' with cross section',  cross_sec[m]
        #print >> result_file,'scale values'
        #print >> result_file, mass_injection[-1]
        print >> result_file, "with signal substraction"
        print >> result_file, 'fitted cross section',fitted_signal
        print >> result_file, 'z values'
        print >> result_file, zvalue_sub
    
        mass_merged_z = mass_merged_signalfit.replace('.root','_SR.root')
        call(['rm '+mass_merged_z],shell=True)
        call(['hadd '+mass_merged_z+' '+mass_merged_rebin_unc+' '+forward_signal_err],shell=True)
        add_signal( mass_merged_z, 'Background', inverted_central_signal, 'Signal_central_'+mass,cross_sec[m])
        scale_hists(mass_merged_z, 'Background',mass_injection[-1])#nominal_results)
        signal_rebin(mass_merged_z,'Signal_forward')
        sys_reweight(mass_merged_z,'PDF')
        sys_reweight(mass_merged_z,'scale')

        #calculate zscore
        inj_exp,inj_obs, zvalue_SR = injected_signal_mc_limits(mass_merged_z, chirality, channel, signal_prefix+production , False, mass+"_injected",mass)
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print 'expected limit ratio for injected mass',mass
        
        #print >> result_file, "++"*10
        #print >> result_file, 'injected result', signal, ' with cross section',  cross_sec[m],'SR only'
        #print >> result_file,'scale values'
        #print >> result_file, mass_injection[-1]
        print >> result_file, 'signal region only'
        print >> result_file, 'fitted cross section',fitted_signal
        print >> result_file, 'z values'
        print >> result_file, zvalue_SR
 
