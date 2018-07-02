execfile("ThetaPostFitPlot.py")
execfile("PostFitUncertainties.py")


import numpy
import matplotlib
#matplotlib.rcParams['text.usetex']=True
#matplotlib.rcParams['text.latex.unicode']=True
# -*- coding: utf-8 -*-
import cPickle as pickle
from subprocess import call
from matplotlib.backends.backend_pdf import PdfPages
import os

from theta_auto import *
from injection_merge import *
from impact import *

def run_cutopt(fname, Chirality, channel = "", particle = "BprimeB", write_report = True, injected_signal = "", beta_sig=0.0, glob_prefix=""):
    write_report = True        
    if write_report:config.report.reopen_file()
    #exit(0)
    factor = 1#.5 
    fname_rescaled = fname.replace(".root","_sig_scaled.root")
    call(['rm '+fname_rescaled],shell=True)
    call(['hadd '+fname_rescaled+' '+fname],shell=True)
    scale_hists(fname_rescaled,particle,factor)
    fname = fname_rescaled

    print 'signal',particle+'*'+Chirality+'*','for particle',particle
    print 'working on', fname


    print 'building model from root file',fname
    model = build_model_from_rootfile(fname,include_mc_uncertainties=True)
    model.fill_histogram_zerobins()
    model.set_signal_processes(particle+'*'+Chirality+'*')

    eletrigger = 1.01
    mutrigger  = 1.02
    for p in model.processes:
        if "Background" not in p:
            if particle in p:
                model.add_lognormal_uncertainty('lumi', math.log(1.025), p)
            process_name = p
            if 'Width' in p:
                process_name =process_name.replace('10p','')
                process_name =process_name.replace('20p','')
                process_name =process_name.replace('30p','')
	    mass = ''.join(x for x in process_name if x.isdigit())
	    wtag_unc = 3.9*math.log(float(mass)/400)            
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
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(mutrigger), p, obsname='Chi2_AntiBTagMu')    
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(mutrigger), p, obsname='Chi2_1_BTagMu')    
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(mutrigger), p, obsname='Chi2_2_BTagMu')    
               model.add_lognormal_uncertainty('mu_trigger_rate'   , math.log(mutrigger), p, obsname='TopTagMu')         
        
	                  
    #10% on the normalization
    uncer = 1.0
    ttag_unc = 1.1        
    if channel =="Mu" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagMu')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagMu')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagMu')    
        model.add_lognormal_uncertainty('W-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_WTagMu')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(ttag_unc), procname='Background',obsname='TopTagMu')    
    if channel =="Ele" or not channel:
        model.add_lognormal_uncertainty('Anti-b-tag_rate', math.log(uncer), procname='Background',obsname='Chi2_AntiBTagEle')    
        model.add_lognormal_uncertainty('1-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_1_BTagEle')    
        model.add_lognormal_uncertainty('2-b-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_2_BTagEle')    
        model.add_lognormal_uncertainty('W-tag_rate'   , math.log(uncer), procname='Background',obsname='Chi2_WTagEle')    
        model.add_lognormal_uncertainty('top-tag_rate'   , math.log(ttag_unc), procname='Background',obsname='TopTagEle')         
    
	#model.add_lognormal_uncertainty('electron_scalefactor',math.log(1.01),)
        
    model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    options.set('main', 'n_threads', '10')
   
    exp = None
    obs = None
    exp, obs = bayesian_limits(model, 'all', n_toy = 2000 ,options=options)
    
    output_directory =""
    if channel:
       output_directory = './'+glob_prefix+'output_'+particle+'_'+Chirality+'_'+channel+'/'
    else:
       output_directory = './'+glob_prefix+'output_'+particle+'_'+Chirality+'/'
    print 'Output directory',output_directory
    if output_directory  and not os.path.exists(output_directory):
       os.makedirs(output_directory)


    if write_report:
        config.report.write_html(output_directory)
        options.set('minimizer', 'minuit_tolerance_factor', '10000')
        options.set('main', 'n_threads', '1')
        mle_output = mle(model, input='data',n=200, options=options,chi2=True,with_covariance=True)
        #print mle_output
        postfit = ThetaPostFitPlot(mle_output)
        postfit.make_plots(output_directory)

        for key, value in mle_output.iteritems() :
            write_covariance_matrices(value,output_directory+"/"+key+"_covariance.root")

        
        limit_file = open(output_directory+"limit.txt",'w+')

        print >> limit_file, 'signal scale factor',factor
        limit_file.write("expected limit "+Chirality+":\n")
        print >> limit_file, exp
        limit_file.write("observed limit "+Chirality+":\n")
        print >> limit_file, obs
        """
        for item in fit_result:
            print >> limit_file, item
        """
        limit_file.close()
    
    #exit(0)
    options.set('main', 'n_threads', '1')
    #options.set('minimizer', 'minuit_tolerance_factor', '100')
    #model.scale_predictions(1/factor, procname=particle+'*', obsname='*')
    try:
        impact(model, output_directory+"/impact.pdf", options)
    except:
	print 'could not calculate the impact'			
        
    print "expected limit "+Chirality+' '+channel +":"
    print exp
    print "observed limit "+Chirality+' '+channel +":"
    print obs
     

    sigma2_color = '#ffcc00'# hopefully kOrange #'yellow' #"yellow"
    sigma1_color = '#00cc00'# hopefully kGreen+1 #'darkgreen' #"green"

    pp =None
    if channel:
        pp = PdfPages(output_directory+injected_signal+"limit_"+particle+"_"+Chirality+'_'+channel+".pdf")
    else:
        pp = PdfPages(output_directory+injected_signal+"limit_"+particle+"_"+Chirality+".pdf")


    theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
    #xanda
    theory13TeV_y =numpy.array([0.014,0.008,0.0046,0.0028,0.0017,0.001,0.00069,0.00046,0.0003,0.0002,0.00014,0.000099])
    theory13TeV_y_10p = numpy.array([0.159,0.084,0.048,0.028,0.017,0.011,0.006,0.0046,0.0031,0.0021,0.0014,0.001])
    # used in AN
    theory13TeV_y =numpy.array([0.745,0.532,0.388,0.285,0.212,0.159,0.12,0.0917,0.0706,0.0541,0.042,0.0324 ])
    

    
    legend_string =""
    if "LH" in Chirality:
        legend_string = "B+t, left handed chirality" #$c_L=1.0, BR(tW)=100\%$"
    elif "RH" in Chirality:
        legend_string = "B+t, right handed chirality" #$\mathbf{c_R=1.0}$, BR(tW)=100$\%$"

    if 'X' in particle:
        legend_string = legend_string.replace('B+t','$X_{53}$')
        #xanda
        #theory13TeV_y =numpy.array([0.108,0.056,0.031,0.018,0.011,0.007,0.0044,0.0029,0.0019,0.0013,0.0009,0.00062])
        #theory13TeV_y_10p = numpy.array([1.146,0.588,0.323,0.187,0.113,0.07,0.045,0.029,0.019,0.013,0.009,0.006])
        
    if 'BprimeB' in particle:
        theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
        theory13TeV_y =numpy.array([4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.249,0.195])
        theory13TeV_y =theory13TeV_y*0.5
        #xanda
        #theory13TeV_y =numpy.array([0.319,0.16,0.086,0.048,0.029,0.018,0.011,0.007,0.0047,0.003,0.002,0.0014])
        #theory13TeV_y_10p =numpy.array([3.16,1.563,0.833,0.47,0.276,0.168,0.105,0.067,0.044,0.0295,0.02,0.014])
        if "LH" in Chirality:
            legend_string = "B+b, left handed chirality" # c$_L$=1.0, BR(tW)=50$\%$"
        elif "RH" in Chirality:
            legend_string = "B+b, right handed chirality" #c$_R$=1.0, BR(tW)=50$\%$"

    #theory13TeV_y05 = theory13TeV_y*0.5*0.5
    #print 'vanilla',theory13TeV_y
    #print '0.5',theory13TeV_y05
    

    print 'legend string',legend_string,'for particle',particle
            
    upper = []
    for i in theory13TeV_x:
        upper.append(0.31*(i/1000)*(i/1000))


    #print exp.y
    #print 'before'
    #print exp.bands
    
    exp.y = [i * factor for i in exp.y]
    exp.bands = [ list(i) for i in exp.bands] #list(exp.bands)
    exp.bands[0][0] = [i * factor for i in exp.bands[0][0]]
    exp.bands[1][0] = [i * factor for i in exp.bands[1][0]]
    exp.bands[0][1] = [i * factor for i in exp.bands[0][1]]
    exp.bands[1][1] = [i * factor for i in exp.bands[1][1]]
    obs.y = [i * factor for i in obs.y]
    
        
    #cross section times branching ratio  limits
    plt.clf()
    plt.semilogy()
    ax=plt.gca()
    #ax.set_ylim([min(theory13TeV_y)*0.1 ,100])
    ax.set_ylim([0.001 ,1000])
    ax.set_xlim([700,1800])
    if 'X' in particle: ax.set_xlim([700,1600])
    #plt.rc('text', usetex=True)
    #plt.title("2.2 $fb^{-1}$ (13 TeV) ",horizontalalignment='center')
    #matplotlib.rcParams['text.usetex']=True
    #matplotlib.rcParams['text.latex.unicode']=True
    #f,ax = plt.subplots()
    plt.title("CMS                   35.9 fb$^{-1}$ (13 TeV)", fontsize=15)# , loc='right')
    #if "LH" in Chirality :plt.plot(theory13TeV_x, theory13TeV_y, label=legend_string,linestyle='--')
    plt.plot(theory13TeV_x, theory13TeV_y, label=legend_string,linestyle='--',color='blue')
    #Xandas cross sections
    #plt.plot(theory13TeV_x, theory13TeV_y, label=legend_string+' $1\%\ \Gamma /\mathrm{VLQ_{mass}}$',linestyle='--',color='blue')
    #plt.plot(theory13TeV_x, theory13TeV_y_10p, label=legend_string+' $10\%\ \Gamma /\mathrm{VLQ_{mass}}$',linestyle='--',color='red')    
    #plt.plot(theory13TeV_x, theory13TeV_y05, label=legend_string.replace('1.0','0.5'),linestyle='--',color='red')
    plt.plot(exp.x, exp.y, label="Exp $95\%$ CL" , color ='black',linestyle='dotted')#,color = exp_LH.bands[0][2])
    plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
                     alpha=0.6, facecolor=sigma2_color, edgecolor=sigma2_color,
                     linewidth=0, label="$\pm$ 2 std. deviation")
    plt.fill_between(exp.x, exp.bands[1][0] ,  exp.bands[1][1],
                     alpha=0.8, facecolor=sigma1_color, edgecolor=sigma1_color, # exp_LH.bands[1][2],
                     linewidth=0, label="$\pm$ 1 std. deviation")


    if obs: plt.plot(obs.x, obs.y, label='Obs $95\%$ CL', color ='black')
    
    plt.xlabel('VLQ mass (GeV)')
    plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)', fontsize=15)
    if 'X' in particle: plt.ylabel(r'$\mathbf{\sigma \times}$  BR(X$\mathbf{\rightarrow}$tW) (pb)', fontsize=15)
    plt.legend(loc="upper center",prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')

    pp.close()

    return exp,obs


def run_cat(fname, Chirality, particle = "BprimeB", write_report = True,channel=''):
    if write_report:config.report.reopen_file()
    #exit(0)
    factor = 0.1
    fname_rescaled = fname.replace(".root","_sig_scaled.root")
    call(['rm '+fname_rescaled],shell=True)
    call(['hadd '+fname_rescaled+' '+fname],shell=True)
    scale_hists(fname_rescaled,particle,factor)
    
    category = ''
    guess_category = fname.split('_')
    pretty_name =''
    if 'X' in particle:
        pretty_name += '$X_{5/3}$'
    if 'BprimeB' in particle:
        pretty_name += 'B+b'
    if 'BprimeT' in particle:
        pretty_name += 'B+t'
    
    if 'toptag' in guess_category[-1].lower():
       category = 'TopTag'
       pretty_name +=' t-tag'
    elif 'wtag' in guess_category[-1].lower():
       category = 'WTag'
       pretty_name +=' W-tag'
    elif 'btag' in guess_category[-1].lower():
       if 'anti' in guess_category[-1].lower():
          category = 'Chi2_AntiBTag'
          pretty_name += ' 0 tags'
       elif '2' in guess_category[-2].lower():
          category = 'Chi2_2_BTag'
          pretty_name += ' 2 b-tags'
       elif '1' in guess_category[-2].lower():
          category = 'Chi2_1_BTag'
          pretty_name += ' 1 b-tag'
    else: 
       print 'no category found. Terminating program'
       exit(-1)
    fname = fname_rescaled

    print 'signal',particle+'*'+Chirality+'*','for particle',particle
    print 'working on', fname


    print 'building model from root file',fname
    model = build_model_from_rootfile(fname,include_mc_uncertainties=True)
    model.fill_histogram_zerobins()
    model.set_signal_processes(particle+'*'+Chirality+'*')

    eletrigger = 1.01
    mutrigger  = 1.02
    for p in model.processes:
        if "Background" not in p:
            if particle in p:
                model.add_lognormal_uncertainty('lumi', math.log(1.025), p)
            process_name = p
            if 'Width' in p:
                process_name =process_name.replace('10p','')
                process_name =process_name.replace('20p','')
                process_name =process_name.replace('30p','')
	    mass = ''.join(x for x in process_name if x.isdigit())
	    wtag_unc = 3.9*math.log(float(mass)/400)            
            wtag_unc = math.sqrt(wtag_unc*wtag_unc+1)/100+1
            print p, 'Wtag_unc', wtag_unc
	    if 'wtag' in category.lower():
               model.add_lognormal_uncertainty('Wtag_unc', math.log(wtag_unc), p, obsname='WTag*')
	    if 'toptag' in category.lower():
               model.add_asymmetric_lognormal_uncertainty('toptag_unc', math.log(1.04), math.log(1.07), p, obsname='TopTag*')
            model.add_lognormal_uncertainty('ele_trigger_rate'   , math.log(eletrigger), p, obsname=category+'*')    
        
	                  
    #10% on the normalization
    uncer = 2 
            
    model.add_lognormal_uncertainty(category+'_rate', math.log(uncer), procname='Background',obsname=category+'*')    
    #model.add_lognormal_uncertainty('electron_scalefactor',math.log(1.01),)
        
    #model_summary(model)
    model_summary(model, create_plots=True, all_nominal_templates=True, shape_templates=True)
    options = Options()
    options.set('minimizer', 'strategy', 'robust')
    options.set('main', 'n_threads', '5')
    exp = None
    obs = None
    if 'anti' in category.lower(): 
        exp, obs = bayesian_limits(model, 'all', n_toy = 6000 ,options=options)
    else:
        exp, obs = bayesian_limits(model, 'all', n_toy = 1000 ,options=options)


    output_directory = './'+'output_'+channel+'_'+particle+'_'+Chirality+'_'+category+'/'
    print 'Output directory',output_directory
    if output_directory  and not os.path.exists(output_directory):
       os.makedirs(output_directory)


    if write_report:
        config.report.write_html(output_directory)
        options.set('minimizer', 'minuit_tolerance_factor', '1000')
        options.set('main', 'n_threads', '1')
        mle_output = mle(model, input='data',n=50, options=options,chi2=True,with_covariance=True)
        postfit = ThetaPostFitPlot(mle_output)
        postfit.make_plots(output_directory)

        
        limit_file = open(output_directory+"limit.txt",'w+')
        print >> limit_file, 'signal scale factor',factor
        limit_file.write("expected limit "+Chirality+":\n")
        print >> limit_file, exp
        limit_file.write("observed limit "+Chirality+":\n")
        print >> limit_file, obs
        """
        for item in fit_result:
            print >> limit_file, item
        """
        limit_file.close()
        

    print "expected limit "+category +":"
    print exp
    print "observed limit "+category +":"
    print obs


    exp.y = [i * factor for i in exp.y]
    exp.bands = [ list(i) for i in exp.bands] #list(exp.bands)
    exp.bands[0][0] = [i * factor for i in exp.bands[0][0]]
    exp.bands[1][0] = [i * factor for i in exp.bands[1][0]]
    exp.bands[0][1] = [i * factor for i in exp.bands[0][1]]
    exp.bands[1][1] = [i * factor for i in exp.bands[1][1]]
    obs.y = [i * factor for i in obs.y]

    

    sigma2_color = '#ffcc00'# hopefully kOrange #'yellow' #"yellow"
    sigma1_color = '#00cc00'# hopefully kGreen+1 #'darkgreen' #"green"

    pp = PdfPages(output_directory+"limit_"+particle+"_"+Chirality+".pdf")


    theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
    theory13TeV_y =numpy.array([0.0014,0.0008,0.00046,0.00028,0.00017,0.000,0.0917,0.0706,0.0541,0.042,0.0324 ])

    legend_string =""
    if "LH" in Chirality:
        legend_string = "B+t, left handed chirality" #$c_L=1.0, BR(tW)=100\%$"
    elif "RH" in Chirality:
        legend_string = "B+t, right handed chirality" #$\mathbf{c_R=1.0}$, BR(tW)=100$\%$"

    if 'X' in particle:
        legend_string = legend_string.replace('B+t','$X_{53}$')

    
        
    if 'BprimeB' in particle:
        theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
        theory13TeV_y =numpy.array([4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.249,0.195])
        theory13TeV_y =theory13TeV_y*0.5
        if "LH" in Chirality:
            legend_string = "B+b, left handed chirality" # c$_L$=1.0, BR(tW)=50$\%$"
        elif "RH" in Chirality:
            legend_string = "B+b, right handed chirality" #c$_R$=1.0, BR(tW)=50$\%$"

    theory13TeV_y05 = theory13TeV_y*0.5*0.5
    #print 'vanilla',theory13TeV_y
    #print '0.5',theory13TeV_y05
    

    print 'legend string',legend_string,'for particle',particle
            
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
    plt.title("CMS $\it{Preliminary}$       35.9 fb$^{-1}$ (13 TeV)", fontsize=10)# , loc='right')
    plt.plot(exp.x, exp.y, label="Exp $95\%$ CL" , color ='black',linestyle='dotted')#,color = exp_LH.bands[0][2])
    plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
                     alpha=0.6, facecolor=sigma2_color, edgecolor=sigma2_color,
                     linewidth=0, label="$\pm$ 2 std. deviation")
    plt.fill_between(exp.x, exp.bands[1][0] ,  exp.bands[1][1],
                     alpha=0.8, facecolor=sigma1_color, edgecolor=sigma1_color, # exp_LH.bands[1][2],
                     linewidth=0, label="$\pm$ 1 std. deviation")

    plt.plot([],[],' ',label=pretty_name)

    if obs: plt.plot(obs.x, obs.y, label='Obs $95\%$ CL', color ='black')
    
    plt.xlabel('VLQ mass (GeV)')
    plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
    if 'X' in particle: plt.ylabel(r'$\mathbf{\sigma \times}$  BR(X$\mathbf{\rightarrow}$tW) (pb)')
    plt.legend(loc="upper center",prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')

    pp.close()

    return exp,obs
