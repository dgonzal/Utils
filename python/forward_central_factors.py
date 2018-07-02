import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

from ROOT import TFile, TH1, TObject, TF1, TLegend,TH1F

import sys
sys.path.insert(0, '../limits/')

from injection_merge import *
from signal_background_uncertainty import *
TH1.AddDirectory(ROOT.kFALSE);

import scipy.stats as stats


def scale_factors(forward, central, output):
    TH1.AddDirectory(ROOT.kFALSE);
    #rootfile = TFile(ROOTFile,"READ")
    #forward = rootfile.Get(forward_name).Clone()
    #central = rootfile.Get(central_name).Clone()

    #print "loaded", forward_name, "and", central_name
    
    central_norm = central.GetSumOfWeights()
    forward_norm = forward.GetSumOfWeights()

   
    #for i in xrange(1,forward.GetNcells()-1):
    #    forward.SetBinError(i,forward.GetBinError(i)/forward.GetBinContent(i))
    #    central.SetBinError(i,central.GetBinError(i)/central.GetBinContent(i))

    
    #central.Scale(1/central_norm)
    #forward.Scale(1/forward_norm)
    
    #for i in xrange(1,forward.GetNcells()-1):
    #    print 'bin',i,'content',central.GetBinContent(i),'\pm',central.GetBinError(i)
    #central = central.Rebin(4)
    #forward = forward.Rebin(4)
    can = ROOT.TCanvas()
    
    #fit_scale = get_scale(central, forward, central_norm/forward_norm,700)
    #forward.Scale(fit_scale)
    

    correction_hist = forward.Clone('corrections')
    for mc_bin in xrange(1,forward.GetNcells()-1):
        a = forward.GetBinContent(mc_bin)
        b = central.GetBinContent(mc_bin)
	if b ==0. or a==0.:
            correction_hist.SetBinContent(mc_bin,-1)
            correction_hist.SetBinError(mc_bin,0)
            continue
        b_err = central.GetBinError(mc_bin)
        a_err = forward.GetBinError(mc_bin)
        factor = a/b
        error = ((1/b*a_err)**2 + (a/(b*b) * b_err)**2)
        # print 'b',b,'+-',b_err
        # print 'a',a,'+-',a_err
        # print 'bin',mc_bin,'factor',factor,'\pm',math.sqrt(error)
        
        correction_hist.SetBinContent(mc_bin,factor)
        correction_hist.SetBinError(mc_bin,math.sqrt(error))
        
    correction_hist.Scale((correction_hist.GetNcells()-2)/correction_hist.GetSumOfWeights())

        
    correction_hist.SetAxisRange(0.5, 4,"Y");
    correction_hist.Draw();
    #can.SaveAs(output)
    return correction_hist

def rebin(histograms,unc):
    for hist in histograms:
        new_binning = update(hist, unc)
        if len(new_binning)<4: continue
        for i in xrange(len(histograms)):
            histograms[i] = histograms[i].Rebin(len(new_binning)-1,hist.GetName(), array.array('d',new_binning))	
    return histograms


def chi2_ratio(MC,data):
    if len(MC) != len(data):
        print 'number of mc and data histograms is not equal'
        print 'mc',len(MC),'data',len(data)
    chi2 = 0
    ndof = 0
    for m in xrange(len(MC)):
        mc_hist = MC[m]
        data_hist = data[m]
        #ndof -=1
        for i in xrange(1,mc_hist.GetNcells()-1):
            chi2 +=(mc_hist.GetBinContent(i)-data_hist.GetBinContent(i))**2/(mc_hist.GetBinError(i)**2+data_hist.GetBinError(i)**2)
            ndof +=1
    print chi2,ndof-1, 1-stats.chi2.cdf(chi2,ndof-1)

    
