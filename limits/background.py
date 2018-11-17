from subprocess import call
import os, sys
from matplotlib.backends.backend_pdf import PdfPages
import glob

sys.path.append('/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/Utils/limits/')
sys.path.append('/nfs/dust/cms/user/gonvaq/theta/utils2/theta_auto')

from signal_background_uncertainty import *    
from backgroundfit import *
from injection_merge import *
from calc_chi2 import *
#execfile("histogram_rebinning.py")


prod_channels = ['Mu','Ele']# 'Mu','Ele',''

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/'
sourcedirs = ['/MuSel_new/','/EleSel_new/'] #'/MuSel_wtag_topjetcorr/'

#prod_channels = ['Ele']# 'Mu','Ele',''
#sourcedirs = ['/EleSel_new/']


rootDir = 'ROOT_50_0_3500_eta4/' #ROOT_50_0_3500_eta4/' #ROOT_DataSideband_04/' # ROOT_DataSideband_04/' #ROOT_50_0_3500/' #'60_0_3500/'DataSideband_04/
prefix =''
postfix=''

#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt.root,QCD.root"
#MC = "SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets*.root,TTbar_Tune.root,TTbar_Mtt700to1000.root,TTbar_Mtt1000toInf.root,WJets_Pt*.root,QCD*.root"
MC = "MC.TTbar_*.root,WJets_Pt*.root,ZJets*.root,SingleT*.root,QCD*.root"
Signal = "BprimeB-1100_RH.root,BprimeB-1700_RH.root"
background = 'Merge_Data.root' #"SingleT_s.root,SingleT_t.root,SingleTWAntitop.root,SingleTWtop.root,ZJets.root,TTbar.root,WJets_Pt.root,QCD.root"

if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)


sigma1_color = 'gray' #"green"

plotting_files = ""
mc_channel_f= []
mc_channel_c= []
data_channel =[]

create = True

signal_files = ""


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
        
    data_rootfile = rootDir+prefix+channel+'_background.root'
    central_mc_rootfile = rootDir+prefix+channel+'_MC_central.root'
    forward_mc_rootfile = rootDir+prefix+channel+'_MC_forward.root'
        
    signal_file  = rootDir+prefix+channel+'_signal.root'
    signal_files= signal_files+" "+signal_file
    
    print rootfile
    if create:
        call(['./../bin/rootfilecreator', background, data_rootfile,dirstring,channel,"","",""])
        call(['./../bin/rootfilecreator', MC, central_mc_rootfile,dirstring,channel,"","","central"])
        call(['./../bin/rootfilecreator', MC, forward_mc_rootfile,dirstring,channel,"","","forward"])
        call(['./../bin/rootfilecreator', Signal, signal_file,dirstring,channel,"","","DATA_forward"])
        

        
    central_merged = central_mc_rootfile.replace('.root','_merged.root')
    forward_merged = forward_mc_rootfile.replace('.root','_merged.root')
    create_merged_hists(central_mc_rootfile,central_merged, 'Background')
    create_merged_hists(forward_mc_rootfile,forward_merged, 'DATA')

    data_channel.append(data_rootfile)
    mc_channel_f.append(forward_merged)
    mc_channel_c.append(central_merged)

    
    mc_merged = rootDir+prefix+channel+'_mc_merged.root'
    mc_merged_unc = mc_merged.replace('.root','_unc.root')
    data_unc = data_rootfile.replace('.root','_unc.root')
    
    #call(['rm '+mc_merged],shell=True) 
    #call(['hadd '+mc_merged+' '+forward_merged+' '+central_merged],shell=True)
    #call(['rm '+mc_merged_unc],shell=True)
    #call(['hadd '+mc_merged_unc+' '+mc_merged],shell=True)
    

    stat_uncer = 5.5
    #rebinned_unc_data = simpleRebin(data_unc,stat_uncer,['DATA'],data_unc.replace('.root','_rebinned.root'),['Background'])
    #rebinned_data = simpleRebin(data_rootfile,stat_uncer,['Background'],data_rootfile.replace('.root','_rebinned.root'),['Background'])
    
    call(['hadd -f '+data_rootfile.replace('.root','_work.root')+' '+data_rootfile],shell=True)
    data_rootfile=data_rootfile.replace('.root','_work.root')
    #antibtag_binning = [300.0, 370.0, 440.0, 510.0, 580.0, 650.0, 720.0, 790.0, 860.0, 930.0, 1000.0, 1070.0, 1140.0, 1210.0, 1280.0, 1350.0, 1420.0, 1490.0, 1560.0, 1630.0, 1700.0, 1770.0, 1840.0, 1910.0, 1980.0, 2050.0, 2190.0, 2540.0, 4500.0]
    
    #fixed_rebin(data_rootfile,2)#,'AntiBTag')
    
    rebinned_data = simpleRebin(data_rootfile,stat_uncer,['Background'],data_unc.replace('.root','_rebinned.root'),['Background'])
    rebinned_data = simpleRebin(rebinned_data,0.4,['DATA'],rebinned_data.replace('.root','_final.root'),['Background'])    
    rebinned_unc_data = add_signal_background_uncer(rebinned_data, forward_merged, central_merged,"",True )
    rebinned_unc_data = simpleRebin(rebinned_unc_data,stat_uncer,['Background'],'',['Background'])

    #if i ==0:
    #    continue
    #else:
    #    exit(0)
    
    print 'final anti b-tag rebinning'
    #rebinned_unc_data = simpleRebin(rebinned_unc_data,0.09,['Background'],'',['Background'],'Anti')
    plotting_files += rebinned_unc_data+" "
    
    
    print '*'*10
    print '*'*10
    print "nominal results"
    #scale_hists(rebinned_unc_data,'Background',0.25)
    nominal_results, fitted = background_fit(rebinned_unc_data, channel, False, "")
    #nominal_results,fitted = background_fit(rebinned_data, channel, False, "")
    
    #print nominal_results    
    print '*'*10
    print '*'*10
    #sys.exit(0)
    #continue
    print '+'*10

    #scale_hists(rebinned_unc_data,'Background',nominal_results) 

