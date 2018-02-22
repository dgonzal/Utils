#include <iostream>
#include <stdlib.h>

#include "TreeHists.h"
#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"
#include "TreeConfig.h"

#include <boost/algorithm/string.hpp>

void print_vectorstring(std::vector<std::string> myvec){
  for(auto i : myvec)
    std::cout<<i<<endl;
}

std::string replace_deltaR(std::string input);


int main(int argc, char* argv[]){
  std::vector<std::string> config_files;
  for(int i=1; i< argc; i++){
    config_files.push_back(argv[i]);
  }

 
  TreeConfig myconfig(config_files);

  string eletriggerfactors = "";
  if(myconfig.get_eletrigcorr()) eletriggerfactors = "("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
  string muontrkfactors    = "";
  if(myconfig.get_mutrkcorr())   muontrkfactors    ="("+muon_trk_factors()+"*(1-isRealData)+isRealData)*";
  string forward_corr      ="";
  if(myconfig.get_forwardcorr()) forward_corr      =forwardfit("TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400"); 
  string corr_factor = muontrkfactors+eletriggerfactors+forward_corr;
  
  
  //configure plotter
  if(myconfig.get_name().empty()){
    std::cout<<"No name given where to store the the histograms aborting"<<std::endl;
    return 1;
  }
  TreeHists treehists(myconfig.get_name(),myconfig.get_single());
  // add the files that are given in the config
  for(auto file : myconfig.get_files())
    treehists.addFile(file.filedir, file.hist_draw_option, file.color, file.marker, file.stack, file.nickname, file.uncer, file.scalefactor);

  //add texts
  for(auto text : myconfig.get_tlatex())
    treehists.addText(text);
  // mc ratio only
  
  if(myconfig.get_mcratio())
    treehists.mcratio_only(true);
  //switch ratio on/off
  treehists.switch_ratio(myconfig.get_ratio());
    
  // ratio limits
  treehists.set_ratiolimits(myconfig.get_ratioLimits()[0],myconfig.get_ratioLimits()[1]);
  // analysistree
  treehists.SetTree(myconfig.get_analysisTree());
  // set legend position
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  
  // Uncertainties: 
  // 1.  Uncertainties calculated through weights
  // 2a. Uncertainties calculated from additional files in different folders.
  // 2b. Change some of the variables for the tree that are not the same.
  for(auto error : myconfig.get_errorWeights()){
    if(error.size()<2){
      std::cout<<"Not enough parameters in error "<<std::endl;
      print_vectorstring(error);
      return -1;
    }
    if(boost::algorithm::contains(error[1],"rms"))
      treehists.AddErrorWeight(error[0],TreeHists::error_method::rms);
    else if(boost::algorithm::contains(error[1],"envelop")) 
      treehists.AddErrorWeight(error[0],TreeHists::error_method::envelop,"up,down");
    else if(boost::algorithm::contains(error[1],"ELETRIGGER"))
      treehists.AddErrorWeight("REPLACE",TreeHists::error_method::envelop,eletriggerscale(1)+","+eletriggerscale(2));
    else if(boost::algorithm::contains(error[1],"MUTRK"))
      treehists.AddErrorWeight("REPLACE",TreeHists::error_method::envelop,muon_trk_factors(-1)+","+muon_trk_factors(1));
    else{
      std::cout<<"Method how to calculate the error unknown"<<std::endl;
      print_vectorstring(error);
      return -2;
    }
  }
  for(auto error : myconfig.get_errorFolders())
    treehists.AddErrorFolder(error);
  for(auto alias : myconfig.get_folderalias()){
    //std::cout<<" size of alias "<<alias.size()<<std::endl;
    if(alias.size()==2)
      treehists.AddErrorFolderAlias(alias[0],alias[1]);
    else if(alias.size()==3)
      treehists.AddErrorFolderAlias(alias[0],alias[1],alias[2]);
    else{
      std::cout<<"Number of arguments for alias wrong"<<std::endl;
      print_vectorstring(alias);
      return -3;
    }
  }
  std::cout<<"************************"<<std::endl;
  std::cout<<"Done with the configuration starting to produce the histograms"<<std::endl;
  //draw the histograms
  //TreeHists::Draw(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events", bool legend=true, std::string data_draw_option="");
  for(auto draw : myconfig.get_drawCommands()){
    draw.print();
    treehists.switch_logy(draw.logy);
    if(boost::algorithm::contains(draw.command,"deltaR")){
      draw.command=replace_deltaR(draw.command);
      std::cout<<"replaced the deltaR in command "<<draw.command<<std::endl;
    }
    if(boost::algorithm::contains(draw.weight,"deltaR")){
      draw.command=replace_deltaR(draw.weight);
      std::cout<<"replaced the deltaR in weight "<<draw.weight<<std::endl;
    }
    treehists.Draw(draw.command, corr_factor+draw.weight, draw.binning, draw.x_axis,draw.y_axis,draw.legend,draw.data_option);
  }
  return 0;
}


  
std::string replace_deltaR(std::string input){
  std::string tmp = input;
  std::string result ="";
  std::cout<<"starting to find the replacemant for delta R"<<std::endl;
  
  while(tmp.find("deltaR(")!=std::string::npos){
    int start = tmp.find("deltaR(");
    result += tmp.substr(0,start);
    tmp=tmp.substr(start+7);
    int first_var = tmp.find(",");
    int second_var = tmp.find(")");
    result += deltaRstring(tmp.substr(0,first_var),tmp.substr(first_var+1,second_var-first_var-1));//+")";
    tmp= tmp.substr(second_var);
  }
  return result;
}
