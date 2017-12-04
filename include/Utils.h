#pragma once 

#include "HistsBase.h"
#include "TreeHists.h"



bool draw_effi_withtreehists(TreeHists & treehistclass, effiPlots & efficlass, string command, string pre_weight, string post_weight, string binning, string x_axis, vector<double>rebin={}){
  treehistclass.Draw(command,pre_weight,binning,x_axis);
  treehistclass.Draw(command,post_weight,binning,x_axis);
  vector<TH1F*> pre  =  treehistclass.return_hists(command,pre_weight,binning,x_axis);
  vector<TH1F*> post =  treehistclass.return_hists(command,post_weight,binning,x_axis);

  TH1F* data_pre  = pre[0];
  TH1F* data_post = post[0];
    
  TH1F* mc_pre = pre[1];  
  TH1F* mc_post = post[1];
  
  for(unsigned int i =2; i<pre.size(); ++i){
    mc_pre->Add(pre[i]);
    mc_post->Add(post[i]);
  }
  if(rebin.size()>0){
     data_pre = (TH1F*) data_pre->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
     data_post = (TH1F*) data_post->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
     mc_pre = (TH1F*) mc_pre->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
     mc_post = (TH1F*) mc_post->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
  }
  efficlass.set_axisTitle(x_axis,"Efficency");
  efficlass.loadHists((TH1F*)data_pre->Clone(),(TH1F*)data_post->Clone(),"Data");
  efficlass.loadHists((TH1F*)mc_pre->Clone(),(TH1F*)mc_post->Clone(),"Simulation");
  efficlass.plotEffi(4,{make_pair(0,1)});
  efficlass.clear_histos();

  return true;
}