combined_fit = rootDir+'background.root'
call(['hadd -f '+combined_fit+' '+plotting_files],shell=True)

#mu_binning = [0.0, 225.0, 450.0, 525.0, 600.0, 675.0, 750.0, 825.0, 900.0, 975.0, 1050.0, 1125.0, 1200.0, 1275.0, 1425.0, 1500.0, 1650.0, 1725.0, 1875.0, 2475.0, 4500.0]
#fixed_rebin(combined_fit,mu_binning,'AntiBTagMu')

#ele_binning = [0.0, 300.0, 375.0, 450.0, 525.0, 600.0, 675.0, 750.0, 825.0, 900.0, 975.0, 1050.0, 1125.0, 1200.0, 1275.0, 1350.0, 1425.0, 1500.0, 1575.0, 1725.0, 1875.0, 2250.0, 4500.0] 
#fixed_rebin(combined_fit,ele_binning,'AntiBTagEle')

combined_fit = rescale_on_count(combined_fit,'DATA','Background')

combined_results, fitted = background_fit(combined_fit, '', False, "")
#scale_hists(combined_fit, 'Background', nominal_results)

prefit_plot  = rootDir+'background_prefit.root'
postfit_plot = rootDir+'background_postfit.root'

call(['hadd -f '+prefit_plot +' '+combined_fit],shell=True)  #+' '+signal_files
call(['hadd -f '+postfit_plot+' '+combined_fit.replace('.root','_fit_out.root')+' '+signal_files],shell=True)


#hist_title(postfit_plot,'m_{reco}')
#hist_title(prefit_plot,'m_{reco}')

signal_rebin(prefit_plot , "Bprime", 'DATA')
signal_rebin(postfit_plot, "Bprime", 'DATA')


print 'prefit chi2'
chi2_calc(prefit_plot,'DATA','Background')
print 'postfif chi2'
chi2_calc(postfit_plot,'DATA','Background')
#exit(0)
print events_per_width(prefit_plot)
print events_per_width(postfit_plot)

#blind_final(postfit_plot,'DATA','Background')
sort_hists(prefit_plot)
#sort_hists(postfit_plot)

call(['Plots -f ../../../SFramePlotter/BackgroundTheta.steer'],shell=True)
call(['Plots -f ../../../SFramePlotter/BackgroundTheta_postfit.steer'],shell=True)
if 'sideband' in rootDir:call(['Plots -f ../../../SFramePlotter/BackgroundTheta_sideband.steer'],shell=True)
#print rootDir+'background.root'

