from subprocess import call
import os, sys
from matplotlib.backends.backend_pdf import PdfPages
import glob
from shutil import copyfile

sys.path.append('/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/Utils/limits/')
sys.path.append('/nfs/dust/cms/user/gonvaq/theta/utils2/theta_auto')

from signal_background_uncertainty import *    
from backgroundfit import *
from injection_merge import *
from calc_chi2 import *
from invert_bin_content import *
"""
execfile("signal_background_uncertainty.py")
execfile("invert_bin_content.py")
execfile("backgroundfit.py")
execfile("injection_merge.py")
execfile("calc_chi2.py")
"""

signal_prefix = 'Bprime' #'X53',
production = 'T' #,'T'] #'B','T']
chirality = 'RH' #,'LH'] #['RH','LH']
 
release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
signal_src_mu  ='MuSigSel'
signal_src_ele ='EleSigSel_v1'

rootDir = "ROOT_Significance/"

if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)


sigma1_color = 'gray' #"green"
#x = [float(i) for i in masses]


signal = signal_prefix+production
model_file = 'ROOTDataShape_50_0_3500_eta4/Eta4_BprimeT_RH_rebinned_sr_rescale.root' #significance_BprimeT_RH.root' # significance_test.root'

nom_exp,nom_obs, zvalue =  injected_signal_mc_limits(model_file, chirality,'' , signal, False)

masses = get_masses(model_file,signal)
print masses

z_values = []
fitted_cross_secs = []
z_values_sub =[]

result_file = open(signal+'_significance_results.txt','w+')
print >> result_file, 'nominal result'
print >> result_file, 'nominal exp'
print >> result_file, nom_exp
print >> result_file, 'nominal obs'
print >> result_file, nom_obs    
    
