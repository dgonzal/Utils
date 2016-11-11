execfile("ThetaPostFitPlot.py")
import numpy
import matplotlib
#matplotlib.rcParams['text.usetex']=True
#matplotlib.rcParams['text.latex.unicode']=True
import cPickle as pickle
from matplotlib.backends.backend_pdf import PdfPages
import os

# -*- coding: utf-8 -*-
def run_cutopt(fname, Chirality, channel = "", particle = "b", write_report = True, injected_signal = "", beta_sig=0.0, run_data=False):
    if write_report:report.reopen_file()

    model = build_model_from_rootfile(fname)
    #model.scale_predictions(5.0 / 1.1)
    model.fill_histogram_zerobins()
    model.set_signal_processes('Bp'+particle+'_TW_*_'+Chirality+'*')

    #for p in model.processes:
    #    model.add_lognormal_uncertainty('lumi', math.log(1.048), p)

    """
    if channel =="Mu" or "":
        model.add_lognormal_uncertainty('Mu_Anti-b-tag_rate', math.log(3), procname='Background',obsname='Chi2_AntiBTagMu')    
        model.add_lognormal_uncertainty('Mu_1-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_1_BTagMu')    
        model.add_lognormal_uncertainty('Mu_2-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_2_BTagMu')    
        model.add_lognormal_uncertainty('Mu_top-tag_rate'   , math.log(3), procname='Background',obsname='TopTagMu')    
    if channel =="Ele" or "":
        model.add_lognormal_uncertainty('Ele_Anti-b-tag_rate', math.log(3), procname='Background',obsname='Chi2_AntiBTagEle')    
        model.add_lognormal_uncertainty('Ele_1-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_1_BTagEle')    
        model.add_lognormal_uncertainty('Ele_2-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_2_BTagEle')    
        model.add_lognormal_uncertainty('Ele_top-tag_rate'   , math.log(3), procname='Background',obsname='TopTagEle')    
    """
    if channel =="Mu" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(3), procname='Background',obsname='Chi2_AntiBTagMu')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_1_BTagMu')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_2_BTagMu')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(3), procname='Background',obsname='TopTagMu')    
    if channel =="Ele" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(3), procname='Background',obsname='Chi2_AntiBTagEle')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_1_BTagEle')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(3), procname='Background',obsname='Chi2_2_BTagEle')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(3), procname='Background',obsname='TopTagEle')         



    #model_summary(model)
    model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    #exp, obs = asymptotic_cls_limits(model, use_data=True, signal_process_groups=None, beta_signal_expected=beta_sig, bootstrap_model=True, input=None, n=1, options=options)    
    exp = None
    obs = None

    if not injected_signal: exp, obs  = bayesian_limits(model, 'all', n_toy = 1000, n_data = 100,options=options)
    else: exp,obs = bayesian_limits(model, 'expected', n_toy = 3000, n_data = 1000,options=options)

    #evaluate_prediction(model)
    if write_report:
        output_directory =""
        if channel:
            output_directory = './output_Bprime'+particle+'_'+Chirality+'_'+channel+'/'
        else:
             output_directory = './output_Bprime'+particle+'_'+Chirality+'/'
            

        if injected_signal: 
            output_directory = './Injected/'+injected_signal+output_directory[1:-1]+"/"
            if not os.path.exists(output_directory):
                os.makedirs(output_directory)

        report.write_html(output_directory)
        savelimits = open(output_directory+"/"+injected_signal+"limits.pickle",'w+') 
        #pickle.dump(exp,savelimits)   
        #pickle.dump(obs,savelimits)
        savelimits.close()
   
        fit_result = []

        try:
        #options.set('minimizer', 'always_mcmc', 'True')
        #options.set('global','debug','True')
            #options.set('minimizer','minuit_tolerance_factor','1000')
            mle_output = mle(model, input='data',n=1000, options=options)
            #mle_output = mle(model, input='data',n=1000, options=options,signal_process_groups ={"background_only":[]} )
            try:
                #options.set('minimizer','minuit_tolerance_factor','0.001')
                mle_background = mle(model, input='data',n=1000,signal_process_groups ={"background_only":[]} )
                mle_output.update(mle_background)
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
                        print key,'Mean',3**wei_mean,"Error",3**mean_err-1
                        fit_result.append(str(key)+' Mean '+str(math.exp(wei_mean))+" Error "+str(math.exp(mean_err)))
                    #print vals,'Error',wei_mean,"Mean",mean_err, 'no exp' 
            except:
                print 'Postfit MLE was not possible for background. Channel Bp'+particle+'_TW_'+Chirality
            postfit = ThetaPostFitPlot(mle_output)
            postfit.make_plots(output_directory)
        except:
            print 'Postfit MLE was not possible Bp'+particle+'_TW_'+Chirality
        limit_file = open(output_directory+"limit.txt",'w+')
        limit_file.write("expected limit "+Chirality+":\n")
        print >> limit_file, exp
        limit_file.write("observed limit "+Chirality+":\n")
        print >> limit_file, obs
        
        for item in fit_result:
            print >> limit_file, item
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

    pp =None
    if channel:
        pp = PdfPages(output_directory+injected_signal+"limit_"+"Bprime"+particle+"_"+Chirality+'_'+channel+".pdf")
    else:
        pp = PdfPages(output_directory+injected_signal+"limit_"+"Bprime"+particle+"_"+Chirality+".pdf")


    theory13TeV_x =[800,900,1000,1100,1200,1300,1400,1500]
    theory13TeV_y =[0.365,0.271,0.203,0.152,0.116,0.0894,0.0692,0.0540]


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
    plt.title("2.2 fb$^{-1}$ (13 TeV)", fontsize=10)# , loc='right')
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


    if obs: plt.plot(obs.x, obs.y, label='Obs $95\%$ CL', color ='black')
    plt.xlabel('B quark Mass (GeV)')
    plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
    plt.legend(loc="upper center",prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')

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
    
