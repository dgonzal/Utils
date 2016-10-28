from subprocess import call
import os

channels = ['Mu']#,"Ele",""]
production_channels = ["b"]#,"t"]
chiralitys = ["RH","LH"]

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/'
Mudirs = ['config/Selection_v40/']
Eledirs = ['config/EleSelection_v7_tree/']
rootDir = "ROOTFiles/"
createfiles = True
rebin = True
signal_injection = True

if not rebin:
    createfiles = False

if not os.path.exists(rootDir):
    os.makedirs(rootDir)



for chirality in chiralitys:
    for production in production_channels:
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
            if dirstring[-1]==',': dirstring = dirstring[:-1]
            if createfiles:
                call(['./../bin/rootfilecreator', 'Bp'+production+'_TW_*'+chirality+'_25ns', str(rootDir+channel+'Bp'+production+'Reco_'+chirality),dirstring,channel])
            execfile("histogram_rebinning.py")
            if rebin:   
                print 'working on  the rebinning for', channel
                binFile(0.3, rootDir+channel+'Bp'+production+'Reco_'+chirality+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
            
    
            execfile("calculation_datadriven.py")
            print 'calculating limits for channel',channel,'ass. production',production,'chirality',chirality
            exp,obs = run_cutopt(rootDir+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root",chirality,channel,production,True)
            print 'calculated limits for: channel',channel,'ass. production',production,'chirality',chirality

            if signal_injection:
                for i in xrange(len(exp.bands[0][0])):
                    """
                    print './../bin/rootfilecreator', 'Bp'+production+'_TW_*'+chirality+'_25ns', str('InjectedSignal_M'+str(int(exp.x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality),dirstring,channel,str('Bp'+production+'_TW_'+str(int(exp.x[i]))+'_'+chirality+'_25ns'),str(exp.bands[0][0][i])
                    print 0.4, 'InjectedSignal_M'+str(int(exp.x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality+'.root', 'M_{B} [GeV/c^{2}]', ['Background']
                    print 'InjectedSignal_M'+str(int(exp.x[i]))+'_'+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root",chirality,channel,production,True
                    continue
                    """
                    call(['./../bin/rootfilecreator', str('Bp'+production+'_TW_*'+chirality+'_25ns'), str(rootDir+"InjectedSignal_M"+str(int(exp.x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality),dirstring,channel,str('Bp'+production+'_TW_'+str(int(exp.x[i]))+'_'+chirality+'_25ns'),str(exp.bands[0][0][i])], shell=False)
                    binFile(0.3, rootDir+'InjectedSignal_M'+str(int(exp.x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
                    run_cutopt(rootDir+'InjectedSignal_M'+str(int(exp.x[i]))+'_'+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root",chirality,channel,production,True,str('Bp'+production+'_TW_'+str(int(exp.x[i]))))



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
    plt.savefig("limit_comp_"+production+".pdf")


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