for m,mass in enumerate(masses):
    print >> result_file
    print >> result_file
    print >> result_file, '*****'*20
    print >> result_file, 'Results for mass', mass, signal 
    
    single_mass_no_unc = sculp_root_file(model_file, signal, '_'+mass+'_', False, model_file.replace('.root','_'+mass+'no_unc.root'))
    single_mass_unc    = sculp_root_file(model_file, signal, '_'+mass+'_', unc = True)

    single_mass_no_unc = rename_process(single_mass_no_unc, signal+'_'+mass+'_'+chirality,"Signal_forward_"+mass)
    single_mass_renamed = rename_process(single_mass_unc, signal+'_'+mass+'_'+chirality,"Signal_forward_"+mass)
    
    #plain z value with unc
    print single_mass_renamed
    inj_exp,inj_obs, zvalue = injected_signal_mc_limits(single_mass_renamed, chirality, '', signal_prefix+production , False, mass+"_injected",mass)
    print >> result_file, 'zvalue',zvalue 
    z_values.append(zvalue)
    
    central_sig = rootDir+signal+mass+'.root'
    mu_central_sig = central_sig.replace('.root','_mu.root')
    ele_central_sig = central_sig.replace('.root','_ele.root')
    
    call(['./../bin/rootfilecreator', signal+'-'+mass+'_'+chirality, mu_central_sig, release+signal_src_mu , 'Mu' ,"","","central"])
    call(['./../bin/rootfilecreator', signal+'-'+mass+'_'+chirality, ele_central_sig, release+signal_src_ele, 'Ele',"","","central"])
    
    call(['rm '+central_sig],shell=True)
    call(['hadd '+central_sig+' '+mu_central_sig+' '+ele_central_sig],shell=True)
    central_sig = rename_process(central_sig,signal+'_'+mass+'_'+chirality,"Signal_central_"+mass)
    inverted_central_signal = invert_bin_content(central_sig,"Signal_central_"+mass,'',-1)
    scale_hists(inverted_central_signal,'Anti'   ,0.25)
    scale_hists(inverted_central_signal,'_BTag'  ,0.295)
    scale_hists(inverted_central_signal,'WTag'   ,0.135)
    scale_hists(inverted_central_signal,'TopTag' ,0.135)

    fit_model = single_mass_no_unc.replace('.root','_central_sig.root')
    call(['rm '+fit_model],shell=True)
    call(['hadd '+fit_model+' '+single_mass_no_unc+' '+inverted_central_signal],shell=True)
    signal_rebin(fit_model,'Signal_forward_'+mass)
    signal_rebin(fit_model,'Signal_central_'+mass)
    cross_section_fit = False
    try:
      fitted_signal = best_signalfit(fit_model,mass)
      print '\033[92m'
      print 'fitted cross section:',fitted_signal
      print '\033[0m'
      
      print >> result_file, 'fitted cross section', fitted_signal
      
      signal_sub_model =  single_mass_renamed.replace('.root','_sig_sub.root')
      call(['rm '+signal_sub_model],shell=True)
      call(['hadd '+signal_sub_model+' '+single_mass_renamed],shell=True)

      add_signal(signal_sub_model, 'Background', inverted_central_signal, 'Signal_central_'+mass,fitted_signal[0])
      add_substraction_unc(single_mass_renamed,central_sig,'Signal_central_'+mass,fitted_signal)
      #exit(0) 
      inj_exp,inj_obs, zvalue_sub = injected_signal_mc_limits(signal_sub_model, chirality, '', signal_prefix+production , False, mass+"_injected",mass)
      print >> result_file, 'z value subtraction',zvalue_sub
      
      fitted_cross_secs.append(fitted_signal)
      z_values_sub.append(zvalue_sub)
      cross_section_fit = True
    except:  
	print >> result_file, 'z value substraction not possible'

    try:
      if cross_section_fit: continue
      factor = 0.01
      fit_model_sig_scaled = fit_model.replace(".root","_sig_scaled.root")
      call(['rm '+fit_model_sig_scaled],shell=True)
      call(['hadd '+fit_model_sig_scaled+' '+fit_model],shell=True)
      scale_hists(fit_model_sig_scaled,'Signal',factor)    
      fitted_signal = best_signalfit(fit_model_sig_scaled,mass)
      fitted_signal[0]= fitted_signal[0]*factor
      fitted_signal[1]= fitted_signal[1]*factor
      
      print '\033[92m'
      print 'fitted cross section:',fitted_signal
      print '\033[0m'
      
      print >> result_file, 'fitted cross section', fitted_signal
      
      signal_sub_model =  single_mass_renamed.replace('.root','_sig_sub.root')
      call(['rm '+signal_sub_model],shell=True)
      call(['hadd '+signal_sub_model+' '+single_mass_renamed],shell=True)

      add_signal(signal_sub_model, 'Background', inverted_central_signal, 'Signal_central_'+mass,fitted_signal[0])
      add_substraction_unc(single_mass_renamed,central_sig,'Signal_central_'+mass,fitted_signal)
      #exit(0) 
      inj_exp,inj_obs, zvalue_sub = injected_signal_mc_limits(signal_sub_model, chirality, '', signal_prefix+production , False, mass+"_injected",mass)
      print >> result_file, 'z value subtraction',zvalue_sub
      fitted_cross_secs.append(fitted_signal)
      z_values_sub.append(zvalue_sub)
    except:  
	print >> result_file, 'z value substraction not possible'

print >> result_file, '*'*50
print >> result_file, '='*50
print >> result_file, 'Done with the calculations, print results nicely'
print >> result_file, signal,'mass [GeV]','fitted cross signal cross', 'z value'
print >> result_file,z_values_sub
print >> result_file,z_values


for i in xrange(len(z_values)):
    print >> result_file, masses[i],fitted_cross_secs[i][0],fitted_cross_secs[i][1], z_values_sub[0][i][0],z_values_sub[0][i][1], z_values[0][i][0],z_values[0][i][0]
    
    
