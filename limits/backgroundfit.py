# -*- c:oding: utf-8 -*-
from scipy.stats import chisqprob


def background_fit(fname, channel = "", write_report = True, glob_prefix=""):
    #if write_report:report.reopen_file()


    
    model = build_model_from_rootfile(fname)
    model.fill_histogram_zerobins()
    uncer = 3.


    names_tup =[('Anti-b-tag_rate','Chi2_AntiBTag'),('1-b-tag_rate','Chi2_1_BTag'),('2-b-tag_rate','Chi2_2_BTag'),('W-tag_rate','Chi2_WTag'),('top-tag_rate','TopTag')]


    """
    if channel =="Mu" or not channel:
        print 'setting Mu uncer'
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagMu')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagMu')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagMu')
        model.add_lognormal_uncertainty('W-tag_rate'     , math.log(uncer), procname='Background',obsname='Chi2_WTagMu')
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(uncer), procname='Background',obsname='TopTagMu')    
    if channel =="Ele" or not channel:
        print 'setting Ele uncer'
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagEle')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagEle')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagEle')
        model.add_lognormal_uncertainty('W-tag_rate'     , math.log(uncer), procname='Background',obsname='Chi2_WTagEle')
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(uncer), procname='Background',obsname='TopTagEle')         
    """
    for unc in names_tup:
      if channel =="Mu"  or not channel:model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Mu') 
      if channel =="Ele" or not channel:model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Ele') 
	

    model_summary(model)
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    mle_output = mle(model, input='data', n=1, options=options,signal_process_groups ={"background_only":[]},ks=True, chi2=True,with_covariance=False)
    #print mle_output
    #z_output = zvalue_approx(model, input='data', n=1, signal_process_groups={"background_only":[]},options=options, eventid_info=False, signal_prior_sb='flat')    

    """
    print mle_output
    parameter_values = {}
    for p in model.get_parameters([]):
    	parameter_values[p] = mle_output['background_only'][p][0][0]
    histos = evaluate_prediction(model, parameter_values, include_signal = False)
    write_histograms_to_rootfile(histos, 'histos-mle.root')
    """
 


    bin_sum = 0 
    for obs in model.get_observables():
	bin_sum +=  model.get_range_nbins(obs)[-1]

    print 'number of bins',bin_sum
    #print 'critical value 5%',math.sqrt(-0.5*math.log((0.05)/2))/math.sqrt(bin_sum+1)

    result =[]
    
    for pf_vals in mle_output.itervalues():
                #print pf_vals
                del pf_vals['__nll']
	 	chi2_val = pf_vals['__chi2'][0]
		print chi2_val, 'freedom',bin_sum-1, chisqprob(chi2_val,bin_sum-1)
		#print pf_vals[unc[0]][0][0]
		#for unc in names_tup:
		#   if channel =="Mu"  or not channel: model.scale_predictions(math.exp(float(pf_vals[unc[0]][0][0])), procname='Background', obsname=unc[1]+'Mu')
		#   if channel =="Ele" or not channel: model.scale_predictions(math.exp(float(pf_vals[unc[0]][0][0])), procname='Background', obsname=unc[1]+'Ele')
                for key in pf_vals:
		    if '__ks' in key or '__chi2' in key:continue
                    #print key
                    vals = list(a for a, _ in pf_vals[key])
                    #print 'vals',vals
                    errs = list(b for _, b in pf_vals[key])
                    #print 'errs',errs
                    weis = list(1/b for b in errs)
                    #print 'weis',weis
                    #wei_mean = numpy.average(vals, weights=weis)
                    wei_mean = numpy.average(vals)
                    mean_err = numpy.average(errs)
                    pf_vals[key] = (math.exp(wei_mean), math.exp(mean_err))
                    #print wei_mean, mean_err
                    print key,'Mean',math.exp(wei_mean),"Error",math.exp(mean_err)-1
                    result.append((key,math.exp(wei_mean),math.exp(mean_err)-1))

    return result


    mle_output = mle(model, input='data', n=1, options=options,signal_process_groups ={"background_only":[]},ks=True, chi2=True,with_covariance=False)
    for pf_vals in mle_output.itervalues():
                print pf_vals
                del pf_vals['__nll']
                chi2_val = pf_vals['__chi2'][0]
                print chisqprob(chi2_val,bin_sum-1)


                    
    return result


    
