from subprocess import call

channels = ["Mu"]#,"Ele",""] 

call(['root', '-l',' limit.C("LH_25ns","BpbReco_LH")'])
call(['root', '-l',' limit.C("RH_25ns","BpbReco_RH")'])

execfile("histogram_rebinning.py")
for channel in channels:
    binFile(0.3, channel+'BpbReco_LH.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])
    binFile(0.3, channel+'BpbReco_RH.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])
#binFile(0.3, 'BpReco_LH.root', 'M_{B} [GeV/c^{2}]', ['Background'])
#binFile(0.3, 'BpReco_RH.root', 'M_{B} [GeV/c^{2}]', ['Background'])


execfile("calculation.py")
for channel in channels:
    exp_RH,obs_RH = run_cutopt(channel+"BpbReco_RH_rebinned.root","RH",channel,"b",True)
    exp_LH,obs_LH = run_cutopt(channel+"BpbReco_LH_rebinned.root","LH",channel,"b",True)
    #exp_RH_res,obs_RH_res = run_cutopt(channel+"BpbReco_RH_res.root","RH",channel,"b",True)
    #exp_RH_boost,obs_RH_boost = run_cutopt(channel+"BpbReco_RH_boost.root","RH",channel,"b",True)

"""
#split stuff into boosted and resolved cases to compare sensitivity of the different channels
plt.clf()
plt.semilogy()
plt.gca().set_ylim([0.1,2])
plt.plot(exp_RH.x, exp_RH.y, label='Combination', color ='black',linestyle='--')#,color = exp_RH.bands[0][2])
plt.plot(exp_RH_res.x, exp_RH_res.y, label='Resolved', color ='red',linestyle='--')#,color = exp_RH.bands[0][2])
plt.plot(exp_RH_boost.x, exp_RH_boost.y, label='Boosted', color ='green',linestyle='--')#,color = exp_RH.bands[0][2])

#plt.plot(theroy13TeV_x, theory13TeV_y, label='Theory cross section 13TeV')
plt.xlabel('Mass B [GeV]')
plt.ylabel('cross section times branching ratio [pb]')
plt.legend(loc=2,prop={'size':10})
plt.savefig("limit_comp_regimes.pdf")
"""
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

