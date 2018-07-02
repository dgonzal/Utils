#include <iostream>

#include "TreeHists.h"
#include "TChainHists.h"
#include "simplePlots.h"
#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"


int main(int argc, char** argv){
  string version = "_new";//"_new";//"_wtag_topjetcorr";//"w2jet";//"wtag_topjetcorr";
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel"+version;
  bool electron = false;
  bool errors = true;
  bool blind = true;
  string resultfile = "moneyplots.ps";
  bool single = true; 
  
  if(argc>1){
    std::string channel(argv[1]);
    if(channel == "electron")
      electron = true;
    else if(channel=="muon")
      electron = false;
    else{
      std::cout<<"you need to choose between muon & electron"<<std::endl;
      std::cout<<"your input was: "<<channel<<std::endl;
      return 1;
    }
  }
  if(argc>2){
    std::string calc_err(argv[2]);
    if(calc_err=="true")
      errors = true;
    else if(calc_err=="false")
      errors = false;
    else{
      std::cout<<"options for error handling are true & false"<<std::endl;
      std::cout<<"your input was: "<<calc_err<<std::endl;
      return 2;
    }
  }
  if(argc>3){
    resultfile = std::string (argv[3]);
  }

  if (electron){
    folder = "EleSel_new";
    if(resultfile=="moneyplots.ps")
      resultfile = "plots/Ele_"+resultfile;
    if(single) resultfile= "plots/singleEleMoney_nomsig_reco_noforwardreweight/";
  }
  else{
    if(resultfile=="moneyplots.ps")
      resultfile = "plots/Mu_"+resultfile;
    if(single) resultfile= "plots/singleMuPaper/";
  }
  cout<<"Folder: "<<folder<<" end file: "<<resultfile<<endl;
  //std::cout<<"going to print "<<resultfile<<" from "<< electron ? "electron" : "muon"<<" channel " <<errors ? " with sys errors":" without sys errors"<<endl; 

  string eletriggerfactors = "";
  if(electron)  eletriggerfactors ="("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
  string muontrkfactors =""; 
  if(!electron) muontrkfactors = "("+muon_trk_factors()+"*(1-isRealData)+isRealData)*";
  string factors = muontrkfactors+eletriggerfactors;//+forwardfit("TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400");
  //factors = eletriggerfactors;
  cout<<"applying factors: "<<factors<<endl;
  
  TreeHists treehists(resultfile,single);
  //if(single) treehists.switch_singleplots(true);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  //treehists.set_ignorePages(15);
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/Merge_Data.root","PE",1,20,false,"Data");
  /*/
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt*.root","",3,-1,true,"W+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root","",2,-1,true,"");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root","",2,-1,true,"");
  /*/
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root","",5,-1,true,"Z+Jets");
  /*/
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root","",41,-1,true,"single t");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD*.root","",4,-1,true,"QCD");
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1100_RH.root","hist",20,-1,false,"B+b M_{B}(1.1 TeV) RH",0);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1700_RH.root","hist",40,-1,false,"B+b M_{B}(1.7 TeV) RH",0);
  /*/

  treehists.set_ratiolimits(1.69,0.39);
  treehists.SetTree("AnalysisTree");
 
  if(errors){
    treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
    treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");   
    if(!electron){
      treehists.AddErrorWeight("weight_sfmu_tight_REPLACE/weight_sfmu_tight",TreeHists::error_method::envelop,"up,down");
      treehists.AddErrorWeight("weight_sfmu_muonTrigger_REPLACE/weight_sfmu_muonTrigger",TreeHists::error_method::envelop,"up,down");
      treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_down_MuSelUNC/"});
      treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_down_MuSelUNC/"});
    }
    else{
      //ele trigger variations
      treehists.AddErrorWeight("REPLACE",TreeHists::error_method::envelop,eletriggerscale(1)+","+eletriggerscale(2));

      treehists.AddErrorWeight("weight_sfelec_eleid_REPLACE/weight_sfelec_eleid",TreeHists::error_method::envelop,"up,down");
      treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_up_EleSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_down_EleSelUNC/"});
      treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_up_EleSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_down_EleSelUNC/"});
    }
    treehists.AddErrorFolderAlias("slimmedJets.","jet_jec_up."  ,"jec_up");
    treehists.AddErrorFolderAlias("slimmedJets.","jet_jec_down.","jec_down");
    treehists.AddErrorFolderAlias("slimmedJets.","jet_jer_up."  ,"jer_up");
    treehists.AddErrorFolderAlias("slimmedJets.","jet_jer_down.","jer_down");

    treehists.AddErrorFolderAlias("slimmedMETs.","met_jec_up."  ,"jec_up");
    treehists.AddErrorFolderAlias("slimmedMETs.","met_jec_down.","jec_down");
    treehists.AddErrorFolderAlias("slimmedMETs.","met_jer_up."  ,"jer_up");
    treehists.AddErrorFolderAlias("slimmedMETs.","met_jer_down.","jer_down");
  }
  //treehists.switch_logy(true);

  
  
  TLatex *cms_text = new TLatex(0.9, 0.95,"#mu channel 35.9 fb^{-1} (13 TeV)");
  if(electron) cms_text = new TLatex(0.9, 0.95,"e channel 35.9 fb^{-1} (13 TeV)");
  cms_text->SetNDC();
  cms_text->SetTextAlign(33);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.04);

  TLatex *preliminary_text = new TLatex(0.1, 0.95,"CMS");
  preliminary_text->SetNDC();
  preliminary_text->SetTextAlign(13);
  preliminary_text->SetTextFont(42);
  preliminary_text->SetTextSize(0.045);
 
  TLatex *chi2_text = new TLatex(0.8, 0.3,"X^{2} reconstruction");
  chi2_text->SetNDC();
  chi2_text->SetTextAlign(33);
  chi2_text->SetTextFont(42);
  chi2_text->SetTextSize(0.05);
  
  TLatex *toptag_text = new TLatex(0.8, 0.3,"t-tag reconstruction");
  toptag_text->SetNDC();
  toptag_text->SetTextAlign(33);
  toptag_text->SetTextFont(42);
  toptag_text->SetTextSize(0.05);

  string eta = "2.4";
  string chi2_central_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) >="+eta;
  string toptag_scalefactor = "1.01";
  //string wtag_scalefactor = "";
  treehists.clearText();
  treehists.addText(cms_text);
  treehists.addText(preliminary_text);
  treehists.addText(chi2_text);
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) < 2.4 )","30,300,3500","m_{reco} [GeV]","Events", true, "", "chi2_mass");

  treehists.clearText();
  treehists.addText(cms_text);
  treehists.addText(preliminary_text);
  treehists.addText(toptag_text);
  treehists.Draw("TopTagDis.mass",factors+"weight*("+toptag_central_string+")","30,300,3500","m_{reco} [GeV]","Events", true, "", "toptag_mass");

}
