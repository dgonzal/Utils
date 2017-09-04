import matplotlib
from matplotlib.backends.backend_pdf import PdfPages


def plot_limits(particle,Chirality,channel,exp,obs):
    sigma2_color = 'yellow' #"yellow"
    sigma1_color = 'darkgreen' #"green"

    pp = PdfPages("limit_"+"Bprime"+particle+"_"+Chirality+".pdf")
    if channel:
        pp = PdfPages("limit_"+"Bprime"+particle+"_"+Chirality+'_'+channel+".pdf")

    theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
    theory13TeV_y =[ 0.745,0.532,0.388,0.285,0.212,0.159,0.120,0.0917,0.0706,0.0541,0.042,0.0324]


    legend_string =""
    if "LH" in Chirality:
        legend_string = "Bt, $c_L=1.0, BR(tW)=100\%$"
    elif "RH" in Chirality:
        legend_string = "Bt, $\mathbf{c_R=1.0}$, BR(tW)=100$\%$"

    if particle is 'B':
        theory13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
        theory13TeV_y =numpy.array([4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.249,0.195])
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

    #pp.close()
    #return
    
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
