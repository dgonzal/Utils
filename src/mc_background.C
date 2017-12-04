#include <iostream>

#include "TreeHists.h"
#include "TChainHists.h"
#include "simplePlots.h"
#include "eletriggerresult.h"
#include "forwardjetfitresult.h"

int main(int argc, char** argv){
  string version = "_new";//"_wtag_topjetcorr";//"w2jet";//"wtag_topjetcorr";
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel"+version;
  bool electron = false;
  bool errors = false;
  bool blind = true;
  string resultfile = "new_test.ps";//"moneyplots.pdf";
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
    folder = "EleSel_cross";
    if(resultfile=="moneyplots.ps")
      resultfile = "plots/Ele_"+resultfile;
    if(single) resultfile= "plots/singleEleMoneyForwardJet_reco/";
  }
  else{
    if(resultfile=="moneyplots.ps")
      resultfile = "plots/Mu_"+resultfile;
    if(single) resultfile= "plots/singleMuMoneyForwardJet_reco/";
  }
  cout<<"Folder: "<<folder<<" end file: "<<resultfile<<endl;
  //std::cout<<"going to print "<<resultfile<<" from "<< electron ? "electron" : "muon"<<" channel " <<errors ? " with sys errors":" without sys errors"<<endl; 

  string eletriggerfactors = "";
  if(electron)  eletriggerfactors ="("+eletriggerscale()+"*(1-isRealData)+isRealData)*"  ;
  string factors = eletriggerfactors+forwardfit("TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400");
  //factors = eletriggerfactors;
  cout<<"applying factors: "<<factors<<endl;



  
  TreeHists treehists(resultfile,single);
  //if(single) treehists.switch_singleplots(true);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  //treehists.set_ignorePages(15);
  if(!electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuData.root","PE",1,20,false,"Data");
  if(electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleEleData.root","PE",1,20,false,"Data");

  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt*.root","",3,-1,true,"W+Jets"); 
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root","",2,-1,true,"");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root","",2,-1,true,"");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root","",41,-1,true,"single t");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD*.root","",4,-1,true,"QCD");
  /*/
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",4,-1,false,"B+b M(0.8) RH #times 3",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",7,-1,false,"B+t M(0.8) RH #times 9",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",20,-1,false,"B+b M(1) RH #times 6",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root","hist",30,-1,false,"B+t M(1) RH #times 18",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",40,-1,false,"B+b M(1.5) RH #times 24",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",800,-1,false,"B+t M(1.5) RH #times 71",0,5);
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

  TLatex *lumi_text = new TLatex(3.5, 24,"35.8 fb^{-1} (13 TeV)"); //CMS Preliminary
  lumi_text->SetNDC();
  lumi_text->SetTextAlign(33);
  lumi_text->SetX(0.9);
  lumi_text->SetTextFont(42);
  lumi_text->SetTextSize(0.04);
  lumi_text->SetY(.95);
  treehists.addText(lumi_text);
  TLatex *preliminary_text = new TLatex(3.5, 24,"CMS #it{Preliminary}");
  preliminary_text->SetNDC();
  preliminary_text->SetTextAlign(33);
  preliminary_text->SetX(0.5);
  preliminary_text->SetTextFont(42);
  preliminary_text->SetTextSize(0.045);
  preliminary_text->SetY(.95);
  treehists.addText(preliminary_text);
  

  string eta = "2.4";
  string chi2_central_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) >="+eta;
  string toptag_scalefactor = "1.01";
  //string wtag_scalefactor = "";

  
  //VLQ mass central
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","central X^{2} 0 b-tags B mass [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","central X^{2} 1 b-tag B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","central X^{2} 2 b-tag B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) < 2 )","50,500,3000","central W-tag B mass","Events");
  treehists.Draw("TopTagDis.mass",factors+"weight*("+toptag_central_string+")","30,500,3000","central toptag B mass [GeV]");

  return 0;

  if(blind){
    treehists.removeFile(0);
    treehists.mcratio_only();
    //treehists.switch_ratio(false);
  }
  
 
  //treehists.switch_logy(true);
  //VLQ mass forward/total
  treehists.Draw("Chi2Dis.mass",factors+"weight*(TopTagDis.mass==-1)","30,500,3000","B mass [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","forward X^{2} 0 b-tags B mass [GeV]");	    
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","forward X^{2} 1 b-tag B mass [GeV]" );	    
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","forward X^{2} 2 b-tag B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) >= 2)","30,0,3000","forward W-tag B mass","Events");
  
  //treehists.switch_logy(false);
  treehists.Draw("TopTagDis.mass",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1 && TopTagDis.topHad.pt()>=400)","30,500,3000","B mass [GeV]");
  treehists.Draw("TopTagDis.mass",factors+toptag_scalefactor+"*weight*("+toptag_forward_string+")","30,500,3000","forward toptag B mass [GeV]");
 
 
  return 0;


}
