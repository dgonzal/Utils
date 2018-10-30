# -*- coding: utf-8 -*-
from scipy.stats import chisqprob
import time
import copy
import ROOT
ROOT.TH1.AddDirectory(0)

execfile("PostFitUncertainties.py")
execfile("ThetaPostFitPlot.py")

from theta_auto import * 
from impact import *

def background_fit(fname, channel = "", write_report = True, glob_prefix="",signal=None):
    if write_report:report.reopen_file()
    print 'building model from',fname,'for mle fit'
    model = build_model_from_rootfile(fname,include_mc_uncertainties=True)
    model.fill_histogram_zerobins()
    uncer = 5
    #if not channel:
    #	uncer = 1.10

    #model.set_signal_processes('Bprime*')

    names_tup =[('Anti-b-tag_rate','Chi2_AntiBTag'),('1-b-tag_rate','Chi2_1_BTag'),('2-b-tag_rate','Chi2_2_BTag'),('W-tag_rate','Chi2_WTag'),('top-tag_rate','TopTag')]

    for unc in names_tup:       
        if channel =="Mu" or not channel:         
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Mu')
            #model.add_lognormal_uncertainty('norm_rate', math.log(uncer), procname='Background',obsname=unc[1]+'Mu')
        if channel =="Ele" or not channel:
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Ele') 
            #model.add_lognormal_uncertainty('norm_rate', math.log(uncer), procname='Background',obsname=unc[1]+'Ele')
    
    if signal:
        model.add_lognormal_uncertainty('signal_cross',math.log(uncer),procname=signal,obsname='*')

    #for p in model.processes:
    #    model.add_lognormal_uncertainty('lumi', math.log(1.025), p)
    #    model.add_lognormal_uncertainty('Wtag_unc', math.log(1.05), p, obsname='WTag*')
    #    model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')

            
    options = Options()
    

    
    #options.set('minimizer', 'strategy', 'robust')
    #model.restrict_to_observables(['TopTag'+channel,'Chi2_WTag'+channel,'Chi2_2_BTag'+channel,'Chi2_1_BTag'+channel])
    #model_summary(model, create_plots=True, shape_templates=True)
    mle_output = mle(model, input='data', n=4000, options=options,signal_process_groups ={"background_only":[]},chi2=True,with_covariance=True) #,signal_process_groups ={"background_only":[]}chi2=True
    mle_background = copy.deepcopy(mle_output)
    
    resultfile = fname.replace('.root','_fit_out.root')
    writeOutputFile(fname, resultfile, mle_output['background_only'], model, pdf = False)
    write_covariance_matrices(mle_output["background_only"],fname.replace('.root','_covariance.root'))
    print fname.replace('.root','_covariance.root')
    #exit(0)
    #write_histograms_to_rootfile(histos, channel+'_histos_mle.root')
    #write_histograms_to_rootfile(data_histos,'histos-mle.root')
    
    bin_sum = 0 
    for obs in model.get_observables():
	bin_sum +=  model.get_range_nbins(obs)[-1]

   
    print 'number of bins',bin_sum
    #print 'critical value 5%',math.sqrt(-0.5*math.log((0.05)/2))/math.sqrt(bin_sum+1)


    output_directory = channel+"background_data_fit"
    result =[]

       
    for pf_vals in mle_output.itervalues():
                #print pf_vals
                del pf_vals['__nll']
                best_fit = 0
                best_chi = 999999999999;
                for i, chival in enumerate(pf_vals['__chi2']):
                    #print chival
                    if chival < best_chi:
                        best_chi = chival
                        best_fit = i
	 	print best_chi, 'freedom',bin_sum-5, chisqprob(best_chi,bin_sum-5),'best fit',best_fit
                for key in pf_vals:
		    if '__ks' in key or '__chi2' in key or '__cov' in key:continue
                    wei_mean = float(pf_vals[key][best_fit][0])
                    mean_err = float(pf_vals[key][best_fit][1]) 
                    pf_vals[key] = (math.exp(wei_mean*math.log(uncer)), math.exp(mean_err*math.log(uncer)))
                    print key,'Mean',wei_mean,"Uncertainty", mean_err
                    result.append((key,math.exp(wei_mean*math.log(uncer)),math.exp(mean_err*math.log(uncer))-1))

    
    for pf_vals in mle_background.itervalues():
        del pf_vals['__chi2']
        #del pf_vals['__ks']
        del pf_vals['__cov']
        del pf_vals['__nll']

    output_directory = channel+"background_data_fit"

    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    #mle_background = mle(model, input='toys:0.0',n=5,signal_process_groups ={"background_only":[]} )
    #print mle_output
    #print mle_background
    postfit = ThetaPostFitPlot(mle_background)
    print output_directory
    postfit.make_plots(output_directory)
        
    #report.write_html(output_directory)    
    return result, resultfile
                    
