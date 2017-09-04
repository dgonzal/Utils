execfile("ThetaPostFitPlot.py")
execfile("LimitPlot.py")
import numpy
#matplotlib.rcParams['text.usetex']=True
#matplotlib.rcParams['text.latex.unicode']=True


# -*- coding: utf-8 -*-
def run_cutopt(fname, Chirality, channel = "", particle = "b", write_report = True):
    if write_report:report.reopen_file()

    model = build_model_from_rootfile(fname)
    #model.scale_predictions(5.0 / 1.1)
    model.fill_histogram_zerobins()
    model.set_signal_processes('Bprime'+particle+'*'+Chirality+'*')

    for p in model.processes:
        model.add_lognormal_uncertainty('lumi', math.log(1.048), p)
        if 'W' in fname or 'Combi' in fname:
            model.add_lognormal_uncertainty('Wtag_unc', math.log(1.05), p, obsname='WTag*')
        if ('TopTag' in fname or 'Combi' in fname) and not 'SingleT_s' in p:
            model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')    
        
    model.add_lognormal_uncertainty('ZJets_rate', math.log(1.20), 'ZJets')
    model.add_lognormal_uncertainty('WJets_rate', math.log(1.20), 'WJets_Pt')
    if 'TopTag' not in fname: model.add_lognormal_uncertainty('SingleTsChannel_rate', math.log(1.50), 'SingleT_s')
    model.add_lognormal_uncertainty('SingleTtChannel_rate', math.log(1.50), 'SingleT_t')
    model.add_lognormal_uncertainty('SingleTWAntitop_rate', math.log(1.50), 'SingleTWAntitop')
    model.add_lognormal_uncertainty('SingleTWTop_rate', math.log(1.50), 'SingleTWtop')
    model.add_lognormal_uncertainty('QCD_rate', math.log(2.0), 'QCD')
    model.add_lognormal_uncertainty('TTbar_rate', math.log(1.20), 'TTbar')
    
    #model_summary(model)
    model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)

    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    exp, obs = asymptotic_cls_limits(model, use_data=False, signal_process_groups=None, beta_signal_expected=0.0, bootstrap_model=True, input=None, n=10, options=options)
    #exp, obs = bayesian_limits(model, 'all', n_toy = 20000, n_data = 2000,options=options)


    
    if write_report:
        output_directory =""
        if channel:
            output_directory = './output_Bprime'+particle+'_'+Chirality+'_'+channel+'/'
        else:
             output_directory = './output_Bprime'+particle+'_'+Chirality+'/'
            
        report.write_html(output_directory)

        plot_limits(particle,Chirality,channel,exp,obs)
        """
        result = mle(model, input='data',n=5, options=options)
        bs = []
        delta_bs = []
        pulls = []

        for b, db in result['s']['beta_signal']:
            bs.append(b)
            delta_bs.append(db)
            pulls.append((1 - b)/db)
    
        pdbs = plotdata()
        pdbs.histogram(bs, 0.0, 2.0, 30, include_uoflow = True)
        plot(pdbs, 'bs', 'ntoys', 'beta_signal.pdf')
    
        pdd = plotdata()
        pdd.histogram(delta_bs, 0.0, 1.0, 30, include_uoflow = True)
        plot(pdd, 'dbs', 'ntoys', 'delta_beta_signal.pdf')
        
        pdp = plotdata()
        pdp.histogram(pulls, -3.0, 3.0, 100, include_uoflow = True)
        plot(pdp, 'print()ull', 'ntoys', 'pull.pdf')
    

        # to write the data # TODO: o a file, use e.g.:
        pdp.write_txt('pull.txt')
        """

    #write_histograms_to_rootfile(model,)
    #evaluate_prediction(model)

   
    print "expected limit "+Chirality+' '+channel +":"
    print exp
    #print "observed limit "+Chirality+' '+channel +":"
    #print obs
    return exp,obs
    
