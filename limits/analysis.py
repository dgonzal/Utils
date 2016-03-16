
from subprocess import call
call(['root', '-l',' limit.C("LH_25ns","BpReco_LH")'])
call(['root', '-l',' limit.C("RH_25ns","BpReco_RH")'])

execfile("histogram_rebinning.py")
binFile(0.3, 'BpReco_LH.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])
binFile(0.3, 'BpReco_RH.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])
#binFile(0.3, 'BpReco_LH.root', 'M_{B} [GeV/c^{2}]', ['Background'])
#binFile(0.3, 'BpReco_RH.root', 'M_{B} [GeV/c^{2}]', ['Background'])


execfile("calculation.py")
exp_LH,obs_LH = run_cutopt("BpReco_LH_rebinned.root","LH",True)
exp_RH,obs_RH = run_cutopt("BpReco_RH_rebinned.root","RH",True)

#comparison plot for left and right handed
plt.clf()
plt.semilogy()
plt.plot(exp_LH.x, exp_LH.y, label='LH Chirality', color ='red',linestyle='--')#,color = exp_LH.bands[0][2])
plt.fill_between(exp_LH.x, exp_LH.bands[0][0] ,  exp_LH.bands[0][1],
                 alpha=0.4, facecolor='red', edgecolor='red',
                 linewidth=0)
plt.plot(exp_RH.x, exp_RH.y, label='RH Chirality', color ='yellow',linestyle='--')#,color = exp_RH.bands[0][2])
plt.fill_between(exp_RH.x, exp_RH.bands[0][0] ,  exp_RH.bands[0][1],
                 alpha=0.4, facecolor='yellow', edgecolor='yellow',
                 linewidth=0)

#plt.plot(theroy13TeV_x, theory13TeV_y, label='Theory cross section 13TeV')
plt.xlabel('Mass B [GeV]')
plt.ylabel('cross section times branching ratio [pb]')
plt.legend(loc=2,prop={'size':7})
plt.savefig("limit_comp.pdf")

