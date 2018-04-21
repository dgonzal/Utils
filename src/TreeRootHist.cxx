#include "TreeRootHist.h"


using namespace std;

TreeRootHist::TreeRootHist(std::string saveName, bool single): HistsBase(saveName, single){
  normArea=false;
  max=-1;
  pad1 = NULL;
  pad2 = NULL;
  legx1=0.6; legx2= 0.92; legy1=0.6; legy2=0.9;
}

TTree* TreeRootHist::load_tree(std::string fileDir){
  Long_t id=0, size=0, flag=0, modtime=0;
  if(gSystem->GetPathInfo(fileDir.c_str(),&id, &size, &flag, &modtime)!=0){
    std::cerr<<"File "<<fileDir.c_str()<<" does not exist"<<std::endl;
    std::cerr<<"id "<<id<<" size "<<size<<" flag "<<flag<<" modtime "<<modtime<<std::endl;
    exit(EXIT_FAILURE);
  }
  TFile* file = new TFile(fileDir.c_str(),"READ");
  if(file->IsZombie()){
    cerr<<"File "<<fileDir<<" is a Zombie"<<endl;
    exit(EXIT_FAILURE);
  }
  if(file->GetListOfKeys()->Contains(treeName.c_str())==0){
    cerr<<"File "<<fileDir<<" does not contain "<<treeName<<endl;
    exit(EXIT_FAILURE); 
    
  }
  if(debug)std::cout<<" Load Tree "<<treeName<<" from "<<fileDir<<std::endl;
  return (TTree*)file->Get(treeName.c_str());   
  
}

vector<TH1F*> TreeRootHist::return_hists(string variable, string draw_option, string binning, std::string x_axis, std::string y_axis){
  vector<TH1F*> result;  
  for(const auto & fileDir : get_filedirs()){
    TTree* mytree = load_tree(fileDir);
    string file_name = "";
    result.push_back(make_hist(mytree, variable, binning, draw_option));
  }
  return result;
}

TH1F* TreeRootHist::make_hist(TTree* mytree, std::string variable, std::string binning, std::string draw_option, std::string plot_name){
  std::string name = "myTmpHist";
  if(!plot_name.empty()){
    name = plot_name;
    boost::replace_all(name, "(", "");
    boost::replace_all(name, ")", "");
    boost::replace_all(name, " ", "");
    boost::replace_all(name, "+", "");
    boost::replace_all(name, "#", "");
    //boost::replace_all(name, "__", "_");
  }
  if(binning.empty()){
    mytree->Draw((variable+">>"+name).c_str(),draw_option.c_str());
  }
  else{
    mytree->Draw((variable+">>"+name+"("+binning+")").c_str(),draw_option.c_str());
  }
  return (TH1F*)gPad->GetPrimitive(name.c_str());
}


void TreeRootHist::addUnc(std::string weight_up, std::string weight_down, std::string method, std::string name){
  uncer_info tmp = uncer_info();
  tmp.method       = method;
  tmp.weight_up    = weight_up;
  tmp.weight_down  = weight_down;
  tmp.nick         = name;
  unc_info.push_back(tmp);
}


void TreeRootHist::envelopmodule(sample &proc, uncer_info info, std::string variable, std::string binning, std::string modified_draw_option, std::string plot_name, std::string x_axis){
  TH1F* up, *down;
  bool created = false;
  std::string up_draw   =  modified_draw_option+"*"+info.weight_up;
  std::string down_draw =  modified_draw_option+"*"+info.weight_down;
  for(auto mytree : proc.trees){
    if(created){
      up->Add(make_hist(mytree, variable, binning,up_draw));
      down->Add(make_hist(mytree, variable, binning, down_draw));
    }
    else{
      up  = make_hist(mytree, variable, binning,up_draw,plot_name+proc.nick+"__"+info.nick+"__plus");
      down= make_hist(mytree, variable, binning, down_draw,plot_name+proc.nick+"__"+info.nick+"__minus");
      created = true;
    }
  }
  for(int i = 1; i<up->GetNcells()-1;i++){
    double max = up->GetBinContent(i);
    double min = down->GetBinContent(i);
    if(min>max){
      up->SetBinContent(i,min);
      down->SetBinContent(i,max);
    }
  }
  uncertainty tmp = uncertainty();
  tmp.up   = up;   tmp.up->SetTitle(x_axis.c_str());
  tmp.down = down; tmp.down->SetTitle(x_axis.c_str());
  tmp.unc_nick = info.nick;
  proc.uncertainties.push_back(tmp);

}

