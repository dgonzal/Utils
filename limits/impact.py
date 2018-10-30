from theta_auto import *


import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from operator import itemgetter, attrgetter
from copy import deepcopy

class result:
    def __init__(self, key, y_plus, y_minus ):
        self.key = key
        self.y_plus = y_plus
        self.y_minus= y_minus

def chi2_selector(chi2_values):
    best_chi2 = float('inf')
    fitnumber = 0
    for i, val in enumerate(chi2_values):
        if best_chi2 > val:
            fitnumber = i
            best_chi2 = val
    return fitnumber, best_chi2



def impact(model, output, options=None, factor = 1, signal_process=''):
    if not options:
	options = Options()
    if not signal_process:
        signal_process = model.signal_process_groups
	print signal_process

    nominal_result = None
    fit = mle(model, input='data',n=400, options=options,chi2=True)
    best_chi2 = float('inf')
    fitnumber = 0
    selected_signal = ""
    beta_signal = -1
    
    for key,p in fit.iteritems():
        #print key
        max_beta = max([x[0] for x in p['beta_signal']])
        if beta_signal < max_beta:
            tmp_fitnumber, tmp_best_chi2 = chi2_selector(p['__chi2'])
            best_fit_beta =  p['beta_signal'][tmp_fitnumber][0]
            if beta_signal < best_fit_beta : 
                beta_signal = best_fit_beta
                selected_signal = key
                fitnumber = tmp_fitnumber
                best_chi2 = tmp_best_chi2 
                nominal_result = p['beta_signal'][tmp_fitnumber]
                
    print 'sig  fit:',fitnumber , 'X2:', best_chi2, selected_signal, 'beta signal', beta_signal
    selected_signal_process = {}
    selected_signal_string  = None
    for k,param in signal_process.iteritems():
        if selected_signal in k:
            selected_signal_process[k] = param
            selected_signal_string     = param

    print selected_signal_process
    par_values = {}    
    for p in model.get_parameters(selected_signal_string):
        par_values[p] = fit[selected_signal][p][fitnumber][0]
    print par_values
    
    nuisance_constraint = get_fixed_dist_at_values(par_values)
    nuisance_constraint.distributions.pop('beta_signal',None) 
        
    result_plus = {}
    result_minus = {}
    for p in model.get_parameters(selected_signal_string):
        if 'beta_signal' in p: continue
        sigma   = fit[selected_signal][p][fitnumber][1]
        nominal = fit[selected_signal][p][fitnumber][0]
        #print p, nominal,'+-',sigma
        plus = nominal+sigma
        minus = nominal-sigma
        
        nuisance_constraint.distributions[p] = {'width': 0.0, 'range': [plus,plus], 'typ': 'gauss', 'mean':plus}
        impact_fit_plus = mle(model, input='data',n=100, options=options,nuisance_constraint=nuisance_constraint, signal_process_groups=selected_signal_process,chi2=True)
        l =  impact_fit_plus[selected_signal]['__chi2']
        min_chi2, idx = min((l[i],i) for i in xrange(len(l)))
        result_plus[p] = {'beta_signal':[impact_fit_plus[selected_signal]['beta_signal'][idx]],'paramter':plus} #beta_signal and error
        
        nuisance_constraint.distributions[p] = {'width': 0.0, 'range': [minus,minus], 'typ': 'gauss', 'mean':minus}
        impact_fit_minus = mle(model, input='data',n=100, options=options,nuisance_constraint=nuisance_constraint, signal_process_groups=selected_signal_process,chi2=True)
        #print impact_fit_minus
        l =  impact_fit_minus[selected_signal]['__chi2']
        min_chi2, idx = min((l[i],i) for i in xrange(len(l)))
        result_minus[p] = {'beta_signal':[impact_fit_minus[selected_signal]['beta_signal'][idx]],'paramter':minus} #beta_signal and error
        
        #set back to nominal values
        nuisance_constraint.distributions[p] = {'width': 0.0, 'range': [nominal,nominal], 'typ': 'gauss', 'mean':nominal}
       
    results = []
    
    for p in model.get_parameters(selected_signal_string):
        if 'beta_signal' in p:
            continue
        mean = nominal_result[0]
        #print 'nominal', nominal_result[0], 'up',result_plus[p]['beta_signal'], 'down',result_minus[p]['beta_signal']
        results.append(result(p, (mean-result_plus[p]['beta_signal'][0][0])/mean, (mean-result_minus[p]['beta_signal'][0][0])/mean))
        
    results = sorted(results, key=lambda result: (abs(result.y_minus)+abs(result.y_plus)))
    upper =[]
    lower =[]
    param_list=[]
    for i in results:
        param_list.append(i.key.replace("_"," ").replace("Chi2",""))
        upper.append(i.y_plus)
        lower.append(i.y_minus)

    
    #if 'Mu' not in output and 'Ele' not in output:
    #    exit(0)
    #plotting
    y_pos = np.arange(len(param_list))
    #x_val = [0]*len(param_list)
    
    pp = PdfPages(output)
    plt.clf()
    fig, ax = plt.subplots()
    plt.title(r'$\beta_{signal} '+str("{0:.1e}".format(nominal_result[0]*factor))+' \pm '+str("{0:.1e}".format(nominal_result[1]*factor))+'$',loc='right')
    #ax.set_ylim([0.001 ,1000])
    #ax.set_xlim([-.5,.5])
    ax.set_yticks(y_pos)
    ax.set_yticklabels(param_list)
    fig.subplots_adjust(left=0.2)
    ax.tick_params(labelsize='small')
    ax.barh(y_pos, upper, align='center', color='blue', ecolor='black',alpha=.5,label=r'pull at $+1\sigma$')
    ax.barh(y_pos, lower, align='center', color='green', ecolor='black',alpha=.5,label=r'pull at $-1\sigma$')
    plt.xlabel(r'$\Delta\beta$')
    #plt.ylabel('', fontsize=15)
    plt.legend(loc="lower left",prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')

    pp.close()
    