def injected_signal_mc_limits(fname, Chirality, channel = "", particle = "BprimeB", write_report = True, glob_prefix="", mass='', beta_sig=0.0):
    if write_report:report.reopen_file()
    print 'building model from',fname
    model = build_model_from_rootfile(fname,include_mc_uncertainties=True)
    #model.fill_histogram_zerobins()

    #my_group = {'Signal_'+mass:['Signal_forward_'+mass,'Signal_central_'+mass]}
    my_group = {'Signal_'+mass:['Signal_forward_'+mass]}
    print 'signal',particle+'*'+Chirality+'*','for particle',particle
    if not mass:
        model.set_signal_processes(particle+'*'+Chirality+'*')
    else:
        model.set_signal_process_groups(my_group)
    print model.signal_processes


    eletrigger = 1.01
    mutrigger  = 1.02
    
    for p in model.processes:
        if "Background" not in p and not 'data' in p.lower():
            process_name = p
            if 'Width' in p:
                process_name =process_name.replace('10p','')
                process_name =process_name.replace('20p','')
                process_name =process_name.replace('30p','')
            sigmass = ''.join(x for x in process_name if x.isdigit())
	    wtag_unc = 3.9*math.log(float(sigmass)/2/200)            
            wtag_unc = math.sqrt(wtag_unc*wtag_unc+1)/100+1
            print p, 'Wtag_unc', wtag_unc
            model.add_lognormal_uncertainty('Wtag_unc', math.log(wtag_unc), p, obsname='WTag*')
            model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')            
            model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')
            if channel =="Ele" or not channel:
               model.add_lognormal_uncertainty('ele_trigger_rate'   , math.log(eletrigger), p, obsname='Chi2_AntiBTagEle')    
               model.add_lognormal_uncertainty('ele_trigger_rate'   , math.log(eletrigger), p, obsname='Chi2_1_BTagEle')    
               model.add_lognormal_uncertainty('ele_trigger_rate'   , math.log(eletrigger), p, obsname='Chi2_2_BTagEle')    
               model.add_lognormal_uncertainty('ele_trigger_rate'   , math.log(eletrigger), p, obsname='TopTagEle')         
            if channel =="Mu" or not channel:
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(eletrigger), p, obsname='Chi2_AntiBTagMu')    
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(eletrigger), p, obsname='Chi2_1_BTagMu')    
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(eletrigger), p, obsname='Chi2_2_BTagMu')    
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(eletrigger), p, obsname='TopTagMu')         

       
    #100% on the normalization
    uncer = 2.
    names_tup =[('Anti-b-tag_rate','Chi2_AntiBTag'),('1-b-tag_rate','Chi2_1_BTag'),('2-b-tag_rate','Chi2_2_BTag'),('W-tag_rate','Chi2_WTag'),('top-tag_rate','TopTag')]

    for unc in names_tup:
        if channel =="Mu"  or not channel:         
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Mu')
           
        if channel =="Ele" or not channel:
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Ele') 
       
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    #options.set('main', 'n_threads', '5')
    #options.set('global', 'debug', 'True')
    #mle_output = mle(model, input='data', n=100, options=options,signal_process_groups ={"background_only":[]},chi2=True,with_covariance=True) #,signal_process_groups ={"background_only":[]}

    exp = None
    obs = None
    discovery_val = None
    
    if not mass:
        exp, obs = bayesian_limits(model, 'all', n_toy = 200, n_data = 100, options=options)

    if mass:
       discovery_val= discovery(model, Z_error_max=0.2, maxit=10, n=500, n_expected=500, input_expected='data', options=options)
       #options.set('minimizer', 'minuit_tolerance_factor', '100')
       #impact(model,"impact_significance_"+mass+".pdf",options,1,my_group) 
    
    print 'Limits based on MC!'
    print "expected limit "+Chirality+' '+channel +":"
    print exp
    print "observed limit "+Chirality+' '+channel +":"
    print obs
    print 'discovery'
    #for item in discovery_val: 
    #    print item

    return exp,obs,discovery_val    
        
    #return exp,obs,discovery_val    
    #model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)

    #model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
    write_report=False
    
    if mass:
        #for i, signal in enumerate(model.signal_processes):
        #if mass and '_'+mass+'_' in signal:
        print signal
        #model.set_signal_processes(signal)
        mle_output = mle(model, input='data', n=1, options=options)
        print mle_output
        #continue
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
        #resultfile = fname.replace('.root','_fit_out.root')
        #writeOutputFile(fname, resultfile, mle_output['BprimeB_700_LH'], model, pdf = False)
   

    return exp,obs,discovery_val    

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

    
def best_signalfit(fname,mass):
    #if write_report:report.reopen_file()
    print 'building model from',fname,'for mle fit'
    model = build_model_from_rootfile(fname,include_mc_uncertainties=True)
    #model = build_model_from_rootfile(fname)
    model.fill_histogram_zerobins()
    uncer = 5
    sig_unc = 20
    # model.set_signal_processes('Bprime*')

    names_tup =[('Anti-b-tag_rate','Chi2_AntiBTag'),('1-b-tag_rate','Chi2_1_BTag'),('2-b-tag_rate','Chi2_2_BTag'),('W-tag_rate','Chi2_WTag'),('top-tag_rate','TopTag')]
    
    for unc in names_tup:
        #if channel =="Mu"  or not channel:         
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Mu')
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Signal_central_'+mass,obsname=unc[1]+'Mu')
        #if channel =="Ele" or not channel:
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Background',obsname=unc[1]+'Ele') 
            model.add_lognormal_uncertainty(unc[0], math.log(uncer), procname='Signal_central_'+mass,obsname=unc[1]+'Ele')
    
    model.add_lognormal_uncertainty('signal_cross',math.log(sig_unc),procname='Signal_central_'+mass,obsname='*')
    model.add_lognormal_uncertainty('signal_cross',math.log(sig_unc),procname='Signal_forward_'+mass,obsname='*')
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    options.set('main', 'n_threads', '5')

    #model.restrict_to_observables(['TopTag'+channel,'Chi2_WTag'+channel,'Chi2_2_BTag'+channel,'Chi2_1_BTag'+channel])
    #model_summary(model, create_plots=True, shape_templates=True)
    mle_output = mle(model, input='data', n=1000, options=options,signal_process_groups ={"background_only":[]},with_error=True,chi2=True,with_covariance=False) #,signal_process_groups ={"background_only":[]}

    resultfile = fname.replace('.root','_bestfit_out.root')
    writeOutputFile(fname, resultfile, mle_output['background_only'], model, pdf = False)
    
    bin_sum = 0 
    for obs in model.get_observables():
	bin_sum +=  model.get_range_nbins(obs)[-1]

    print 'number of bins',bin_sum
    #print 'critical value 5%',math.sqrt(-0.5*math.log((0.05)/2))/math.sqrt(bin_sum+1)

    signal = None

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
                    pf_vals[key] = (math.exp(wei_mean*math.log(uncer)))
                    print key,'Mean',math.exp(wei_mean*math.log(uncer)),"Uncertainty", math.exp(wei_mean*math.log(uncer))*mean_err*math.log(uncer)
	            if 'signal_cross' in key: signal =  [math.exp(wei_mean*math.log(sig_unc)),math.exp(wei_mean*math.log(sig_unc))*mean_err*math.log(sig_unc)]
    return signal
