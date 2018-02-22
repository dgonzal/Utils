

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

rootDir = 'ROOT_DATASideband_ele005/' #DATABlinded/
prefix =''

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets*.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt*.root,QCD*.root"
#MC = "TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root"#,QCD.root"
#background = 'Data.root,DATA.root' #"SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"
DATA = 'Merge_Data.root'

signal_prefix = 'Bprime' #'X53',
production = 'B' #,'T'] #'B','T']
chirality = 'LH' #,'LH'] #['RH','LH']
masses = ['700','800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800']
witdth = '' #,'10','20','30']
cross_sec = [4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.248,0.195]
cross_sec = [x*0.5 for x in cross_sec]


output_values='_fitted_signal_data_sideband.txt'


#cross_sec = [1.5, 0.9, 0.4, 0.2, 0.12, 0.11, 0.08, 0.09, 0.08, 0.05, 0.05, 0.07]
create = False 

blind = False 

print cross_sec

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
    signal_fit=[]
    signal_fit_err=[]
    
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
    rootfile_data = rootDir+prefix+channel+'_data.root'
    
    
    if i == 0:continue

    # create rootfile with hists for theta
    if create:
      call(['./../bin/rootfilecreator', MC, rootfile_MC_central,dirstring,channel,"","","central"])
      call(['./../bin/rootfilecreator', MC, rootfile_MC_forward,dirstring,channel,"","","forward"])
      if blind: call(['./../bin/rootfilecreator', DATA, rootfile_data,dirstring,channel,"","","DATA_central"])
      else: call(['./../bin/rootfilecreator', DATA, rootfile_data,dirstring,channel,"","",""])
    #merge histos into one
    rootfile_MC_central_merged = rootfile_MC_central.replace('.root','_h_merged.root')
    rootfile_MC_forward_merged = rootfile_MC_forward.replace('.root','_h_merged.root')
    create_merged_hists(rootfile_MC_central, rootfile_MC_central_merged, 'Background')
    create_merged_hists(rootfile_MC_forward, rootfile_MC_forward_merged, 'DATA')


    # blinding part
    if blind:
        print 'doing the blind analysis where the CR is also used for the SR'
        signal_region_data = rootfile_data.replace('.root','_pseudo.root ')
        scale_numbers = [('Anti-b-tag_rate', 0.23482468021707187, 0.04941860742118931), ('1-b-tag_rate', 0.252859306612846, 0.061548437257207045), ('W-tag_rate', 0.14808373799084665, 0.06869228328108234), ('2-b-tag_rate', 0.2817799964046489, 0.07399835429932677),('top-tag_rate', 0.35274197958677067, 0.08424110129534856)]
        call(['rm '+signal_region_data],shell=True)
        call(['hadd '+signal_region_data+' '+rootfile_data],shell=True)
        #print copyfile(rootfile_data, signal_region_data)
        #print 'copy',rootfile_data,signal_region_data
        #exit(0)
        rename_hists(signal_region_data,'Background','DATA')
        scale_hists(signal_region_data, 'DATA', scale_numbers)
        set_sqrt_errors(signal_region_data)
	randomize_hists(signal_region_data,'DATA')
	#exit(0)
        rootfile_data_tmp = rootfile_data
        rootfile_data = rootDir+prefix+channel+'_data_blinded.root'
        
        if os.path.exists(rootfile_data):
            print 'removing file',rootfile_data
            os.remove(rootfile_data)
        call(['hadd '+rootfile_data+' '+rootfile_data_tmp+' '+signal_region_data],shell=True)
    
    #add mc region uncertainty
    rootfile_data_mc_unc = rootfile_data.replace('.root','_mc_unc.root')
    call(['rm '+rootfile_data_mc_unc],shell=True)
    unc = 0.2
    first_rebin = simpleRebin(rootfile_data, unc,['Background'],'first_rebin.root',['Background','DATA'])    
    add_signal_background_uncer(first_rebin, rootfile_MC_forward_merged, rootfile_MC_central_merged, rootfile_data_mc_unc,True)
    #rootfile_MC_merged_mc_unc = first_rebin
    rootfile_data_mc_unc_rebin =  simpleRebin(rootfile_data_mc_unc, unc, ['Background'],rootfile_data_mc_unc.replace('.root','_rebinned.root'),['Background','DATA'])    


    #calculate normalisation
    print '*'*10
    print '*'*10
    print "nominal results"
    nominal_results,resultfile = background_fit(rootfile_data_mc_unc_rebin, channel, False, "")
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

    call(['hadd '+model_file+' '+rootfile_data_mc_unc_rebin+' '+signal_rootfile],shell=True)
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

    result_file = open(prefix+channel+'_'+output_values,'w+')
    print >> result_file, 'nominal result'
    print >> result_file, 'nominal exp'
    print >> result_file, nom_exp
    print >> result_file, 'nominal obs'
    print >> result_file, nom_obs
    print >> result_file, 'nominal results for skale factors'
    print >> result_file, nominal_results
    
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


        data_h_merged = model_file.replace('.root','_'+mass+'.root')
        call(['rm '+data_h_merged],shell=True)
        call(['hadd '+data_h_merged+' '+rootfile_data],shell=True)
        
        #create signal histos to add to the CR & SR
        central_sig = rootDir+prefix+channel+'_MC_central_sig_'+mass+'.root'
        forward_sig = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'.root'
        #forward_sig_unc = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'_unc.root'

        print 'Creating signal histograms for mass',mass
        call(['./../bin/rootfilecreator', signal, central_sig, dirstring,channel,"","","central"])
        call(['./../bin/rootfilecreator', signal, forward_sig, dirstring,channel,"","","forward"])

        #add signal
        print '*'*10
        print signal,'files produced'
        central_signal = rename_process(central_sig,'central','')
        forward_signal = rename_process(forward_sig,'forward','')
        
        add_signal(data_h_merged, 'Background', central_signal, signal, cross_sec[m])
        add_signal(data_h_merged, 'DATA'      , forward_signal, signal, cross_sec[m])
	#set_sqrt_errors(data_h_merged)
	#randomize_hists(data_h_merged,'DATA')
        #randomize_hists(data_h_merged,'Background')
        
        mass_merged_rebin     = simpleRebin(data_h_merged,unc,['Background','Data'],data_h_merged.replace('.root','_rebinned.root'),['Background','Data'])
        mass_merged_rebin_unc = mass_merged_rebin.replace('.root','_unc.root')
        add_signal_background_uncer(mass_merged_rebin, rootfile_MC_forward_merged, rootfile_MC_central_merged, mass_merged_rebin_unc,True)

        #calculate the SR/CR fit
        print '#'*5
        mass_injection.append(background_fit(mass_merged_rebin_unc, channel, False, "")[0])
        print '*'*5
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print '#'*5
        
        mass_merged_signalfit = data_h_merged.replace('.root','_signalfit.root')
       
        central_signal = rename_process(central_signal,signal.replace('-','_'),"Signal_central_"+mass)
        forward_signal = rename_process(forward_signal,signal.replace('-','_'),"Signal_forward_"+mass)
        inverted_central_signal = invert_bin_content(central_signal,"Signal_central_"+mass,'',-1)
        
        call(['rm '+mass_merged_signalfit],shell=True)
        call(['hadd '+mass_merged_signalfit+' '+mass_merged_rebin_unc+' '+forward_signal+' '+inverted_central_signal],shell=True)#

        #scale hists according to the background mle fit

        scale_factor = nom_exp.y[m]*0.1
        
        scale_hists(mass_merged_signalfit, 'Background',mass_injection[-1])#nominal_results)
        scale_hists(mass_merged_signalfit, 'Signal_central_'+mass,mass_injection[-1])#nominal_results)
        scale_hists(mass_merged_signalfit, 'Signal_central_'+mass,scale_factor)#nominal_results)
        scale_hists(mass_merged_signalfit, 'Signal_forward_'+mass,scale_factor)#nominal_results)
        signal_rebin(mass_merged_signalfit,'Signal_forward_'+mass)
        signal_rebin(mass_merged_signalfit,'Signal_central_'+mass)
        sys_reweight(mass_merged_signalfit,'PDF')
        sys_reweight(mass_merged_signalfit,'scale')
        #fitted_signal =0
        fitted_signal = best_signalfit(mass_merged_signalfit,mass)
        fitted_signal[0] = fitted_signal[0]*scale_factor
        fitted_signal[1] = fitted_signal[1]*scale_factor
	print '\033[92m'
        print 'injected cross section:',cross_sec[m],'fitted cross section:',fitted_signal
        print '\033[0m'
        signal_fit.append(fitted_signal[0])
        signal_fit_err.append(fitted_signal[1])
        
        #continue
        #signal_rebin(inverted_central_signal,'Signal_central')
        print >> result_file, "++"*10
        print >> result_file, 'injected result', signal, ' with cross section',  cross_sec[m]
        print >> result_file, 'no signal substraction'
        print >> result_file, 'scale values'
        print >> result_file, mass_injection[-1]
        print >> result_file, 'fitted cross section',fitted_signal
        
        continue
        
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

    print >> result_file,''
    print 'signal fit values and error'
    print signal_fit
    print signal_fit_err
    print >> result_file, 'signal fit values and error'
    print >> result_file,signal_fit
    print >> result_file,signal_fit_err
    print >> result_file, 'masses'
    print >> result_file, nom_exp.x
    print >> result_file, 'y limit values'
    print >> result_file, nom_exp.y
    print >> result_file, 'bands'
    print >> result_file, nom_exp.bands
    print >> result_file, 'observed'
    print >> result_file, nom_obs.y
    print >> result_file, nom_obs.yerrors
    
