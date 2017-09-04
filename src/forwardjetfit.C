#include <iostream>

#include "TreeHists.h"
#include "TChainHists.h"
#include "simplePlots.h"
#include "eletriggerresult.h"

int main(int argc, char** argv){
  string version = "_wtag_topjetcorr";//"w2jet";//"wtag_topjetcorr";
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel"+version;
  bool electron = true;
  bool errors = true;
  string resultfile = "forwardjet_scale_ele.ps";
  bool single = false;
  string eletriggerfactors = "";
  if(electron){
    eletriggerfactors ="("+eletriggerscale()+"*(1-isRealData)+isRealData)*"  ;
    folder = "EleSel_cross";
  }
  string factors = eletriggerfactors;

  TreeHists treehists(resultfile,single);
  //if(single) treehists.switch_singleplots(true);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  //treehists.set_ignorePages(1);
  if(!electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuData.root","PE",1,20,false,"Data");
  if(electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleEleData.root","PE",1,20,false,"Data");


  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root","",3,-1,true,"W+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root","",2,-1,true,"");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root","",2,-1,true,"");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"single t");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");

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
  

  string eta = "2.0";
  string chi2_central_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) >="+eta;
  string toptag_scalefactor = "1.01";
  //string wtag_scalefactor = "";
  //forward jet
  //X2

  treehists.fit_ratio(true,"[0]*x+[1]") ;
  treehists.Draw("fabs(Chi2Dis.forwardJet.eta())",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.forwardJet.pt() > 0 )","5,3,5","X^{2} forward ak4 #eta","Events");
  
 
  //treehists.Draw("Chi2Dis.forwardJet.energy()",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 )","50,100,1200","X^{2} forward ak4 E","Events");
  //treehists.Draw("Chi2Dis.forwardJet.pt()",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 )","50,30,200","X^{2} forward ak4 p_{T}","Events");

 
 
  return 0;
}
