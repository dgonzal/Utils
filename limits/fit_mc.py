# -*- coding: utf-8 -*-
from scipy.stats import chisqprob
import time
execfile("PostFitUncertainties.py")

def injected_signal_mc_limits(fname, Chirality, channel = "", particle = "BprimeB", write_report = True, glob_prefix="", mass='', beta_sig=0.0):
    if write_report:report.reopen_file()
    model = build_model_from_rootfile(fname)
    model.fill_histogram_zerobins()
    
    print 'signal',particle+'*'+Chirality+'*','for particle',particle
    print 'using file', fname, 'to build the model'
    print 'injected mass',mass
    model.set_signal_processes(particle+'*'+Chirality+'*')
    for p in model.processes:
        model.add_lognormal_uncertainty('lumi', math.log(1.026), p)
        model.add_lognormal_uncertainty('Wtag_unc', math.log(1.07), p, obsname='WTag*')
        model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')
    if channel =="Ele" or not channel:
        model.add_lognormal_uncertainty('ele_scale_rate', math.log(1.04), p, obsname='*')
    if channel =="Mu" or not channel:
        model.add_lognormal_uncertainty('muon_scale_rate', math.log(1.02), p, obsname='*')
        
    model.add_lognormal_uncertainty('QCD_rate', math.log(3.00), 'QCD')
    model.add_lognormal_uncertainty('ZJets_rate', math.log(1.50), 'ZJets')
    model.add_lognormal_uncertainty('WJets_rate', math.log(1.50), 'WJets')
    model.add_lognormal_uncertainty('SingleT_rate', math.log(1.50), 'SingleT')
    model.add_lognormal_uncertainty('TTbar_rate', math.log(1.50), 'TTbar')
    
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    options.set('main', 'n_threads', '15')
    #options.set('main', 'n_threads', '4')


    #discovery_options =  Options()
    #discovery_options.set('main', 'n_threads', '10')
    #mle_output = mle(model, input='data', n=100, options=options,signal_process_groups ={"background_only":[]},chi2=True,with_covariance=True) #,signal_process_groups ={"background_only":[]}

    exp = None
    obs = None
    zvalue = None

   
    exp, obs = bayesian_limits(model, 'all', n_toy = 500, n_data = 200, options=options)

    discovery_val = []
    for i, signal in enumerate(model.signal_processes):
        if mass and '_'+mass+'_' in signal: 
            discovery_val.append(discovery(model, spid=signal , Z_error_max=0.2, maxit=20, n=200, n_expected=200, input_expected='toys:1.0', options=options))
    #discovery_val = pvalue(model, input='data', n=1, options=options, bkgtoys_n_runs=5, bkgtoys_n=200, bkgtoys_seed_min=1)
    #discovery_val = p_to_Z(discovery_val)
    #exp, obs = asymptotic_cls_limits(model, use_data=False, beta_signal_expected=beta_sig, bootstrap_model=True, n=1, options=options)

    print 'Limits based on MC!'
    print "expected limit "+Chirality+' '+channel +":"
    print exp
    print "observed limit "+Chirality+' '+channel +":"
    print obs
    print 'discovery'
    print discovery_val
    #for item in discovery_val: 
    #    print item
                             
    return exp,obs,discovery_val

    model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
    
    output_directory = './'+glob_prefix+'MC_output_'+particle+'_'+Chirality+'_'+channel+'/'
    if write_report:
        print 'Going to store results in',output_directory
        if not os.path.exists(output_directory):
            os.makedirs(output_directory)
        report.write_html(output_directory)

    print 'Limits based on MC!'
    print "expected limit "+Chirality+' '+channel +":"
    print exp
    print "observed limit "+Chirality+' '+channel +":"
    print obs

    return exp,obs,discovery_val









    
    
