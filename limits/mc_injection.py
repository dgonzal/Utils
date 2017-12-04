

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

rootDir = 'ROOT_Injection_MC_v2/'
prefix =''

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets*.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt*.root"
#MC = "TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root"#,QCD.root"
background = 'Data.root,DATA.root' #"SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"


signal_prefix = 'Bprime' #'X53',
production = 'B' #,'T' #'B','T'
chirality = 'RH' #,'LH' #'RH','LH'
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
execfile("fit_mc.py")
execfile("injection_merge.py")
execfile("mc_merge.py")
execfile("histogram_rebinning.py")


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
    rootfile_MC_central_errors = rootfile_MC_central.replace('.root', '_errors.root')
    rootfile_MC_forward_errors =  rootfile_MC_forward.replace('.root', '_errors.root')
    signal_files = find_files(dirstring+'/*'+signal_prefix+production+'*'+chirality+'.root')
    signal_rootfile_central = rootDir+prefix+channel+'_signal_MC_central.root'
    signal_rootfile_forward = signal_rootfile_central.replace('central','forward')
    # create rootfile with hists for theta
    if create:
      call(['./../bin/rootfilecreator', MC, rootfile_MC_central,dirstring,channel,"","","central"])
      call(['./../bin/rootfilecreator', MC, rootfile_MC_forward,dirstring,channel,"","","forward"])
      call(['./../bin/rootfilecreator', MC, rootfile_MC_central_errors,dirstring,channel,"","","central_errors"])
      call(['./../bin/rootfilecreator', MC, rootfile_MC_forward_errors,dirstring,channel,"","","forward_errors"])
      call(['./../bin/rootfilecreator', signal_files, signal_rootfile_central,dirstring,channel,"","","central_errors"])
      call(['./../bin/rootfilecreator', signal_files, signal_rootfile_forward,dirstring,channel,"","","forward_errors"])

    #continue
    #merge histos into one
    rootfile_MC_central_merged = rootfile_MC_central.replace('.root','_h_merged.root')
    rootfile_MC_forward_merged = rootfile_MC_forward.replace('.root','_h_merged.root')
    create_merged_hists(rootfile_MC_central, rootfile_MC_central_merged, 'DATA',[channel,'central'+channel])
    create_merged_hists(rootfile_MC_forward, rootfile_MC_forward_merged, 'DATA',[channel,'forward'+channel])


    signal_comb = signal_rootfile_central.replace('central','')
    call(['rm '+signal_comb],shell=True) 
    call(['hadd '+signal_comb+' '+signal_rootfile_central+' '+signal_rootfile_forward],shell=True)
    sys_reweight(signal_comb,'PDF')
    sys_reweight(signal_comb,'scale')

    
    rootfile_MC_central_errors = merge_mc(['ZJets','WJets','SingleT','TTbar'],rootfile_MC_central_errors)
    rootfile_MC_forward_errors = merge_mc(['ZJets','WJets','SingleT','TTbar'],rootfile_MC_forward_errors)
    
    
    #merge root files into one 
    rootfile_MC_merged = rootDir+prefix+channel+'_merged.root'
    print 'hadd '+rootfile_MC_merged+' '+rootfile_MC_central_merged+' '+rootfile_MC_forward_merged, rootfile_MC_central_errors, rootfile_MC_forward_errors, signal_rootfile_central, signal_rootfile_forward
    call(['rm '+rootfile_MC_merged],shell=True) 
    call(['hadd '+rootfile_MC_merged+' '+rootfile_MC_central_merged+' '+rootfile_MC_forward_merged+' '+rootfile_MC_central_errors+' '+rootfile_MC_forward_errors+' '+signal_comb],shell=True)

   
    rootfile_MC_merged_rebin =  binFile(0.2, rootfile_MC_merged, 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleT','TTbar'])

    #calculate normalisation
    print '*'*10
    print '*'*10
    print "nominal results"
    nom_exp,nom_obs, zvalue = injected_signal_mc_limits(rootfile_MC_merged_rebin, chirality, channel, signal_prefix+production , True)
    print nominal_results    
    print '*'*10
    print '*'*10
    #continue
    print '+'*10
      
    continue
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



                    
    