if __name__ == "__main__":
    
    categories = ["Anti_btag","1_btag","2_btags","ttag","Wtag"]
    files = ["../electron_hists.root","../muon_hists.root"]
    scale_hist_mc = []
    scale_hist_data =[]
    for input_file in files:
        channel = ''
        legchannel = ''
        if 'muon' in input_file:
            channel = 'mu_'
            legchannel = '#mu channel ' 
        if 'ele' in input_file:
            channel = 'ele_'
            legchannel = 'e channel '
        for i in categories:
            rootfile = TFile(input_file,"READ")
            histogramlist = rebin([rootfile.Get("Data__"+i+"_forward").Clone(), rootfile.Get("Data__"+i+"_central").Clone(),rootfile.Get("MC_merge__"+i+"_forward").Clone(), rootfile.Get("MC_merge__"+i+"_central").Clone()], .15)
            
            data_hist = scale_factors(histogramlist[0], histogramlist[1], "data_"+i+".pdf")
            mc_hist = scale_factors(histogramlist[2], histogramlist[3], "MC_"+i+".pdf")
            print 'raw histogram compability'
            print 'single hists'
            chi2_ratio([histogramlist[0]],[histogramlist[2]])
            chi2_ratio([histogramlist[1]],[histogramlist[3]])
            print 'combined'
            chi2_ratio(histogramlist[0:1],histogramlist[2:3])
            print '-'*10
            scale_hist_mc.append(mc_hist)
            scale_hist_data.append(data_hist)
            
	    data_hist.Chi2Test(mc_hist,'WWP')
            can = ROOT.TCanvas()

            leg_comp = TLegend(0.3, 0.6, 0.5, 0.85)
            leg_comp.AddEntry(data_hist,"Data","le")
            leg_comp.AddEntry(mc_hist,"MC","le")
            leg_comp.SetHeader(legchannel+(i.replace('_',' ').replace('Anti','0').replace('tag',' tag'))+' ratio')
            leg_comp.SetFillColor(0);
            leg_comp.SetLineColor(1);
            leg_comp.SetBorderSize(0);
            leg_comp.SetTextFont(42);
            leg_comp.SetTextSize(.05);
            leg_comp.SetFillStyle(0);

            data_hist.GetXaxis().SetTitle('m_{reco} [GeV]') 
            data_hist.SetLineColor(1)
            data_hist.Draw()
            mc_hist.SetLineColor(2)
            mc_hist.Draw("same")
            leg_comp.Draw()
            can.SaveAs(channel+i+".pdf")

    print 'combined ratio prob'
    chi2_ratio(scale_hist_mc, scale_hist_data)
    print '+'*10
    """
            continue

        
            rootfile = TFile(input_file,"READ")
            data_forward = rootfile.Get("Data__"+i+"_forward").Clone()
            data_central = rootfile.Get("Data__"+i+"_central").Clone()
        
            MC_forward = rootfile.Get("MC_merge__"+i+"_forward").Clone()
            MC_central = rootfile.Get("MC_merge__"+i+"_central").Clone()

            data_forward.SetLineColor(1)
            data_central.SetLineColor(3)
            MC_forward.SetLineColor(2)
            MC_central.SetLineColor(4)

            data_forward.DrawNormalized("e")
            MC_forward.DrawNormalized("same")
            can.SaveAs(channel+i+"_forward.pdf")
        
            data_central.DrawNormalized("e")
            MC_central.DrawNormalized("same")
            can.SaveAs(channel+i+"_central.pdf")
        
            leg = TLegend(0.5, 0.5, 0.75, 0.75)
            leg.AddEntry(data_forward,"Data forward","le")
            leg.AddEntry(data_central,"Data central","le")
            leg.AddEntry(MC_forward,"MC forward","le")
            leg.AddEntry(MC_central,"MC central","le")
            leg.SetHeader(legchannel+(i.replace('_',' ').replace('Anti','0').replace('tag',' tag')))
        
            leg.SetFillColor(0);
            leg.SetLineColor(1);
            leg.SetBorderSize(0);
            leg.SetTextFont(42);
            leg.SetTextSize(.05);
            leg.SetFillStyle(0);

            data_central.GetXaxis().SetTitle("m_{reco} [GeV]")
            data_central.GetYaxis().SetTitle("normalised to area")
            data_central.DrawNormalized("E")
            data_forward.DrawNormalized("sameE")
            MC_forward.DrawNormalized("same")
            MC_central.DrawNormalized("same")
            leg.Draw()
        
            can.SaveAs(channel+i+"_all.pdf")
    """