void TreeRootHist::rmsmodule(sample &proc, uncer_info info, std::string variable, std::string binning, std::string modified_draw_option, std::string plot_name, std::string x_axis){
  TH1F* rms;
  bool created = false;
  std::string rms_draw   =  modified_draw_option+"*"+info.weight_up;
  for(auto mytree : proc.trees){
    if(created){
      rms->Add(make_hist(mytree, variable, binning,rms_draw));
    }
    else{
      rms = make_hist(mytree, variable, binning,rms_draw);
      created = true;
    }
  }
  rms->Scale(0.5);
  uncertainty tmp = uncertainty();
  tmp.up   = (TH1F*)proc.hist->Clone((plot_name+proc.nick+"__"+info.nick+"__plus").c_str());
  tmp.down = (TH1F*)proc.hist->Clone((plot_name+proc.nick+"__"+info.nick+"__minus").c_str());
  tmp.up->Add(rms);
  tmp.down->Add(rms,-1);
  tmp.up->SetTitle(x_axis.c_str());
  tmp.down->SetTitle(x_axis.c_str());
  tmp.unc_nick = info.nick;
  proc.uncertainties.push_back(tmp);

}

vector<std::string> TreeRootHist::autocomplete_filedir(std::string filedir){
  if(boost::algorithm::contains(filedir,"*"))
    return matching(filedir);
  else
    return {filedir};
}

bool TreeRootHist::load_folder_trees(sample &proc, std::string filedir, uncer_info info){
  uncertainty tmp = uncertainty();
  tmp.unc_nick = info.nick;
  vector<string> splitted_string;
  boost::split(splitted_string,filedir,boost::is_any_of("/"));
  std::string file_name = splitted_string.at(splitted_string.size()-1);
  for(const auto &f : autocomplete_filedir(info.weight_up+"/"+file_name))
      tmp.trees_up.push_back(load_tree(f));
  for(const auto &f : autocomplete_filedir(info.weight_down+"/"+file_name))
      tmp.trees_down.push_back(load_tree(f));
  proc.folder_uncertainties.push_back(tmp);
  return true;
}


bool TreeRootHist::load_trees_for_samples(){
  work_samples = get_samples();
  //load trees
  std::cout<<"number of samples "<<work_samples.size()<<std::endl;
  for(auto & proc : work_samples){
    if(debug)
      std::cout<<" nick "<<proc.nick<<" dir "<<proc.dir <<std::endl;
    std::string filedir = proc.dir;
    for(const auto &f : autocomplete_filedir(filedir))
      proc.trees.push_back(load_tree(f));

    for(const auto & unc : unc_info){
      if(unc.method.compare("folder")==0)
	load_folder_trees(proc, filedir, unc);
    }
  }
  loaded_trees=true;
  return true;
}




