#! /usr/bin/env python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)


class histo_store:
    def __init__(self, categories, sample, uncertainties):
        self.categories = categories
        self.uncertainties = uncertainties
        self.sample = sample
        self.histos = [None]*len(categories)
        self.uncer_histos =[]
        #self.uncer_creator=[]
        for i in xrange(len(categories)):
            self.uncer_histos.append([None]*len(uncertainties))
            #self.uncer_creator.append([0]*len(uncertainties))
        #print categories,uncertainties,sample
        
    def check_key(self, hist_name, root):
        if self.sample in hist_name:
            for i, cat in enumerate(self.categories):
                #print cat
                if cat in hist_name:
                    new_hist_name = hist_name.replace(hist_name.split('__')[1],self.sample)
                    if len(hist_name.split('__'))<3:
                        if self.histos[i]: self.histos[i].Add(root.Get(hist_name))
                        else: self.histos[i] = root.Get(hist_name).Clone(new_hist_name)
                        return True
                    else:
                        for m, unc in enumerate(self.uncertainties):
                            #print unc, hist_name
                            if unc in hist_name:
                                #print unc, hist_name
                                #print self.uncer_creator
                                if not self.uncer_histos[i][m]:
                                    #print 'create',i,m, new_hist_name
                                    self.uncer_histos[i][m]  = root.Get(hist_name).Clone(new_hist_name)
                                    #self.uncer_creator[i][m] = 1
                                else:
                                    #print 'add',i,m, str(self.uncer_histos[i][m].GetName())
                                    self.uncer_histos[i][m].Add(root.Get(hist_name))
                                return True
        return False
    
    def write(self):
        for m, item in enumerate(self.histos):
            if item:
                for i in xrange(item.GetNcells()):
                    if item.GetBinContent(i)<=0: item.SetBinContent(i,0.001)
                item.Write()
            else:
                help_hist = ROOT.TH1F(self.categories[m]+'__'+self.sample,self.categories[m]+'__'+self.sample, 60,300,3000)
                help_hist.Write()
                        
        for m, collection in enumerate(self.uncer_histos):
            for p, item in enumerate(collection):
                if item:
                    for i in xrange(item.GetNcells()):
                        if item.GetBinContent(i)<=0: item.SetBinContent(i,0.001)
                    #print 'writing',str(item.GetName())
                    item.Write()
                else:
                    help_hist = ROOT.TH1F(self.categories[m]+'__'+self.sample+'__'+self.uncertainties[p],self.categories[m]+'__'+self.sample+'__'+self.uncertainties[p], 60,300,3000)
                    help_hist.Write()
        

def merge_mc(mc_samples, input_fname, output=''):
    TH1.AddDirectory(ROOT.kFALSE);    
    in_file = TFile(input_fname)
    if not output:
        output = input_fname.replace('.root', '_mc_merge.root')

    print 'going to merge the different mc contribution to one histogram'
    print 'input file',input_fname
    print 'output file',output
    print 'samples:',mc_samples

    keys = in_file.GetListOfKeys()

    categories = []
    uncertainties = []
    
    for key in keys:
        key = str(key.GetName())
        category = key.split('__')[0]
        uncertainty = None
        if len(key.split('__'))>2:
           uncertainty = key.split('__')[2]+'__'+key.split('__')[3]
        if category not in categories: categories.append(category)
        if uncertainty and uncertainty not in uncertainties: uncertainties.append(uncertainty)

           
    print 'considered categories:',categories 
    print 'considered uncertainties:',uncertainties
        
    sample_histos=[]
    for item in mc_samples:
        sample_histos.append(histo_store(categories,item,uncertainties))
        

    for key in keys:
        key = str(key.GetName())
        #print key
        for item in sample_histos:
            if item.check_key(key,in_file): break
        #print '%'*10
            
    in_file.Close()
    out_file = TFile(output,"RECREATE")
    out_file.cd()
    for item in sample_histos:
        item.write()
    out_file.Close()
    return output
