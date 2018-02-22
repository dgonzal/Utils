

from subprocess import call
import os, sys
from matplotlib.backends.backend_pdf import PdfPages
import glob

prod_channels = ['Mu','Ele']# 'Mu','Ele',''

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
sourcedirs = ['/MuSel_new/','/EleSel_new/'] #'/MuSel_wtag_topjetcorr/'

prod_channels = ['Mu']# 'Mu','Ele',''
sourcedirs = ['/MuSel_recoptcuts_v2/']#new

signal_src = ['/MuSel_recoptcuts_v2/']#new

rootDir = 'ROOTRecoComp/'
prefix =''#nominal_

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets*.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt*.root,QCD*.root"
#MC = "TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root"#,QCD.root"



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

    central_rootfile = rootDir+prefix+channel+'_MC_central.root'
    #call(['./../bin/rootfilecreator', str(MC), str(central_rootfile), str(dirstring),str(channel),"","","central"])
    forward_rootfile = rootDir+prefix+channel+'_MC_forward.root'
    ##call(['./../bin/rootfilecreator', str(MC), str(forward_rootfile),str(dirstring),str(channel),"","","forward"])
    signal_files=find_files(dirstring+'/*'+signal_prefix+production+'*'+chirality+'.root')
    signal_rootfile = rootDir+prefix+channel+'_MC_forward_signal.root'
    #call(['./../bin/rootfilecreator', signal_files, signal_rootfile, dirstring, channel,"","","forward"])
   
    merged_central = create_merged_hists(central_rootfile, central_rootfile.replace('.root','_merged.root') , 'Background')
    merged_forward = create_merged_hists(forward_rootfile, forward_rootfile.replace('.root','_merged.root') , 'DATA')
    
    merged_central_unc = add_signal_background_uncer(merged_central, merged_forward , merged_central, merged_central,True )

    background_model = rootDir+prefix+channel+'_MC_model.root'
    #print background_model
    call(['rm '+background_model],shell=True)
    call(['hadd '+background_model+' '+merged_central_unc+' '+' '+merged_forward],shell=True)

    stat_uncer = 0.15
    model_rebinned = simpleRebin(background_model, stat_uncer, ['Background'])

    print '*'*10
    print '*'*10
    print "nominal results"
    nominal_results = background_fit(model_rebinned, channel, False, "")    
    print nominal_results    
    print '*'*10
    print '*'*10

    
    full_model = rootDir+prefix+channel+'_MC_fullmodel.root'
    call(['hadd -f '+full_model+' '+background_model+' '+signal_rootfile], shell=True)
    
    full_model_rebin = simpleRebin(full_model, stat_uncer, ['Background'])
    scale_hists(full_model_rebin, 'Background', nominal_results)

    nom_exp,nom_obs,zvalue = injected_signal_mc_limits(full_model_rebin, chirality, channel, signal_prefix+production , False, "nominal")
    
    result_file = open(prefix+channel+'_recoComp_result.txt','w+')
    print >> result_file, 'nominal result'
    print >> result_file, 'nominal exp'
    print >> result_file, nom_exp
    print >> result_file, 'nominal obs'
    print >> result_file, nom_obs    

    for m,mass in enumerate(masses):
        #signal to inject
        signal = signal_prefix+production+'-'+mass+'_'+chirality
        print 'Injecting ', signal, ' with cross section',  cross_sec[m]
                
        #create signal histos to add to the CR & SR
        central_sig = rootDir+prefix+channel+'_MC_central_sig_'+mass+'.root'
        forward_sig = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'.root'
        #forward_sig_unc = rootDir+prefix+channel+'_MC_forward_sig_'+mass+'_unc.root'
        call(['./../bin/rootfilecreator', str(signal), str(central_sig), str(dirstring), str(channel),"","","central"])
        call(['./../bin/rootfilecreator', str(signal), str(forward_sig), str(dirstring), str(channel),"","","forward"])

        mass_injected = full_model.replace('.root','_injected_'+mass+'.root')
        #copy file
        call(['rm '+mass_injected],shell=True)
        call(['hadd '+mass_injected+' '+background_model+' '+signal_rootfile],shell=True)
        #add signal 
        add_signal(mass_injected, 'Background', central_sig, signal, cross_sec[m])
        add_signal(mass_injected, 'DATA'      , forward_sig, signal, cross_sec[m])
       
        #rebin with 20% stat unc.
        final_injected = simpleRebin(mass_injected, stat_uncer, ['Background'])
        #calculate the SR/CR fit
        scale_hists(final_injected,'Background',nominal_results) 
        #calculate limits
        inj_exp,inj_obs, zvalue = injected_signal_mc_limits(final_injected, chirality, channel, signal_prefix+production , False, mass+"_injected", mass)
        print 'Injected ', signal_prefix, production, mass, 'GeV', 'cross section',  cross_sec[m]
        print 'expected limit ratio for injected mass',mass


        print >> result_file, "++"*10
        print >> result_file, 'injected result', signal, ' with cross section',  cross_sec[m]
        print >> result_file, 'injected exp'
        print >> result_file, inj_exp
        print >> result_file, 'injected obs'
        print >> result_file, inj_obs
        print >> result_file, 'z values'
        print >> result_file, zvalue
        



                    
    
