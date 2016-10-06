from subprocess import call
channels = ["Mu","Ele",""]
production_channels = ["b","t"]

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/'
Mudirs = ['config/Selection_v31/']
Eledirs = ['config/EleSelection_v6_tree/']

createfiles = False
rebin = False

for production in production_channels:
    if createfiles:
        for channel in channels:
            print 'Creating Files',production,channel
            dirstring = ""
            if "Mu" in channel:
                for inf in Mudirs:
                    dirstring = dirstring+release+inf+','
            elif "Ele" in channel:
                for inf in Eledirs:
                    dirstring = dirstring+release+inf+','
            else:
                for inf in Mudirs:
                    dirstring = dirstring+release+inf+','
                for inf in Eledirs:
                    dirstring = dirstring+release+inf+','
            dirstring = dirstring[:-1]
            #print dirstring
            #call(['root', '-l',' limit.C("LH_25ns","Bp'+production+'Reco_LH")'])
            #call(['root', '-l',' limit.C("RH_25ns","Bp'+production+'Reco_RH")'])
            #print 'creating histograms',str(channel+'Bp'+production+'Reco_LH'), 'and',str(channel+'Bp'+production+'Reco_RH')
            #print 'Arguments', 'LH_25ns',str(channel+'Bp'+production+'Reco_LH'),dirstring
            call(['./../bin/rootfilecreator', 'Bp'+production+'_TW_*LH_25ns', str(channel+'Bp'+production+'Reco_LH'),dirstring,channel])
            call(['./../bin/rootfilecreator', 'Bp'+production+'_TW_*RH_25ns', str(channel+'Bp'+production+'Reco_RH'),dirstring,channel])

    execfile("histogram_rebinning.py")
    if rebin:
        for channel in channels:   
            print 'working on  the rebinning for', channel
            binFile(0.3, channel+'Bp'+production+'Reco_LH.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])
            binFile(0.3, channel+'Bp'+production+'Reco_RH.root', 'M_{B} [GeV/c^{2}]', ['ZJets','WJets', 'SingleTsChannel','SingleTtChannel', 'SingleTWAntitop','SingleTWTop','TTJets','QCD'])

    execfile("calculation.py")
    for channel in channels:
        print 'calculating limits for channel',channel,'and production',production
        exp_RH,obs_RH = run_cutopt(channel+"Bp"+production+"Reco_RH_rebinned.root","RH",channel,production,True)
        print 'calculated limits for: channel',channel,'production',production,'chirality RH'
        exp_LH,obs_LH = run_cutopt(channel+"Bp"+production+"Reco_LH_rebinned.root","LH",channel,production,True)
        print 'calculated limits for: channel',channel,'production',production,'chirality LH'
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
    plt.savefig("limit_comp_"+production+".pdf")

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
