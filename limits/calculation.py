execfile("ThetaPostFitPlot.py")

# -*- coding: utf-8 -*-
def run_cutopt(fname, Chirality, write_report = True):
    if write_report:report.reopen_file()

    model = build_model_from_rootfile(fname)
    #model.scale_predictions(5.0 / 1.1)
    model.fill_histogram_zerobins()
    model.set_signal_processes('Bpt_TW*_'+Chirality+'*')
    for p in model.processes:
        model.add_lognormal_uncertainty('lumi', math.log(1.048), p)
    
    model.add_lognormal_uncertainty('ZJets_rate', math.log(1.20), 'ZJets')
    model.add_lognormal_uncertainty('WJets_rate', math.log(1.08), 'WJets')
    model.add_lognormal_uncertainty('SingleTsChannel_rate', math.log(1.20), 'SingleTsChannel')
    model.add_lognormal_uncertainty('SingleTtChannel_rate', math.log(1.20), 'SingleTtChannel')
    model.add_lognormal_uncertainty('SingleTWAntitop_rate', math.log(1.20), 'SingleTWAntitop')
    model.add_lognormal_uncertainty('SingleTWTop_rate', math.log(1.20), 'SingleTWTop')
    model.add_lognormal_uncertainty('QCD_rate', math.log(2.0), 'QCD')
    model.add_lognormal_uncertainty('TTbar_rate', math.log(1.16), 'TTJets')
    
    model_summary(model)

    #dist = model.distribution
    #model.distribution = get_fixed_dist(dist)

    #for r in runs: run_theta(options, in_background_thread = True)
    #for r in runs: r.wait_for_result_available()

    #exp, obs = asymptotic_cls_limits(model, use_data=True, signal_process_groups=None, beta_signal_expected=0.0, bootstrap_model=True, input=None, n=1, options=None)
    exp, obs = bayesian_limits(model, 'all', n_toy = 20000, n_data = 2000)
         

    if write_report:
        output_directory = './output_'+ Chirality+'/'
        report.write_html(output_directory)
        postfit = ThetaPostFitPlot(mle(model,'data',n=1))
        postfit.make_plots(output_directory)

    print "expected limit "+Chirality+":"
    print exp
    print "observed limit "+Chirality+":"
    print obs
        
    #x = np.linspace(0, 2, 100)
    #plt.title("B+t -> tW 100%")
    #plt.ylim([0.4,10])

    theroy13TeV_x =[800,900,1000,1100,1200,1300,1400,1500]
    theory13TeV_y =[0.365,0.271,0.203,0.152,0.116,0.0894,0.0692,0.0540]
    
    plt.clf()
    plt.semilogy()
    plt.plot(exp.x, exp.y, label= Chirality+' Chirality', color ='black',linestyle='--')#,color = exp_LH.bands[0][2])
    plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
                     alpha=0.6, facecolor='yellow', edgecolor='yellow',
                     linewidth=0)
    plt.fill_between(exp.x, exp.bands[1][0] ,  exp.bands[1][1],
                     alpha=0.8, facecolor = 'green', edgecolor='green', # exp_LH.bands[1][2],
                     linewidth=0)

    plt.plot(theroy13TeV_x, theory13TeV_y, label='Theory cross section 13TeV')
    plt.plot(obs.x, obs.y, label='Obs', color ='black')
    plt.xlabel('Mass B [GeV]')
    plt.ylabel('cross section times branching ratio [pb]')
    plt.legend(loc=2,prop={'size':7})
    plt.savefig("limit_"+Chirality+".pdf")
    return exp,obs
    """
    plt.clf()
    plt.semilogy()
    plt.plot(exp_RH.x, exp_RH.y, label='RH Chirality', color ='black',linestyle='--')#,color = exp_RH.bands[0][2])
    plt.fill_between(exp_RH.x, exp_RH.bands[0][0] ,  exp_RH.bands[0][1],
                     alpha=0.6, facecolor='yellow', edgecolor='yellow',
                     linewidth=0)
    plt.fill_between(exp_RH.x, exp_RH.bands[1][0] ,  exp_RH.bands[1][1],
                     alpha=0.8, facecolor = 'green', edgecolor='green', # exp_RH.bands[1][2],
                     linewidth=0)

    plt.plot(theroy13TeV_x, theory13TeV_y, label='Theory cross section 13TeV')
    plt.plot(obs_RH.x, obs_RH.y, label='Obs', color ='black')
    plt.xlabel('Mass B [GeV]')
    plt.ylabel('cross section times branching ratio [pb]')
    plt.legend(loc=2,prop={'size':7})
    plt.savefig("limit_RH.pdf")
    """
    
