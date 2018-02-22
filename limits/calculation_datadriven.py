execfile("ThetaPostFitPlot.py")
import numpy
import matplotlib
#matplotlib.rcParams['text.usetex']=True
#matplotlib.rcParams['text.latex.unicode']=True
# -*- coding: utf-8 -*-
import cPickle as pickle
from matplotlib.backends.backend_pdf import PdfPages
import os

def run_cutopt(fname, Chirality, channel = "", particle = "BprimeB", write_report = True, injected_signal = "", beta_sig=0.0, glob_prefix=""):
    #write_report = True        
    if write_report:report.reopen_file()
    print 'signal',particle+'*'+Chirality+'*','for particle',particle
    print 'working on', fname

    model = build_model_from_rootfile(fname)
    model.fill_histogram_zerobins()
    model.set_signal_processes(particle+'*'+Chirality+'*')

    eletrigger = 1.01
    mutrigger  = 1.02
    for p in model.processes:
        if "Background" not in p:
            model.add_lognormal_uncertainty('lumi', math.log(1.025), p)
            process_name = p
            if 'Width' in p:
                process_name =process_name.replace('10p','')
                process_name =process_name.replace('20p','')
                process_name =process_name.replace('30p','')
	    mass = ''.join(x for x in process_name if x.isdigit())
	    wtag_unc = 3.9*math.log(float(mass)/2/200)            
            wtag_unc = math.sqrt(wtag_unc*wtag_unc+1)/100+1
            print p, 'Wtag_unc', wtag_unc
            model.add_lognormal_uncertainty('Wtag_unc', math.log(wtag_unc), p, obsname='WTag*')
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
        
	                  

    #10% on the normalization
    uncer = 1.2
            
    if channel =="Mu" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagMu')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagMu')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagMu')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(uncer), procname='Background',obsname='TopTagMu')    
    if channel =="Ele" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagEle')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagEle')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagEle')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(uncer), procname='Background',obsname='TopTagEle')         
    
	#model.add_lognormal_uncertainty('electron_scalefactor',math.log(1.01),)
        
    #model_summary(model)
    model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    options.set('main', 'n_threads', '5')
    exp = None
    obs = None

    #exp, obs = asymptotic_cls_limits(model, use_data=False, signal_process_groups=None, beta_signal_expected=beta_sig, bootstrap_model=True, input=None, n=1, options=options)    
     
    #if not injected_signal: 
    exp, obs = bayesian_limits(model, 'expected', n_toy = 2000 ,options=options)


    output_directory =""
    if channel:
       output_directory = './'+glob_prefix+'output_'+particle+'_'+Chirality+'_'+channel+'/'
    else:
       output_directory = './'+glob_prefix+'output_'+particle+'_'+Chirality+'/'
    print 'Output directory',output_directory
    if output_directory  and not os.path.exists(output_directory):
       os.makedirs(output_directory)

    if write_report:

        report.write_html(output_directory)
        #savelimits = open(output_directory+"/"+injected_signal+"limits.pickle",'w+') 
        #pickle.dump(exp,savelimits)   
        #pickle.dump(obs,savelimits)
        #savelimits.close()
        
        fit_result = []
        mle_output = None
        """
        try:
            #options.set('minimizer', 'always_mcmc', 'True')
            #options.set('global','debug','True')
            #options.set('minimizer','minuit_tolerance_factor','1000')
            mle_output = mle(model, input='toys:0',n=5, options=options)
            #mle_output = mle(model, input='data',n=1000, options=options,signal_process_groups ={"background_only":[]} )
            postfit = ThetaPostFitPlot(mle_output)
            postfit.make_plots(output_directory)

        except Exception as e:
            print 'Postfit MLE was not possible Bp'+particle+'_TW_'+Chirality+":"
            print e

        try:
            #options.set('minimizer','minuit_tolerance_factor','0.001')
            mle_background = mle(model, input='toys:0',n=5,signal_process_groups ={"background_only":[]} )
            for pf_vals in mle_background.itervalues():
                del pf_vals['__nll']
                for key in pf_vals:
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
                    pf_vals[key] = ((3**wei_mean, 3**mean_err),)
                    #print wei_mean, mean_err
                    print key,'Mean',math.exp(wei_mean),"Error",math.exp(mean_err-1)
                    fit_result.append(str(key)+' Mean '+str(math.exp(wei_mean))+" Error "+str(math.exp(mean_err)))
                    #print vals,'Error',wei_mean,"Mean",mean_err, 'no exp' 
            postfit = ThetaPostFitPlot(mle_background)
            postfit.make_plots(output_directory)
        except Exception as e:
            print 'Postfit MLE was not possible for background. Channel '+particle+'_'+Chirality+":"
            print e
        """  
        limit_file = open(output_directory+"limit.txt",'w+')
        limit_file.write("expected limit "+Chirality+":\n")
        print >> limit_file, exp
        limit_file.write("observed limit "+Chirality+":\n")
        print >> limit_file, obs
        """
        for item in fit_result:
            print >> limit_file, item
        """
        limit_file.close()
        

    print "expected limit "+Chirality+' '+channel +":"
    print exp
    print "observed limit "+Chirality+' '+channel +":"
    print obs

    #return exp,obs

    #x = np.linspace(0, 2, 100)
    #plt.title("B+t -> tW 100%")
    #plt.ylim([0.4,10])
     

    sigma2_color = '#ffcc00'# hopefully kOrange #'yellow' #"yellow"
    sigma1_color = '#00cc00'# hopefully kGreen+1 #'darkgreen' #"green"

    pp =None
    if channel:
        pp = PdfPages(output_directory+injected_signal+"limit_"+particle+"_"+Chirality+'_'+channel+".pdf")
    else:
        pp = PdfPages(output_directory+injected_signal+"limit_"+particle+"_"+Chirality+".pdf")


    theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
    theory13TeV_y =numpy.array([0.745,0.532,0.388,0.285,0.212,0.159,0.12,0.0917,0.0706,0.0541,0.042,0.0324 ])

    legend_string =""
    if "LH" in Chirality:
        legend_string = "Bt, $c_L=1.0, BR(tW)=100\%$"
    elif "RH" in Chirality:
        legend_string = "Bt, $\mathbf{c_R=1.0}$, BR(tW)=100$\%$"

    if 'X' in particle:
        legend_string = legend_string.replace('Bt','$X_{53}$')

    
        
    if 'BprimeB' in particle:
        theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
        theory13TeV_y =numpy.array([4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.249,0.195])
        theory13TeV_y =theory13TeV_y*0.5
        if "LH" in Chirality:
            legend_string = "Bb, c$_L$=1.0, BR(tW)=50$\%$"
        elif "RH" in Chirality:
            legend_string = "Bb, c$_R$=1.0, BR(tW)=50$\%$"

    theory13TeV_y05 = theory13TeV_y*0.5*0.5
    #print 'vanilla',theory13TeV_y
    #print '0.5',theory13TeV_y05
    

    print 'legen string',legend_string,'for particle',particle
            
    upper = []
    for i in theory13TeV_x:
        upper.append(0.31*(i/1000)*(i/1000))

    #cross section times branching ratio  limits
    plt.clf()
    plt.semilogy()
    ax=plt.gca()
    ax.set_ylim([.01,1000])
    ax.set_xlim([700,1800])
    if 'X' in particle: ax.set_xlim([700,1600])
    #plt.rc('text', usetex=True)
    #plt.title("2.2 $fb^{-1}$ (13 TeV) ",horizontalalignment='center')
    #matplotlib.rcParams['text.usetex']=True
    #matplotlib.rcParams['text.latex.unicode']=True
    #f,ax = plt.subplots()
    plt.title("CMS $\it{Preliminary}$       35.9 fb$^{-1}$ (13 TeV)", fontsize=10)# , loc='right')
    #if "LH" in Chirality :plt.plot(theory13TeV_x, theory13TeV_y, label=legend_string,linestyle='--')
    plt.plot(theory13TeV_x, theory13TeV_y, label=legend_string,linestyle='--',color='blue')
    plt.plot(theory13TeV_x, theory13TeV_y05, label=legend_string.replace('1.0','0.5'),linestyle='--',color='red')
    plt.plot(exp.x, exp.y, label="Exp $95\%$ CL" , color ='black',linestyle='dotted')#,color = exp_LH.bands[0][2])
    plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
                     alpha=0.6, facecolor=sigma2_color, edgecolor=sigma2_color,
                     linewidth=0, label="$\pm$ 2 std. deviation")
    plt.fill_between(exp.x, exp.bands[1][0] ,  exp.bands[1][1],
                     alpha=0.8, facecolor=sigma1_color, edgecolor=sigma1_color, # exp_LH.bands[1][2],
                     linewidth=0, label="$\pm$ 1 std. deviation")
    #workaround since normal mode does not work!
    #plt.plot([],[],label="$\pm$ 1 std. deviation",color=sigma1_color,linewidth=10)#, edgecolor='black')
    #plt.plot([],[],label="$\pm$ 2 std. deviation",color=sigma2_color,linewidth=10)#, edgecolor='black')
    #plt.plot(theory13TeV_x,upper,linestyle='-.',color='cyan')


    if obs: plt.plot(obs.x, obs.y, label='Obs $95\%$ CL', color ='black')
    
    plt.xlabel('VLQ mass (GeV)')
    plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
    if 'X' in particle: plt.ylabel(r'$\mathbf{\sigma \times}$  BR(X$\mathbf{\rightarrow}$tW) (pb)')
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
    if obs:
        for i in range(len(obs.x)):
            for m in range(len(theory13TeV_x)):
                if obs.x[i] == theory13TeV_x[m]:
                    #print obs.x[i],theory13TeV_x[m],obs.y[i],theory13TeV_y[m],
                    obs_cl_y.append(math.sqrt(obs.y[i]/theory13TeV_y[m]))

    plt.clf()
    plt.plot(exp.x, exp_cl_y, label= Chirality+' Chirality', color ='black',linestyle='--')#,color = exp_LH.bands[0][2])
    
    plt.fill_between(exp.x, exp_cl_bands_sigma1up_y , exp_cl_bands_sigma1down_y ,
                     alpha=0.6, facecolor='yellow', edgecolor='yellow',
                     linewidth=0)
    plt.fill_between(exp.x, exp_cl_bands_sigma2up_y , exp_cl_bands_sigma2down_y ,
                     alpha=0.8, facecolor = 'green', edgecolor='green', # exp_LH.bands[1][2],
                     linewidth=0)
    
    if obs: plt.plot(obs.x, obs_cl_y, label='Obs', color ='black')
    plt.xlabel('Mass B [GeV]')
    plt.ylabel('|cL|')
    plt.legend(loc=2,prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')
    
    pp.close()

    return exp,obs
 
    
def background_signal_discovery(fname, Chirality, channel = "", particle = "BprimeB", write_report = True, injected_signal = "", beta_sig=0.0, glob_prefix=""):
    if write_report:report.reopen_file()
    print 'signal',particle+'*'+Chirality+'*','for particle',particle
    print 'working on', fname

    model = build_model_from_rootfile(fname)
    model.fill_histogram_zerobins()
    model.set_signal_processes(particle+'*'+Chirality+'*')

    eletrigger = 1.01
    mutrigger  = 1.02
    for p in model.processes:
        if "Background" not in p and not 'Signal' in p:
            model.add_lognormal_uncertainty('lumi', math.log(1.025), p)
            process_name = p
            if 'Width' in p:
                process_name =process_name.replace('10p','')
                process_name =process_name.replace('20p','')
                process_name =process_name.replace('30p','')
	    mass = ''.join(x for x in process_name if x.isdigit())
	    wtag_unc = 3.9*math.log(float(mass)/2/200)
            wtag_unc = math.sqrt(wtag_unc*wtag_unc+1)/100+1
            print p, 'Wtag_unc', wtag_unc
            model.add_lognormal_uncertainty('Wtag_unc', math.log(wtag_unc), p, obsname='WTag*')
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
        elif 'Signal' in p:
            model.add_lognormal_uncertainty('beta_signal', math.log(2.), procname='Signal',obsname='*')
    #model.scale_predictions(0, procname='Signal', obsname='*')              

    #10% on the normalization
    uncer = 5.
            
    if channel =="Mu" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagMu')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagMu')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagMu')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(uncer), procname='Background',obsname='TopTagMu')    
    if channel =="Ele" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagEle')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagEle')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagEle')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(uncer), procname='Background',obsname='TopTagEle')         
 
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    #options.set('main', 'n_threads', '5')

    theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000]
    theory13TeV_y =numpy.array([0.745,0.532,0.388,0.285,0.212,0.159,0.12,0.0917,0.0706,0.0541,0.042,0.0324,0.0252,0.0198])

    legend_string =""
    if "LH" in Chirality:
        legend_string = "Bt, $c_L=1.0, BR(tW)=100\%$"
    elif "RH" in Chirality:
        legend_string = "Bt, $\mathbf{c_R=1.0}$, BR(tW)=100$\%$"

    if 'X' in particle:
        legend_string = legend_string.replace('Bt','$X_{53}$')

    
        
    if 'BprimeB' in particle:
        theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000]
        theory13TeV_y =numpy.array([4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.249,0.195, 0.157, 0.120])
        theory13TeV_y =theory13TeV_y*0.5
        if "LH" in Chirality:
            legend_string = "Bb, c$_L$=1.0, BR(tW)=50$\%$"
        elif "RH" in Chirality:
            legend_string = "Bb, c$_R$=1.0, BR(tW)=50$\%$"

    theory13TeV_y05 = theory13TeV_y*0.5*0.5
    #print 'vanilla',theory13TeV_y
    #print '0.5',theory13TeV_y05

    cross_dict = dict(zip(theory13TeV_x, theory13TeV_y))
    
    print cross_dict[1100]
    
    discovery_val = []
    for i, signal in enumerate(model.signal_processes):
        mass = signal
        mass = mass.replace('10p','').replace('20p','').replace('30p','')
        mass = ''.join(x for x in mass if x.isdigit())
        discovery_val.append(discovery(model, use_data=True, spid=signal , Z_error_max=0.2, maxit=1, n=1000, n_expected=400, input_expected='toys:'+str(cross_dict[int(mass)]), options=options, ts_method=derll))
        
    for i, signal in enumerate(model.signal_processes):
        print signal, discovery_val[i]
    #print discovery_val
