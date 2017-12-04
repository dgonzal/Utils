from subprocess import call
import os, sys

channel = 'Mu' #'Mu',"Ele"
production = 'B' #'T','B' # For b or top quarks 
chirality = 'RH'

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/'
source_dir = 'config/MuSel_wtag_topjetcorr'  #short_jets/'
rootDir = 'ROOTtest/'
prefix = ""
createfiles = True
rebin = True 
limits = True
signal_injection = False
signal_pre = 'Bprime'


if not rebin:
    createfiles = False

if not os.path.exists(rootDir):
    os.makedirs(rootDir)
if not os.path.exists("h_"+rootDir):
    os.makedirs("h_"+rootDir)

#rootfile = str(rootDir+channel+'Bp'+production+'Reco_'+chirality)

execfile("histogram_rebinning.py")
execfile("calculation_test.py")

vanilla = channel+'Bp'+production+'Reco_'+chirality
#
# comparing the reco just for the same W-tag events between X^2 and W-tag reco
#
rootfile = rootDir+"Chi2_"+vanilla
"""
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","chi2"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD','SingleT_s','SingleT_t','SingleTWAntitop','SingleTWtop'])
exp_chi2,obs_chi2 = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

rootfile = rootDir+"WReco_"+vanilla
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","wreco"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD'])
exp_wrec,obs_wrec = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

#
# Old reconstruction method with 50-500 GeV W mass range 
#
rootfile = rootDir+"Chi2Old_"+vanilla
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+"config/MuSel_chs_vanilla",channel,"","","chi2_btag_only"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD','SingleT_s','SingleT_t','SingleTWAntitop','SingleTWtop'])
exp_chi2old,obs_chi2old = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

#
# New sheme with up to 9 jets without a cut
#
rootfile = rootDir+"Chi2New_"+vanilla
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","chi2_btag_only"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD','SingleT_s','SingleT_t','SingleTWAntitop','SingleTWtop'])
exp_chi2new,obs_chi2new = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

#
# Exclude W-tag events to see the effect of the new categroy
#
rootfile = rootDir+"Chi2BTag_"+vanilla
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","chi2_btag"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD'])
exp_ChiBTag,obs_ChiBTag = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

#
# X^2 b-tag + W-tag reco
#
rootfile = rootDir+"Chi2WTag_"+vanilla
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","chi2_wtag"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD'])
exp_ChiWTag,obs_ChiWTag = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

#
# X^2 b-tag+W-tag categorization
#
rootfile = rootDir+"Chi2CatWTag_"+vanilla
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","chi2_wtag_cat"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD'])
exp_ChiCatWTag,obs_ChiCatWTag = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

#
# t-tag category
#
rootfile = rootDir+"TopTag_"+vanilla
#call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","toptagreco"])
#binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD'])
exp_TopTag,obs_TopTag = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)
"""
#
# combination of all categories
#
rootfile = rootDir+"Combi_"+vanilla
call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality,rootfile,release+source_dir,channel,"","","mc"])
binFile(0.2, rootfile+'.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets_Pt','TTbar','QCD'])
exp_combi,obs_combi = run_cutopt(rootfile+"_rebinned.root",chirality,channel,production,True)

sys.exit(0)

#Comparison plots
pp = PdfPages("CLRecoComparison_v2.pdf")


plt.clf()
plt.semilogy()
plt.title("New VS old reconstruction", fontsize=10)# , loc='right')
plt.plot(exp_chi2old.x, exp_chi2old.y, label="$old X^2$ Reco." , color ='black',linestyle='dotted')
plt.plot(exp_chi2new.x, exp_chi2new.y, label="$new X^2$ Reco." , color ='red'  ,linestyle='dotted')
plt.xlabel('B quark Mass (GeV)')
plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
plt.legend(loc=2,prop={'size':12},frameon=False)
plt.savefig(pp, format='pdf')
plt.close()


plt.clf()
plt.semilogy()
plt.title("limit comparison for w-tag events", fontsize=10)# , loc='right')
plt.plot(exp_chi2.x, exp_chi2.y, label="$X^2$ Reco." , color ='black',linestyle='dotted')
plt.plot(exp_wrec.x, exp_wrec.y, label="W-tag Reco." , color ='red',linestyle='dotted')
plt.xlabel('B quark Mass (GeV)')
plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
plt.legend(loc=2,prop={'size':12},frameon=False)
plt.savefig(pp, format='pdf')
plt.close()

plt.clf()
plt.semilogy()
plt.title("limit comparison with and without w-tag", fontsize=10)# , loc='right')
plt.plot(exp_chi2new.x, exp_chi2new.y, label="$X^2$ reco. with b-tags" , color ='orange',linestyle='dotted')
plt.plot(exp_ChiBTag.x, exp_ChiBTag.y, label="$X^2$ reco. with b-tags & without W-tag" , color ='green',linestyle='dotted')
plt.plot(exp_ChiCatWTag.x, exp_ChiCatWTag.y, label="+ W-tag category" , color ='black',linestyle='dotted')
plt.plot(exp_ChiWTag.x, exp_ChiWTag.y, label="+ W-tag reco." , color ='blue',linestyle='dotted')
plt.xlabel('B quark Mass (GeV)')
plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
plt.legend(loc=2,prop={'size':12},frameon=False)
plt.savefig(pp, format='pdf')
plt.close()

plt.clf()
plt.semilogy()
plt.title("reconstruction comparison", fontsize=10)# , loc='right')
plt.plot(exp_ChiBTag.x, exp_ChiBTag.y, label="$X^2$ reco. with b-tags" , color ='green',linestyle='dotted')
plt.plot(exp_wrec.x, exp_wrec.y, label="W-tag reco." , color ='blue',linestyle='dotted')
plt.plot(exp_TopTag.x, exp_TopTag.y, label="top-tag reco." , color ='red',linestyle='dotted')
plt.plot(exp_combi.x, exp_combi.y, label="combination" , color ='black',linestyle='dotted')

plt.xlabel('B quark Mass (GeV)')
plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
plt.legend(loc=2,prop={'size':12},frameon=False)
plt.savefig(pp, format='pdf')
plt.close()




#ratio
ratio =[]
for i in xrange(len(exp_chi2.y)):
    ratio.append(exp_chi2.y[i]/exp_wrec.y[i])
plt.clf()
plt.title("limit ratio", fontsize=10)
plt.plot(exp_chi2.x, ratio, label="$X^2$/W-tag Reco." , color ='black',linestyle='dotted')
plt.xlabel('B quark Mass (GeV)')
plt.legend(loc=2,prop={'size':12},frameon=False)
plt.savefig(pp, format='pdf')
plt.close()
pp.close()
    
