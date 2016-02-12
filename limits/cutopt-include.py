# -*- coding: utf-8 -*-
def run_cutopt(fname, write_report = True):
    model = build_model_from_rootfile(fname)
    #model.scale_predictions(5.0 / 1.1)
    model.fill_histogram_zerobins()
    model.set_signal_processes('Bpt_TW*_LH*')
    for p in model.processes:
        model.add_lognormal_uncertainty('lumi', math.log(1.046), p)
     
    model.add_lognormal_uncertainty('ZJetsM50toInf_rate', math.log(1.20), 'ZJetsM50toInf')
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
    #res = pl_intervals(model, input = 'toys-asimov:0.0', n = 1, write_report = False, nuisance_constraint = dist)
    #exp_LH, obs = bayesian_quantiles(model, input= 'toys-asimov:0', n=1, quantiles=[0.95], signal_process_groups=None, nuisance_constraint=None, nuisance_prior_toys=None, signal_prior='flat', options=None, parameter='beta_signal', iterations=10000, run_theta=True, seed=0, hint_method='asimov-ll')

    #for r in runs: run_theta(options, in_background_thread = True)
    #for r in runs: r.wait_for_result_available()

    #return
    #exp_LH, obs = asymptotic_cls_limits(model, use_data=False, signal_process_groups=None, beta_signal_expected=0.0, bootstrap_model=True, input=None, n=1, options=None)
    exp_LH, obs = bayesian_limits(model, 'all', n_toy = 200, n_data = 20)
    mle(model,'data',n=1)
   
    
    model.set_signal_processes('Bpt_TW*_RH*')
    #exp_RH, obs2 = asymptotic_cls_limits(model, use_data=False, signal_process_groups=None, beta_signal_expected=0.0, bootstrap_model=True, input=None, n=1, options=None)
    exp_RH, obs2 = bayesian_limits(model, 'all', n_toy = 200, n_data = 20)
    mle(model,'data',n=1)
    if write_report: report.write_html('./htmlout/')

    print "expected limit LH:"
    print exp_LH
    print "expected limit RH:"
    print exp_RH
    print "observed limit:"
    print obs
    
    #print "end"
    
    

    plt.clf()

    #x = np.linspace(0, 2, 100)
    theroy13TeV_x =[800,900,1000,1100,1200]
    #theory8TeV_y =[0.745,0.497,0.325,0.215,0.146]
    theory13TeV_y =[0.431,0.308,0.223,0.164,0.122]

    plt.semilogy()
    plt.plot(exp_LH.x, exp_LH.y, label='LH',color = exp_LH.bands[0][2])
    plt.fill_between(exp_LH.x, exp_LH.bands[0][0] ,  exp_LH.bands[0][1],
                     alpha=0.2, edgecolor=exp_LH.bands[0][2], facecolor=exp_LH.bands[0][2],
                     linewidth=2)



    plt.plot(exp_RH.x, exp_RH.y, label='RH',color = 'red')
    plt.fill_between(exp_RH.x, exp_RH.bands[0][0] ,  exp_RH.bands[0][1],
                     alpha=0.2, edgecolor='red', facecolor='red',
                     linewidth=2)

    plt.plot(theroy13TeV_x, theory13TeV_y, label='Theory cross section 13TeV')
    #theory8TeV_y = [x*4 for x in theory8TeV_y]
    #plt.plot(theroy8TeV_x, theory8TeV_y, label='Theory cross section 8TeV times 4', color='black')

    #plt.ylim([0.4,10])
    plt.xlabel('Mass B [GeV]')
    plt.ylabel('cross section times branching ratio [pb]')

    plt.title("B+t -> tW 100%")
    
    plt.legend(loc=2,prop={'size':7})
    plt.savefig("limit.pdf")