bool TreeRootHist::fill_histograms(std::string variable, std::string draw_option, std::string binning, std::string x_axis, std::string y_axis, bool legend, std::string data_draw_option, std::string plot_name){
  //fill histograms
  for(auto & proc : work_samples){
    std::string modified_draw_option = draw_option +"*"+to_string(proc.scale);
    if(debug){
      std::cout<<"variable "<<variable<<" binning "<<binning<<" section "<<modified_draw_option<<std::endl;
      std::cout<<"number of trees "<<proc.trees.size()<<std::endl;
    }
    for(auto mytree : proc.trees){
      if(proc.created)
	proc.hist->Add(make_hist(mytree, variable, binning, modified_draw_option));
      else{
	std::cout<<"initializing hist for "<<proc.nick<<" with "<<plot_name+proc.nick<<std::endl;
	proc.hist= make_hist(mytree, variable, binning, modified_draw_option,plot_name+proc.nick);
	proc.hist->SetTitle(x_axis.c_str());
	proc.created = true;
      }
    }
    if(boost::contains(proc.nick,"data") || boost::contains(proc.nick,"Data") || boost::contains(proc.nick,"DATA") ) continue;
    for(auto & i :unc_info){
      if(i.method.compare("envelop")==0)
	envelopmodule(proc, i, variable, binning, modified_draw_option, plot_name, x_axis);
      else if(i.method.compare("rms")==0)
	rmsmodule(proc, i, variable, binning, modified_draw_option, plot_name, x_axis);
      else if(i.method.compare("folder")==0){}
      else
	std::cerr<<"method not found "<<i.method<<std::endl;
    }
    for(auto &unc: proc.folder_uncertainties){
      bool created = false;
      for(const auto & uptree : unc.trees_up){
	if(created)
	  unc.up->Add(make_hist(uptree, variable, binning, modified_draw_option));
	else{
	  std::cout<<"initializing hist for "<<proc.nick<<" with "<<plot_name+proc.nick<<std::endl;
	  unc.up= make_hist(uptree, variable, binning, modified_draw_option,plot_name+proc.nick);
	  unc.up->SetTitle(x_axis.c_str());
	  created =   true;
	}
      }
      created = false;
      for(const auto & downtree : unc.trees_down){
	if(created)
	  unc.down->Add(make_hist(downtree, variable, binning, modified_draw_option));
	else{
	  std::cout<<"initializing hist for "<<proc.nick<<" with "<<plot_name+proc.nick<<std::endl;
	  unc.down= make_hist(downtree, variable, binning, modified_draw_option,plot_name+proc.nick);
	  unc.down->SetTitle(x_axis.c_str());
	  created =   true;
	}
      }
    }
  }
  return true;
}

bool TreeRootHist::histo_cleanup(){
  for(auto & proc : work_samples){
    proc.created = false;
    proc.uncertainties = std::vector<uncertainty>();
  }
  return true;
}

bool TreeRootHist::save_histograms_to_root(std::string file_name){
  //store in root file
  TFile* plot_file = new TFile((file_name).c_str(),"RECREATE");
  plot_file->cd();
  for(const auto &proc : work_samples){
    proc.hist->Write();
    for(const auto &unc :  proc.uncertainties){
      unc.up->Write();
      unc.down->Write();
    }
  }
  plot_file->Close();
  return true;
}
  
bool TreeRootHist::Draw_ROOT(std::string variable, std::string draw_option, std::string binning, std::string x_axis, std::string y_axis, bool legend, std::string data_draw_option, std::string plot_name){
  if(!loaded_trees)
    load_trees_for_samples();
    
  fill_histograms(variable, draw_option, binning, x_axis, y_axis, legend, data_draw_option, plot_name);
  std::string file_name = get_resultFile().Data()+to_string(page_counter)+".root";
  if(!plot_name.empty()){
    file_name = get_resultFile().Data()+plot_name+".root";
    boost::replace_all(file_name,"__","");
  }
  else
    page_counter++;
  std::cout<<"re-creating root file "<<file_name<<std::endl;
  save_histograms_to_root(file_name);
  histo_cleanup();
  return true;
}
/*/
bool TreeRootHist::Draw(std::string variable, std::string draw_option, std::string binning, std::string x_axis, std::string y_axis, bool legend, std::string data_draw_option, std::string plot_name){
  
  return true;
}
/*/
