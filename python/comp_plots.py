#!/usr/bin/python

import matplotlib
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt

def read_limit(name,leg=''):
    exp = []
    obs = []
    scale_factor = 1
    find_exp = False
    find_obs = True
    for line in open(name,'r'):
        if not line:continue
        if "scale factor" in line: scale_factor =line.split()[-1]
        if "exp" in line :
            find_exp = True
            find_obs = False
        if "obs" in line:
            find_exp = False
            find_obs = True
        splitline = line.split()
        if len(splitline) ==0  or not splitline[0].isdigit(): continue
        if find_exp:
            exp.append(line.split())
        elif find_obs:
            obs.append(line.split())
    return {'exp':exp,'obs':obs,'scale':scale_factor,'leg':leg}


def plot_dicts(dict_list,title,output):

    #reading the content
    y_axis =[]
    x_axis =[]
    legs   =[]
    #sigma1 =[]
    #sigma2 =[]
    for limit in dict_list:
        scaling= limit['scale']
        legs.append(limit['leg'])
        obs = limit['obs']
        exp = limit['exp']
        y = []
        x = []
        for i in exp:
            x.append(float(i[0]))
            y.append(float(i[1])*float(scaling))
        y_axis.append(y)
        x_axis.append(x)

    #plotting everything
    pp = PdfPages(output)
    plt.clf()
    plt.semilogy()
    ax=plt.gca()
    ax.set_ylim([.01,1000])
    ax.set_xlim([700,1800])
    plt.title(title, fontsize=10)# , loc='right')
    for i in xrange(len(y_axis)):
        plt.plot(x_axis[i], y_axis[i], label=legs[i]) #, linestyle='dotted')#,color = exp_LH.bands[0][2])


    plt.xlabel('VLQ mass (GeV)')
    plt.ylabel(r'$\mathbf{\sigma \times}$  BR(B$\mathbf{\rightarrow}$tW) (pb)')
    plt.legend(loc="upper center",prop={'size':12},frameon=False)
    plt.savefig(pp, format='pdf')
    pp.close()

    
if __name__ == '__main__':
    plot_dicts([read_limit("B_t_RH_0_btag.txt", "0 b-tag cat."),read_limit("B_t_RH_1_btag.txt", "1 b-tag cat."),read_limit("B_t_RH_2_btag.txt", "2 b-tag cat."),read_limit("B_t_RH_t_tag.txt", "t-tag cat."),read_limit("B_t_RH_W_tag.txt", "W-tag cat."),read_limit("B_t_RH.txt", "combination")],"category comparison","cat_comp.pdf")
    plot_dicts([read_limit("B_t_RH.txt", "vanilla"),read_limit("eta4_B_t_RH.txt", "$|\eta|<4$")],"$|\eta|<4$ comparison","eta4_comp.pdf")
