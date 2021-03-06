from subprocess import call
import os

channels = ["Mu"]#['Mu',"Ele",""]
production_channels = ["B","T"]#,"t"]#"b",
chiralitys = ["RH","LH"]

release = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/'
Mudirs = ['config/MuSel_v5/']
Eledirs = ['config/EleSel_v3/']
rootDir = "ROOTFiles/"
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
            if (createfiles or not os.path.exists(rootDir+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root")) and limits: 
                call(['./../bin/rootfilecreator', signal_pre+production+'*'+chirality, str(rootDir+channel+'Bp'+production+'Reco_'+chirality),dirstring,channel])
            execfile("histogram_rebinning.py")
            if (rebin or not os.path.exists(rootDir+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root")) and limits:   
                print 'working on  the rebinning for', channel
                binFile(0.3, rootDir+channel+'Bp'+production+'Reco_'+chirality+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
            
    
            execfile("calculation_datadriven.py")
            if limits:
                print 'calculating limits for channel',channel,'ass. production',production,'chirality',chirality
                exp,obs = run_cutopt(rootDir+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root",chirality,channel,production,True,"",0.0,prefix)
                print 'calculated limits for: channel',channel,'ass. production',production,'chirality',chirality

            if signal_injection:
                sigma2_color = 'yellow' #"yellow"
                sigma1_color = 'darkgreen' #"green"
                
                pp = PdfPages("InjectedSignals_Bprime"+production+"_"+chirality+'_'+channel+".pdf")
                """
                plt.clf()
                plt.semilogy()
                plt.title("2.2 fb$^{-1}$ (13 TeV)", fontsize=10)# , loc='right')   
                plt.plot(exp.x, exp.y, label="Exp $95\%$ CL" , color ='black',linestyle='dotted')#,color = exp_LH.bands[0][2])
                plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
                                 alpha=0.6, facecolor=sigma2_color, edgecolor=sigma2_color,
                                 linewidth=0, label="$\pm$ 2 std. deviation")
                plt.fill_between(exp.x, exp.bands[1][0] ,  exp.bands[1][1],
                                 alpha=0.8, facecolor=sigma1_color, edgecolor=sigma1_color, # exp_LH.bands[1][2],
                                 linewidth=0, label="$\pm$ 1 std. deviation"
                                 )
                """
                ##
                ## signal injection with theory cross section
                ##
                
                #For B+t or X5/3:
                theory13TeV_x =[800,900,1000,1100,1200,1300,1400,1500]
                theory13TeV_y =[0.365,0.271,0.203,0.152,0.116,0.0894,0.0692,0.0540]

                #For B+b
                if production is 'b':
                    print 'inject b'
                    theory13TeV_x =[800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
                    theory13TeV_y =numpy.array([3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.249,0.195])
                    theory13TeV_y =theory13TeV_y*0.5
                #For B+t since I did not process all files:
                if production is 't':
                    print 'inject t'
                    theory13TeV_x =[800,1000,1100,1500]
                    theory13TeV_y =numpy.array([0.365,0.203,0.152,0.0540])


                for i in xrange(len(theory13TeV_x)):
                    print "injecting signal for",production,"Mass",int(theory13TeV_x[i]),"with cross section",theory13TeV_y[i]
                    if not os.path.exists(rootDir+'InjectedSignal_M'+str(int(theory13TeV_x[i]))+'_'+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root") or createfiles:
                        call(['./../bin/rootfilecreator', str('Bp'+production+'_TW_*'+chirality+'_25ns'), str(rootDir+"InjectedSignal_M"+str(int(theory13TeV_x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality),dirstring,channel,str('Bp'+production+'_TW_'+str(int(theory13TeV_x[i]))+'_'+chirality+'_25ns'),str(theory13TeV_y[i])], shell=False)
                        binFile(0.3, rootDir+'InjectedSignal_M'+str(int(theory13TeV_x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
                    exp_i, obs_i = run_cutopt(rootDir+'InjectedSignal_M'+str(int(theory13TeV_x[i]))+'_'+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root",chirality,channel,production,True,str('Bp'+production+'_TW_'+str(int(theory13TeV_x[i]))))
                    plt.plot(exp_i.x, exp_i.y, color ='blue',linestyle='dotted')#,color = exp_LH.bands[0][2])

                """
                ##
                ## Two sigma band signal injection
                ##
                for i in xrange(len(exp.bands[0][0])):
                    call(['./../bin/rootfilecreator', str('Bp'+production+'_TW_*'+chirality+'_25ns'), str(rootDir+"InjectedSignal_M"+str(int(exp.x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality),dirstring,channel,str('Bp'+production+'_TW_'+str(int(exp.x[i]))+'_'+chirality+'_25ns'),str(exp.bands[0][0][i])], shell=False)
                    binFile(0.3, rootDir+'InjectedSignal_M'+str(int(exp.x[i]))+'_'+channel+'Bp'+production+'Reco_'+chirality+'.root', 'M_{B} [GeV/c^{2}]', ['Background'])
                    exp_i, obs_i = run_cutopt(rootDir+'InjectedSignal_M'+str(int(exp.x[i]))+'_'+channel+"Bp"+production+"Reco_"+chirality+"_rebinned.root",chirality,channel,production,True,str('Bp'+production+'_TW_'+str(int(exp.x[i]))))
                    plt.plot(exp_i.x, exp_i.y, color ='blue',linestyle='dotted')#,color = exp_LH.bands[0][2])
                    pdf.savefig()
                """
                pp.close()
        
                
                


def write_sh(name):
    myfile = open(name+'.sh','w')
    myfile.write(
        """#!/bin/bash
##This is a simple example of a SGE batch script
##Use home server with scientific linux 6 
#$ -l os=sld6 
#$ -l site=hh 
#$ -cwd
##You need to set up sframe
#$ -V 
##email Notification
#$ -m as
#$ -M daniel.gonzalez@desy.de
##running in local mode with 8-12 cpu slots
##$ -pe local 8-12
##CPU memory
#$ -l h_vmem=2G
##DISK memory
#$ -l h_fsize=2G   
""")    
    return myfile



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
