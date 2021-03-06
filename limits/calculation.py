execfile("ThetaPostFitPlot.py")
import numpy
import matplotlib
#matplotlib.rcParams['text.usetex']=True
#matplotlib.rcParams['text.latex.unicode']=True
import cPickle as pickle
from matplotlib.backends.backend_pdf import PdfPages


# -*- coding: utf-8 -*-
def run_cutopt(fname, Chirality, channel = "", particle = "b", write_report = True):
    if write_report:report.reopen_file()

    model = build_model_from_rootfile(fname)
    #model.scale_predictions(5.0 / 1.1)
    model.fill_histogram_zerobins()
    model.set_signal_processes('Bprime'+particle+'-*_'+Chirality+'*')

    for p in model.processes:
        model.add_lognormal_uncertainty('lumi', math.log(1.048), p)
        model.add_lognormal_uncertainty('Wtag_unc', math.log(1.05), p, obsname='WTag*')
        if not 'SingleT_s' in p:
            model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')
        
    model.add_lognormal_uncertainty('ZJets_rate', math.log(1.20), 'ZJets')
    model.add_lognormal_uncertainty('WJets_rate', math.log(1.20), 'WJets_Pt')
    model.add_lognormal_uncertainty('SingleTsChannel_rate', math.log(1.50), 'SingleT_s')
    model.add_lognormal_uncertainty('SingleTtChannel_rate', math.log(1.50), 'SingleT_t')
    model.add_lognormal_uncertainty('SingleTWAntitop_rate', math.log(1.50), 'SingleTWAntitop')
    model.add_lognormal_uncertainty('SingleTWTop_rate', math.log(1.50), 'SingleTWtop')
    model.add_lognormal_uncertainty('QCD_rate', math.log(2.0), 'QCD')
    model.add_lognormal_uncertainty('TTbar_rate', math.log(1.20), 'TTbar')
    
    #model_summary(model)
    model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
 

    #dist = model.distribution
    #model.distribution = get_fixed_dist(dist)

    #for r in runs: run_theta(options, in_background_thread = True)
    #for r in runs: r.wait_for_result_available()
    options = Options()
    #options.set('model','use_tbb','True')
    #options.set('model','tbb_nthreads','-1')
    options.set('minimizer', 'strategy', 'robust')
    #options.set('main','n_threads','10')
    exp, obs = asymptotic_cls_limits(model, use_data=False, signal_process_groups=None, beta_signal_expected=0.0, bootstrap_model=True, input=None, n=1, options=options)
    #exp, obs = bayesian_limits(model, 'all', n_toy = 20000, n_data = 2000,options=options)
         
    #evaluate_prediction(model)


    mle_input = 'toys:0.'
    signal_process_groups = {'':[]}
    fit = mle(model, input=mle_input, n = 10, signal_process_groups=signal_process_groups, with_error=False,  options=options)
    parameter_values = {}
    for p in model.get_parameters([]):
        parameter_values[p] = fit[''][p][0][0]
    histos = evaluate_prediction(model, parameter_values, include_signal = False)
    write_histograms_to_rootfile(histos, 'fit.root') 
    
    
    if write_report:
        output_directory =""
        if channel:
            output_directory = './output_Bprime'+particle+'_'+Chirality+'_'+channel+'/'
        else:
             output_directory = './output_Bprime'+particle+'_'+Chirality+'/'
            
        report.write_html(output_directory)
        savelimits = open(output_directory+"limits.pickle",'w+') 
        pickle.dump(exp,savelimits)   
        pickle.dump(obs,savelimits)
        savelimits.close()
   
        try:
            options.set('minimizer', 'always_mcmc', 'True')
            options.set('minimizer','minuit_tolerance_factor','10000')
            mle_output = mle(model, input=mle_input,n=10, options=options)
            mle_background = mle(model, input=mle_input,n=1, signal_process_groups ={"background_only":[]}, options=options)
            mle_output.update(mle_background)
            fit =  mle_background  #mle(model, input=mle_input, n = 10, signal_process_groups=signal_process_groups, with_error=False,  options=options)
            parameter_values = {}
            for p in model.get_parameters([]):
                parameter_values[p] = fit[''][p][0][0]
            histos = evaluate_prediction(model, parameter_values, include_signal = True)
            write_histograms_to_rootfile(histos, output_directory+'/fit.root') 
            postfit = ThetaPostFitPlot(mle_output)
            postfit.make_plots(output_directory)
        except:
            print 'Postfit MLE was not possible Bprime'+particle+"_"+Chirality
        limit_file = open(output_directory+"limit.txt",'w+')
        limit_file.write("expected limit "+Chirality+":\n")
        print >> limit_file, exp
        limit_file.write("observed limit "+Chirality+":\n")
        print >> limit_file, obs
        limit_file.close()

    print "expected limit "+Chirality+' '+channel +":"
    print exp
    print "observed limit "+Chirality+' '+channel +":"
    print obs

    


    #x = np.linspace(0, 2, 100)
    #plt.title("B+t -> tW 100%")
    #plt.ylim([0.4,10])


    sigma2_color = 'yellow' #"yellow"
    sigma1_color = 'darkgreen' #"green"

    pp = PdfPages("limit_"+"Bprime"+particle+"_"+Chirality+".pdf")
    if channel:
        pp = PdfPages("limit_"+"Bprime"+particle+"_"+Chirality+'_'+channel+".pdf")

    theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
    theory13TeV_y =[0.745,0.532,0.388,0.285,0.212,0.159,0.12,0.0917,0.0706,0.0541,0.042,0.0324 ]

    legend_string =""
    if "LH" in Chirality:
        legend_string = "Bt, $c_L=1.0, BR(tW)=100\%$"
    elif "RH" in Chirality:
        legend_string = "Bt, $\mathbf{c_R=1.0}$, BR(tW)=100$\%$"

    if particle is 'b':
        theory13TeV_x =[800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
        theory13TeV_y =numpy.array([3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.249,0.195])
        theory13TeV_y =theory13TeV_y*0.5
        if "LH" in Chirality:
            legend_string = "Bb, c$_L$=1.0, BR(tW)=50$\%$"
        elif "RH" in Chirality:
            legend_string = "Bb, c$_R$=1.0, BR(tW)=50$\%$"
            
    upper = []
    for i in theory13TeV_x:
        upper.append(0.31*(i/1000)*(i/1000))

    #cross section times branching ratio  limits
    plt.clf()
    plt.semilogy()
    #plt.rc('text', usetex=True)
    #plt.title("2.2 $fb^{-1}$ (13 TeV) ",horizontalalignment='center')
    #matplotlib.rcParams['text.usetex']=True
    #matplotlib.rcParams['text.latex.unicode']=True
    #f,ax = plt.subplots()
    axes = plt.gca()
    axes.set_xlim([700,1800])
    axes.set_ylim([0.01,5])
    plt.title("35.8 fb$^{-1}$ (13 TeV)", fontsize=10)# , loc='right')
    plt.plot(theory13TeV_x, theory13TeV_y, label=legend_string,linestyle='--')
    plt.plot(exp.x, exp.y, label="Exp $95\%$ CL" , color ='black',linestyle='dotted')#,color = exp_LH.bands[0][2])
    plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
                     alpha=0.6, facecolor=sigma2_color, edgecolor=sigma2_color,
                     linewidth=0, label="$\pm$ 2 std. deviation")
    plt.fill_between(exp.x, exp.bands[1][0] ,  exp.bands[1][1],
                     alpha=0.8, facecolor=sigma1_color, edgecolor=sigma1_color, # exp_LH.bands[1][2],
                     linewidth=0, label="$\pm$ 1 std. deviation")
    #workaround since normal mode does not work!
    plt.plot([],[],label="$\pm$ 1 std. deviation",color=sigma1_color,linewidth=10)#, edgecolor='black')
    plt.plot([],[],label="$\pm$ 2 std. deviation",color=sigma2_color,linewidth=10)#, edgecolor='black')
    #plt.plot(theory13TeV_x,upper,linestyle='-.',color='cyan')


    #if obs: plt.plot(obs.x, obs.y, label='Obs $95\%$ CL', color ='black')
    plt.xlabel('B quark Mass (GeV)')
    plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
    plt.legend(loc="upper center",prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')

    pp.close()

    return exp,obs

    
    # coupling limits
    exp_cl_y = []
    exp_cl_bands_sigma1up_y = []
    exp_cl_bands_sigma1down_y = []
    exp_cl_bands_sigma2up_y = []
    exp_cl_bands_sigma2down_y = []
    if obs:obs_cl_y = []
    
    for i in range(len(exp.x)):
        for m in range(len(theory13TeV_x)):
            if exp.x[i] == theory13TeV_x[m]:
                exp_cl_y.append(math.sqrt(exp.y[i]/theory13TeV_y[m]))
                exp_cl_bands_sigma1up_y.append(math.sqrt(exp.bands[0][0][i]/theory13TeV_y[m]))
                exp_cl_bands_sigma1down_y.append(math.sqrt(exp.bands[0][1][i]/theory13TeV_y[m]))
                exp_cl_bands_sigma2up_y.append(math.sqrt(exp.bands[1][0][i]/theory13TeV_y[m]))
                exp_cl_bands_sigma2down_y.append(math.sqrt(exp.bands[1][1][i]/theory13TeV_y[m]))
    """
    if obs:
        for i in range(len(obs.x)):
            for m in range(len(theory13TeV_x)):
                if obs.x[i] == theory13TeV_x[m]:
                    #print obs.x[i],theory13TeV_x[m],obs.y[i],theory13TeV_y[m],
                    obs_cl_y.append(math.sqrt(obs.y[i]/theory13TeV_y[m]))
    """
    plt.clf()
    plt.plot(exp.x, exp_cl_y, label= Chirality+' Chirality', color ='black',linestyle='--')#,color = exp_LH.bands[0][2])
    
    plt.fill_between(exp.x, exp_cl_bands_sigma1up_y , exp_cl_bands_sigma1down_y ,
                     alpha=0.6, facecolor='yellow', edgecolor='yellow',
                     linewidth=0)
    plt.fill_between(exp.x, exp_cl_bands_sigma2up_y , exp_cl_bands_sigma2down_y ,
                     alpha=0.8, facecolor = 'green', edgecolor='green', # exp_LH.bands[1][2],
                     linewidth=0)
    
    #if obs: plt.plot(obs.x, obs_cl_y, label='Obs', color ='black')
    plt.xlabel('Mass B [GeV]')
    plt.ylabel('|cL|')
    plt.legend(loc=2,prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')
    
    pp.close()

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

    plt.plot(theory13TeV_x, theory13TeV_y, label='Theory cross section 13TeV')
    plt.plot(obs_RH.x, obs_RH.y, label='Obs', color ='black')
    plt.xlabel('Mass B [GeV]')
    plt.ylabel('cross section times branching ratio [pb]')
    plt.legend(loc=2,prop={'size':7})
    plt.savefig("limit_RH.pdf")
    """
    
