# -*- coding: utf-8 -*-
from scipy.stats import chisqprob
import time
execfile("PostFitUncertainties.py")



def background_fit(fname, channel = "", write_report = True, glob_prefix="",signal=None):
    #if write_report:report.reopen_file()
    print 'building model from',fname,'for mle fit'
    model = build_model_from_rootfile(fname,include_mc_uncertainties=True)
    #model = build_model_from_rootfile(fname)
    model.fill_histogram_zerobins()
    uncer = 5
   # model.set_signal_processes('Bprime*')

    names_tup =[('Anti-b-tag_rate','Chi2_AntiBTag'),('1-b-tag_rate','Chi2_1_BTag'),('2-b-tag_rate','Chi2_2_BTag'),('W-tag_rate','Chi2_WTag'),('top-tag_rate','TopTag')]

    for unc in names_tup:
        if channel =="Mu"  or not channel:         
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Mu')

        if channel =="Ele" or not channel:
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Ele') 

    if signal:
        model.add_lognormal_uncertainty('signal_cross',math.log(uncer),procname=signal,obsname='*')

    #for p in model.processes:
    #    model.add_lognormal_uncertainty('lumi', math.log(1.048), p)
    #    model.add_lognormal_uncertainty('Wtag_unc', math.log(1.05), p, obsname='WTag*')
    #    model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')

            
    options = Options()
    #options.set('minimizer', 'strategy', 'robust')
    #model.restrict_to_observables(['TopTag'+channel,'Chi2_WTag'+channel,'Chi2_2_BTag'+channel,'Chi2_1_BTag'+channel])
    model_summary(model, create_plots=True, shape_templates=True)
    mle_output = mle(model, input='data', n=1000, options=options,signal_process_groups ={"background_only":[]},chi2=True,with_covariance=True) #,signal_process_groups ={"background_only":[]}

    writeOutputFile(fname, fname.replace('.root','_fit_out.root'), mle_output['background_only'], model, pdf = False)

    #write_histograms_to_rootfile(histos, channel+'_histos_mle.root')
    #write_histograms_to_rootfile(data_histos,'histos-mle.root')
    
    bin_sum = 0 
    for obs in model.get_observables():
	bin_sum +=  model.get_range_nbins(obs)[-1]

    print 'number of bins',bin_sum
    #print 'critical value 5%',math.sqrt(-0.5*math.log((0.05)/2))/math.sqrt(bin_sum+1)

    result =[]

    for pf_vals in mle_output.itervalues():
                #print pf_vals
                del pf_vals['__nll']
                best_fit = 0
                best_chi = 999999999999;
                for i, chival in enumerate(pf_vals['__chi2']):
                    if chival < best_chi:
                        best_chi = chival
                        best_fit = i
                
	 	print best_chi, 'freedom',bin_sum-1, chisqprob(best_chi,bin_sum-1)
                for key in pf_vals:
		    if '__ks' in key or '__chi2' in key or '__cov' in key:continue
                    
                    wei_mean = float(pf_vals[key][best_fit][0])
                    mean_err = float(pf_vals[key][best_fit][1]) 
                    pf_vals[key] = (math.exp(wei_mean*math.log(uncer)), math.exp(mean_err*math.log(uncer)))
                    print key,'Mean',math.exp(wei_mean*math.log(uncer)),"Error",math.exp(mean_err*math.log(uncer))-1
                    result.append((key,math.exp(wei_mean*math.log(uncer)),math.exp(mean_err*math.log(uncer))-1))


    output_directory = channel+"_Test_mle_fit"
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)
        report.write_html(output_directory)
                    
    return result



    
def injected_signal_mc_limits(fname, Chirality, channel = "", particle = "BprimeB", write_report = True, glob_prefix="", mass='', beta_sig=0.0):
    if write_report:report.reopen_file()
    model = build_model_from_rootfile(fname)
    model.fill_histogram_zerobins()
    
    print 'signal',particle+'*'+Chirality+'*','for particle',particle
    model.set_signal_processes(particle+'*'+Chirality+'*')
    for p in model.processes:
        if "Background" not in p: 
            model.add_lognormal_uncertainty('lumi', math.log(1.026), p)
            model.add_lognormal_uncertainty('Wtag_unc', math.log(1.07), p, obsname='WTag*')
            model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')
            
    #400% on the normalization
    uncer = 5.
    names_tup =[('Anti-b-tag_rate','Chi2_AntiBTag'),('1-b-tag_rate','Chi2_1_BTag'),('2-b-tag_rate','Chi2_2_BTag'),('W-tag_rate','Chi2_WTag'),('top-tag_rate','TopTag')]

    for unc in names_tup:
        if channel =="Mu"  or not channel:         
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Mu')

        if channel =="Ele" or not channel:
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Ele') 

    
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    #options.set('main', 'n_threads', '4')
     
    #mle_output = mle(model, input='data', n=100, options=options,signal_process_groups ={"background_only":[]},chi2=True,with_covariance=True) #,signal_process_groups ={"background_only":[]}

    exp = None
    obs = None
    zvalue =  zvalue_approx(model, input='data', n=1, options=options)
    exp, obs = bayesian_limits(model, 'all', n_toy = 200, n_data = 100, options=options)
    #exp, obs = asymptotic_cls_limits(model, use_data=True, beta_signal_expected=beta_sig, bootstrap_model=True, n=1, options=options)

    print 'Limits based on MC!'
    print "expected limit "+Chirality+' '+channel +":"
    print exp
    print "observed limit "+Chirality+' '+channel +":"
    print obs
    return exp,obs,zvalue

    #model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
    write_report=False
    if mass: 
        mle_output = mle(model, input='data', n=1, options=options,signal_process_groups ={glob_prefix:[particle+'-'+mass+'_'+Chirality]})
    
        for pf_vals in mle_output.itervalues():
            #print pf_vals
            del pf_vals['__nll']
            #chi2_val = pf_vals['__chi2'][0]
	    #print chi2_val, 'freedom',bin_sum-1, chisqprob(chi2_val,bin_sum-1)
            for key in pf_vals:
		if '__ks' in key or '__chi2' in key:continue
                vals = list(a for a, _ in pf_vals[key])
                errs = list(b for _, b in pf_vals[key])
                weis = list(1/b for b in errs)
                wei_mean = numpy.average(vals)
                mean_err = numpy.average(errs)
                pf_vals[key] = (math.exp(wei_mean), math.exp(mean_err))
                print key,'Mean',math.exp(wei_mean),"Error",math.exp(mean_err)-1
    
    
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

    return exp,obs,zvalue

    










    
    
