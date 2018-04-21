import ROOT
from copy import deepcopy
from numpy import linalg
import numpy as np


execfile("distribution.py")


def writeOutputFile(InputFileName, OutputFileName, InputDict, model, pdf = True):  

    InputFile = ROOT.TFile(InputFileName, "READ")
    print 'creating',OutputFileName
    OutputFile = ROOT.TFile(OutputFileName, "RECREATE")

  

    observables = model.get_observables()
    for o in observables:
        d_o = dict()

        #write data hist 
        h_data = ROOT.TH1F()
        InputFile.GetObject(o+'__DATA', h_data)
        h_data.Write()


        best_fit = 0
        if '__chi2' in InputDict:
            best_chi2 =9999999999999 
            for i, chi2_val in enumerate(InputDict['__chi2']):
                if chi2_val < best_chi2:
                    best_chi2 = chi2_val
                    best_fit = i

        for p in model.get_processes(o):

            #set the input histogram 
            h_in = ROOT.TH1F()
            InputFile.GetObject(o+'__'+p, h_in)
            
            distr = distribution()
            distr.set(h_in)  

            print o, h_in.Chi2Test(h_data)
            
            

            #apply shape morphing and uncertainties
            parameters = model.get_histogram_function(o, p).parameters
            for par in parameters:
                if not pdf and 'PDF' in par:
                    continue
                shape = InputDict[par][best_fit][0]
                shape_err = InputDict[par][best_fit][1]

                h_up = ROOT.TH1F()
                h_down = ROOT.TH1F()
                InputFile.GetObject(o+'__'+p+'__'+par+'__plus',h_up)
                InputFile.GetObject(o+'__'+p+'__'+par+'__minus',h_down)
                
                distr.addShapeVar(par, h_up, h_down, shape, shape_err)

            #apply rate scaling and uncertainties
            coeffs = model.get_coeff(o,p)
            for c in coeffs.factors:
                scn = InputDict[c][best_fit][0]
                scn_err = InputDict[c][best_fit][1]
                lambda_scale = coeffs.factors[c]['lambda_plus']
        
                scale = numpy.exp(scn*lambda_scale)
                scale_err = scale*lambda_scale*scn_err

                distr.scale(c,scale,scale_err) 
        
            #write to file
            distr.writeToFile(OutputFile)    
          

def write_covariance_matrices(parVals,output_name):
    if not output_name: output_name = 'mle_covcorr_noQ2.root'
    theta_res = parVals
    param_list = []
    for k, res in theta_res.iteritems():
        #print k,',',res
        if any(k == i for i in ['__nll','__cov','__chi2']): continue
        err_sq = res[0][1]*res[0][1]
        param_list.append((k, err_sq))
    
    for i in xrange(len(param_list)):
        helper = list(param_list[i])
        helper[0] = helper[0].replace('Anti-','0 ')
        helper[0] = helper[0].replace('Anti','0 ')
        helper[0] = helper[0].replace('Chi2_','')
        helper[0] = helper[0].replace('_',' ')
        helper[0] = helper[0].replace('WT','W-t')
        helper[0] = helper[0].replace('BT','b-t')
        helper[0] = helper[0].replace('top','t')
        helper[0] = helper[0].replace('TopT','t-t')

        param_list[i] = tuple(helper)    
        #    print proc, val
    

    cov_matrix = theta_res['__cov'][0]
    ind_dict = {}
    for i in xrange(cov_matrix.shape[0]):
        for ii in xrange(cov_matrix.shape[1]):
            entry = cov_matrix[i,ii]
            for proc, val in param_list:
                #print proc, abs(val-entry)
                if abs(val-entry) < 1e-6:
                    if i != ii:
                        self.message("WARNING row and column index don't match")
                    print 'found match for',proc
                    ind_dict[i] = proc
                if i not in ind_dict.keys():
                    ind_dict[i] = 'beta_signal'

    
    #exit(0)
    
    cov_matrix = np.matrix(cov_matrix)
    diag_matrix = np.matrix(np.sqrt(np.diag(np.diag(cov_matrix))))
    #try:
    inv_matrix = diag_matrix.I
    corr_matrix = inv_matrix * cov_matrix * inv_matrix

    corr_hist = ROOT.TH2D("correlation_matrix","",len(param_list),0,len(param_list),len(param_list),0,len(param_list))
    cov_hist = ROOT.TH2D("covariance_matrix","",len(param_list),0,len(param_list),len(param_list),0,len(param_list))
    
    for i in xrange(corr_matrix.shape[0]):
        if i not in ind_dict.keys(): continue
        corr_hist.GetXaxis().SetBinLabel(i+1, ind_dict.get(i,'unknown'))
        corr_hist.GetYaxis().SetBinLabel(i+1, ind_dict.get(i,'unknown'))
        cov_hist.GetXaxis().SetBinLabel(i+1, ind_dict.get(i,'unknown'))
        cov_hist.GetYaxis().SetBinLabel(i+1, ind_dict.get(i,'unknown'))
        corr_hist.SetLabelSize(0.03,'x')
        cov_hist.SetLabelSize(0.03,'x')
        corr_hist.GetZaxis().SetRangeUser(-1,1)
        for ii in xrange(corr_matrix.shape[1]):
            #if i<ii :continue
            entry_corr = corr_matrix[i,ii]
            entry_cov = cov_matrix[i,ii]
            corr_hist.Fill(i,ii,entry_corr)
            cov_hist.Fill(i,ii,entry_cov)

    matrices = ROOT.TFile(output_name,'RECREATE')
    
    cov_hist.Write()
    corr_hist.Write()
    matrices.Close()
