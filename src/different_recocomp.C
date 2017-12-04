#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"



int main(int argc, char** argv){
  string version = "new";
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel_"+version;
  bool electron = false;
  bool errors = false;
  string resultfile = "muon_chi2_prob_van.ps";
  bool single = false;

 
  //std::cout<<"going to print "<<resultfile<<" from "<< electron ? "electron" : "muon"<<" channel " <<errors ? " with sys errors":" without sys errors"<<endl; 

  TreeHists treehists(resultfile,single);
  //if(single) treehists.switch_singleplots(true);
  treehists.SetLegend(0.6, 0.3, 0.8, 0.7);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",6,-1,false,"B+b M(0.8) new ",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",5,-1,false,"B+t M(0.8) new",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",20,-1,false,"B+b M(1) new",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root","hist",4,-1,false,"B+t M(1) new",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",7,-1,false,"B+t M(1.5) new",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",8,-1,false,"B+b M(1.5) new",0,5);

  //treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",40,-1,false,"B+b M(1.5)",0,5);
  //treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",800,-1,false,"B+t M(1.5)",0,5);
  folder = "MuSel_wtag_topjetcorr";
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",40,-1,false,"B+b M(0.8)",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root","hist",2,-1,false,"B+t M(1)",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",3,-1,false,"B+t M(1.5)",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",42,-1,false,"B+b M(0.8)",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",50,-1,false,"B+b M(1)",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",60,-1,false,"B+b M(1.5)",0,5);


  
  treehists.SetTree("AnalysisTree");
  treehists.switch_ratio(false);
  if(errors){
    treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
    treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");
    //if(!electron){
      //treehists.AddErrorWeight("weight_sfmu_mediummuon2016_REPLACE/weight_sfmu_mediummuon2016",TreeHists::error_method::envelop,"up,down");
      //treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_down_MuSelUNC/"});
      //treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_down_MuSelUNC/"});
    //}
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

  TLatex *cms_text = new TLatex(3.5, 24," 35.8 fb^{-1} (13 TeV)"); //CMS Preliminary
  cms_text->SetNDC();
  cms_text->SetTextAlign(33);
  cms_text->SetX(0.9);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.04);
  cms_text->SetY(.95);
  treehists.addText(cms_text);

  string eta = "2.0";
  string energy = "0.";
  string chi2_central_string = "TopTagDis.mass==-1 &&((abs(Chi2Dis.forwardJet.eta()) <" +eta+") || Chi2Dis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "TopTagDis.mass==-1 && (abs(Chi2Dis.forwardJet.eta()) >="+eta+") && Chi2Dis.forwardJet.E()>="+energy;
  string toptag_central_string = "TopTagDis.mass>-1 &&((abs(TopTagDis.forwardJet.eta()) <" +eta+") || TopTagDis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "TopTagDis.mass>-1 && (abs(TopTagDis.forwardJet.eta()) >="+eta+") && TopTagDis.forwardJet.E()>="+energy;

  string chi2cut ="Chi2Dis.chi<500000";
  
  //AK4 Jets
  //treehists.Draw("Length$(slimmedJets.slimmedJets.m_eta)","weight *(slimmedMETs.m_pt>0)","16,-0.5,15.5","Number of ak4","Events",false);
  //treehists.Draw("slimmedJets.slimmedJets.m_eta","weight","50,-5,5","ak4 #eta","Events",false);
  /*/
  treehists.Draw(deltaRstring("WTagDis.wHad","WTagDis.topJets"),"weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62 && WTagDis.topJets.pt()>20 )","70,0,4","W-tag top_{had} #DeltaR(W,b)");
  
  treehists.Draw("WTagDis.wHad.M()","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62 && WTagDis.topJets.pt()>20 && "+deltaRstring("WTagDis.wHad","WTagDis.topJets")+"<0.8)","70,0,200","W-tag top_{had} #DeltaR(W,b)");
  treehists.Draw("WTagDis.wHad.pt()","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62 && WTagDis.topJets.pt()>20 && "+deltaRstring("WTagDis.wHad","WTagDis.topJets")+"<0.8)","70,0,800","W-tag top_{had} #DeltaR(W,b)");
  treehists.Draw("WTagDis.mass","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62 && WTagDis.topJets.pt()>20 && "+deltaRstring("WTagDis.wHad","WTagDis.topJets")+"<0.8)","70,0,3000","W-tag top_{had} #DeltaR(W,b)");

  
  treehists.Draw(deltaRstring("WTagDis.wLep","WTagDis.topJets"),"weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==61 && WTagDis.topJets.pt()>20 )","70,0,4","W-tag top_{lep} #DeltaR(W,b)");
  /*/
  treehists.Draw("Chi2Dis.num_whad","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+")","6,-.5,5.5","number of jets in W_{had}");
  treehists.Draw("Chi2Dis.num_top","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+")","6,-.5,5.5","number of add. jets in top");
  treehists.Draw("Chi2Dis.num_top+Chi2Dis.num_whad","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+")","6,-.5,5.5","number of jets in B");
  
  treehists.Draw("Chi2Dis.num_whad","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+" && Chi2Dis.recoTyp==12)","6,-.5,5.5","number of jets in W_{had} for top_{had}");
  treehists.Draw("Chi2Dis.num_top","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+" && Chi2Dis.recoTyp==12)","6,-.5,5.5","number of add. jets in top for top_{had}");
  treehists.Draw("Chi2Dis.num_top+Chi2Dis.num_whad","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+"&& Chi2Dis.recoTyp==12)","6,-.5,5.5","number of jets in B for top_{had}");
  

 
  treehists.Draw("Chi2Dis.num_whad","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+"&& Chi2Dis.recoTyp==11)","6,-.5,5.5","number of jets in W_{had} for top_{lep}");
  treehists.Draw("Chi2Dis.num_top","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+"&& Chi2Dis.recoTyp==11)","6,-.5,5.5","number of add. jets in top for top_{lep}");
  treehists.Draw("Chi2Dis.num_top+Chi2Dis.num_whad","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+"&& Chi2Dis.recoTyp==11)","6,-.5,5.5","number of jets in B for top_{lep}");


  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+"&& Chi2Dis.recoTyp==12)","11,-.5,10.5","forward jet candidates for top_{had}");
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+"&& Chi2Dis.recoTyp==11)","11,-.5,10.5","forward jet candidates for top_{lep}");
  treehists.Draw("Chi2Dis.recoTyp==12","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+")","6,-.5,5.5","X^{2} reconstruction mode");
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+")","11,-.5,10.5"," number of forward jet candidates");

  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 && abs(Chi2Dis.forwardJet.eta()) <" +eta+" && "+chi2cut+")","11,-.5,10.5"," number of forward jet candidates, central");
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 && abs(Chi2Dis.forwardJet.eta()) >=" +eta+" && "+chi2cut+")","11,-.5,10.5"," number of forward jet candidates, forward");
  
  treehists.switch_logy(true);
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+"&& Chi2Dis.recoTyp==12)","11,-.5,10.5","forward jet candidates for top_{had}");
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+"&& Chi2Dis.recoTyp==11)","11,-.5,10.5","forward jet candidates for top_{lep}");
  treehists.Draw("Chi2Dis.recoTyp==12","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 &&"+chi2cut+")","6,-.5,5.5","X^{2} reconstruction mode");
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500&&"+chi2cut+")","11,-.5,10.5"," number of forward jet candidates");
  
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 && abs(Chi2Dis.forwardJet.eta()) <" +eta+" && "+chi2cut+")","11,-.5,10.5"," number of forward jet candidates, central");
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_pt) - (Chi2Dis.num_top+Chi2Dis.num_whad)","weight*(TopTagDis.mass==-1 && Chi2Dis.mass>500 && abs(Chi2Dis.forwardJet.eta()) >=" +eta+" && "+chi2cut+")","11,-.5,10.5"," number of forward jet candidates, forward ");
  
  treehists.switch_logy(false);
  //X2 reco masses
  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && "+chi2cut+")","70,0,300","X^{2} top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && "+chi2cut+")","70,0,300","X^{2} top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1 && "+chi2cut+")","70,0,120","X^{2} W_{had} mass [GeV]","Events",false);
  treehists.Draw("Chi2Dis.wHad.pt()","weight*(TopTagDis.mass==-1 && "+chi2cut+")","70,30,700","X^{2} W_{had} p_{T} [GeV]","Events",false);


  treehists.Draw("Chi2Dis.topHad.pt()/Chi2Dis.wLep.pt()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && "+chi2cut+")","70,0,5","X^{2} p_{T} ratio top_{had} / W_{lep}");
  treehists.Draw("Chi2Dis.topLep.pt()/Chi2Dis.wHad.pt()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && "+chi2cut+")","70,0,5","X^{2} p_{T} ratio top_{lep} / W_{had}");

  treehists.Draw(deltaRstring("Chi2Dis.topHad","Chi2Dis.wLep"),"weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && "+chi2cut+")","70,0,10","X^{2} #Delta R(top_{had},W_{lep})");
  treehists.Draw(deltaRstring("Chi2Dis.topLep","Chi2Dis.wHad"),"weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && "+chi2cut+")","70,0,10","X^{2} #Delta R(top_{lep},W_{had}) ");


  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && "+chi2cut+")","70,0,3000","X^{2} B mass for top_{had} & W_{lep}");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && "+chi2cut+")","70,0,3000","X^{2} B mass for top_{lep} & W_{had}");

  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && "+chi2cut+")","70,0,1","X^{2} for top_{had} & W_{lep}");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && "+chi2cut+")","70,0,1","X^{2} for top_{lep} & W_{had}");

  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1 &&"+chi2cut+")","70,0,120","X^{2} W_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && "+chi2cut+")","70,100,3000","B mass [GeV]");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && "+chi2cut+")","70,0,1","X^{2} top_{had} X_{dis}");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && "+chi2cut+")","70,0,1","X^{2} top_{lep} X_{dis}");
  treehists.Draw("Chi2Dis.forwardJet.eta()","weight*(TopTagDis.mass==-1 &&"+chi2cut+")","40,-5,5","forward ak4 #eta");


  treehists.Draw(deltaRstring("Chi2Dis.bprime","BprimeGen.bprime")+"<0.4","weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 )","5,-.5,4.5","matched VLQ #Delta R(B_{reco},B_{gen})<0.4");
  treehists.Draw(deltaRstring("Chi2Dis.bprime","BprimeGen.bprime"),"weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 )","50,0,10","#Delta R(B_{reco},B_{gen})");
  treehists.Draw("fabs(fabs(Chi2Dis.bprime.M()-BprimeGen.bprime)/BprimeGen.bprime.M()-1) <0.15","weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 )","5,-.5,4.5","matched VLQ |B_{reco}-B_{gen}|/B_{gen} < 15%");
  treehists.Draw("fabs(Chi2Dis.bprime.M()-BprimeGen.bprime)/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 )","50,0,5","|B_{reco}-B_{gen}|/B_{gen}");


  
  treehists.Draw(deltaRstring("Chi2Dis.bprime","BprimeGen.bprime")+"<0.8","weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 && Chi2Dis.btagEventNumber>1)","5,-.5,4.5","matched VLQ #Delta R(B_{reco},B_{gen})<0.8");
  treehists.Draw(deltaRstring("Chi2Dis.bprime","BprimeGen.bprime"),"weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 && Chi2Dis.btagEventNumber>1)","50,0,10","#Delta R(B_{reco},B_{gen})");
  treehists.Draw("fabs(fabs(Chi2Dis.bprime.M()-BprimeGen.bprime)/BprimeGen.bprime.M()-1) <0.15","weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 && Chi2Dis.btagEventNumber>1)","5,-.5,4.5","matched VLQ |B_{reco}-B_{gen}|/B_{gen} < 15%");
  treehists.Draw("fabs(Chi2Dis.bprime.M()-BprimeGen.bprime)/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 &&"+chi2cut+" && BprimeGen.bprime.M()>400 && Chi2Dis.btagEventNumber>1)","50,0,5","|B_{reco}-B_{gen}|/B_{gen}");


  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 || TopTagDis.wHad.pt()<400)","70,-9,3000","B mass [GeV]");
  return 0;

  
  //X2 reco masses
  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && Chi2Dis.num_whad == 2 &&"+chi2cut+")","70,0,300","X^{2} top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && Chi2Dis.num_whad == 2 &&"+chi2cut+")","70,0,300","X^{2} top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.num_whad == 2 &&"+chi2cut+")","70,0,120","X^{2} W_{had} mass [GeV]","Events",false);
  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.num_whad == 2 &&"+chi2cut+")","70,0,120","X^{2} W_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.num_whad == 2 &&"+chi2cut+")","70, 300,3000","X^{2} B mass [GeV]");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && Chi2Dis.num_whad == 2 &&"+chi2cut+")","70,0,20","X^{2} top_{had} X_{dis}");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && Chi2Dis.num_whad == 2 &&"+chi2cut+")","70,0,20","X^{2} top_{lep} X_{dis}");
  
  //X2 reco masses
  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && Chi2Dis.num_top == 0 &&"+chi2cut+")","70,0,300","X^{2} top_{had} mass [GeV]");
  //treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && Chi2Dis.num_top == 0)","50,120,300","X^{2} top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.num_top == 0 &&"+chi2cut+")","70,0,120","X^{2} W_{had} mass [GeV]","Events",false);
  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.num_top == 0 &&"+chi2cut+")","70,0,120","X^{2} W_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.num_top == 0 &&"+chi2cut+")","70,300,3000","X^{2} B mass [GeV]");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && Chi2Dis.num_top == 0 &&"+chi2cut+")","70,0,20","X^{2} top_{had} X_{dis}");
 
  //X2 reco masses
  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && Chi2Dis.num_top == 1 &&"+chi2cut+")","70,0,300","X^{2} top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && Chi2Dis.num_top == 1 &&"+chi2cut+")","70,0,300","X^{2} top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.num_top == 1 &&"+chi2cut+")","70,0,120","X^{2} W_{had} mass [GeV]","Events",false);
  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.num_top == 1 &&"+chi2cut+")","70,0,120","X^{2} W_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.num_top == 1 &&"+chi2cut+")","70,300,3000","B mass [GeV]");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && Chi2Dis.num_top == 1 &&"+chi2cut+")","70,0,20","X^{2} top_{had} X_{dis}");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && Chi2Dis.num_top == 1 &&"+chi2cut+")","70,0,20","X^{2} top_{lep} X_{dis}");
 
  //WTag reco masses
  treehists.Draw("WTagDis.topHad.M()>0","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62 )","5,-.5,4.5","W-tag top_{had} number of Events");
  treehists.Draw("WTagDis.topLep.M()>0","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==61 )","5,-.5,4.5","W-tag top_{lep} number of Events");
  treehists.Draw("WTagDis.wHad.M()>0","weight*(TopTagDis.mass==-1 )","5,-.5,4.5","W-tag W_{had} number of Events","Events",false);

  treehists.Draw("WTagDis.topHad.M()","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62 )","70,90,300","W-tag top_{had} mass [GeV]");
  treehists.Draw("WTagDis.topLep.M()","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==61 )","70,90,300","W-tag top_{lep} mass [GeV]");
  treehists.Draw("WTagDis.wHad.M()","weight*(TopTagDis.mass==-1 )","70,60,300","W-tag W_{had} mass [GeV]","Events",false);
  treehists.Draw("WTagDis.wLep.M()","weight*(TopTagDis.mass==-1 )","70,60,300","W-tag W_{lep} mass [GeV]");
  treehists.Draw("WTagDis.mass","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62)","70,300,3000","top_{had} W-tag B mass [GeV]");
  treehists.Draw("WTagDis.mass","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==61)","70,300,3000","top_{lep} W-tag B mass [GeV]");
  treehists.Draw("WTagDis.mass","weight*(TopTagDis.mass==-1 )","70,300,3000","W-tag B mass [GeV]");

  treehists.Draw("WTagDis.chi","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==62 )","70,0,20","W-tag top_{had} X^{2}_{dis}");
  treehists.Draw("WTagDis.chi","weight*(TopTagDis.mass==-1 && WTagDis.recoTyp==61 )","70,0,20","W-tag top_{lep} X^{2}_{dis}");
  

  //X2 with WTag
  treehists.Draw("Chi2Dis.topHad.M()>0","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && WTagDis.mass>0)","5,-0.5,4.5","X^{2} with W-tag top_{had} number of Events");
  treehists.Draw("Chi2Dis.topLep.M()>0","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && WTagDis.mass>0)","5,-.5,4.5","X^{2} with W-tag top_{lep} number of Events");
  treehists.Draw("Chi2Dis.wHad.M()>0","weight*(TopTagDis.mass==-1 && WTagDis.mass>0)","5,-0.5,4.5","X^{2} with W-tag W_{had} number of Events","Events",false);

  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && WTagDis.mass>0)","70,0,300","X^{2} with W-tag top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && WTagDis.mass>0)","70,0,300","X^{2} with W-tag top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0)","70,0,120","X^{2} with W-tag W_{had} mass [GeV]","Events",false);
  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0)","70,0,120","X^{2} with W-tag W_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && Chi2Dis.recoTyp==12)","70,300,3000","X^2 with W-tag top_{had} B mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && Chi2Dis.recoTyp==11)","70,300,3000","X^2 with W-tag top_{lep} B mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0)","70,300,3000","X^2 with W-tag B mass [GeV]");

  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12 && WTagDis.mass>0)","70,0,20","X^{2} with W-tag top_{had} X_{dis}");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11 && WTagDis.mass>0)","70,0,20","X^{2} with W-tag top_{lep} X_{dis}");


  
  //Mass resolution
  treehists.removeFile(0);
  treehists.switch_ratio(false);

  treehists.Draw("(Chi2Dis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.bprime.M() >400)","70,-2,2","X^2 B mass [GeV]");
  treehists.Draw("(WTagDis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.bprime.M() >400)","70,-2,2","W-tag B mass [GeV]");

  treehists.Draw("(Chi2Dis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.bprime.M() >400 && Chi2Dis.recoTyp==12)","70,-2,2","top_{had} X^2 B mass [GeV]");
  treehists.Draw("(WTagDis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.bprime.M() >400 && WTagDis.recoTyp==62)","70,-2,2","top_{had} W-tag B mass [GeV]");
  treehists.Draw("(Chi2Dis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.bprime.M() >400 && Chi2Dis.recoTyp==11)","70,-2,2","top_{lep} X^2 B mass [GeV]");
  treehists.Draw("(WTagDis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.bprime.M() >400 && WTagDis.recoTyp==61)","70,-2,2","top_{lep} W-tag B mass [GeV]");

  //Distance to gen particles
  treehists.Draw(deltaRstring("Chi2Dis.topHad","BprimeGen.topHad"),"weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topHad.pt() >0 && Chi2Dis.recoTyp==12 )","70,0,5","X^2 top_{had} #Delta R(gen,reco)");
  treehists.Draw(deltaRstring("WTagDis.topHad","BprimeGen.topHad"),"weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topHad.pt() >0 && WTagDis.recoTyp==62)","70,0,5","W-tag top_{had} #Delta R(gen,reco)");
  treehists.Draw(deltaRstring("Chi2Dis.wHad","BprimeGen.wHad"),"weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.wHad.pt() >0 && Chi2Dis.recoTyp==11 )","70,0,5","X^2 W_{had} #Delta R(gen,reco)");
  treehists.Draw(deltaRstring("WTagDis.wHad","BprimeGen.wHad"),"weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.wHad.pt() >0 && WTagDis.recoTyp==61)","70,0,5","W-tag W_{had} #Delta R(gen,reco)");
  treehists.Draw(deltaRstring("Chi2Dis.topLep","BprimeGen.topLep"),"weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topLep.pt() >0 && Chi2Dis.recoTyp==11 )","70,0,5","X^2 top_{lep} #Delta R(gen,reco)");
  treehists.Draw(deltaRstring("WTagDis.topLep","BprimeGen.topLep"),"weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topLep.pt() >0 && WTagDis.recoTyp==61)","70,0,5","W-tag top_{lep} #Delta R(gen,reco)");


  treehists.Draw(deltaRstring("Chi2Dis.topHad","BprimeGen.topHad")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topHad.pt() >0 && Chi2Dis.recoTyp==12 )","6,-.5,5.5","X^2 top_{had} matched");
  treehists.Draw(deltaRstring("WTagDis.topHad","BprimeGen.topHad")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topHad.pt() >0 && WTagDis.recoTyp==62)","6,-0.5,5.5","W-tag top_{had} matched");
  treehists.Draw(deltaRstring("Chi2Dis.wHad","BprimeGen.wHad")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.wHad.pt() >0 && Chi2Dis.recoTyp==11 )","6,-0.5,5.5","X^2 W_{had} matched");
  treehists.Draw(deltaRstring("WTagDis.wHad","BprimeGen.wHad")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.wHad.pt() >0 && WTagDis.recoTyp==61)","6,-0.5,5.5","W-tag W_{had} matched");
  treehists.Draw(deltaRstring("Chi2Dis.topLep","BprimeGen.topLep")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topLep.pt() >0 && Chi2Dis.recoTyp==11)","6,-0.5,5.5","X^2 top_{lep} matched");
  treehists.Draw(deltaRstring("WTagDis.topLep","BprimeGen.topLep")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topLep.pt() >0 && WTagDis.recoTyp==61)","6,-0.5,5.5","W-tag top_{lep} matched");
   

  //only the cases where the right decay mode is taken
  treehists.Draw(deltaRstring("Chi2Dis.topHad","BprimeGen.topHad")+"<0.4 && "+deltaRstring("Chi2Dis.wLep","BprimeGen.wLep")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topHad.pt() >0 && Chi2Dis.recoTyp==12 )","6,-.5,5.5","X^2 top_{had} W_{lep} matched right decay mode");
  treehists.Draw(deltaRstring("WTagDis.topHad","BprimeGen.topHad")+"<0.4 &&"+deltaRstring("WTagDis.wLep","BprimeGen.wLep")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.topHad.pt() >0 && WTagDis.recoTyp==62)","6,-0.5,5.5","W-tag top_{had} W_{lep} matched right decay mode");
  treehists.Draw(deltaRstring("Chi2Dis.topLep","BprimeGen.topLep")+"<0.4 && "+deltaRstring("Chi2Dis.wHad","BprimeGen.wHad")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.wHad.pt() >0 && Chi2Dis.recoTyp==11 )","6,-0.5,5.5","X^2 top_{lep} W_{had} matched right decay mode");
  treehists.Draw(deltaRstring("WTagDis.topLep","BprimeGen.topLep")+"<0.4 && "+deltaRstring("WTagDis.wHad","BprimeGen.wHad")+"<0.4","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && BprimeGen.wHad.pt() >0 && WTagDis.recoTyp==61)","6,-0.5,5.5","W-tag top_{lep} W_{had} matched right decay mode");


  //all 
  treehists.Draw(deltaRstring("Chi2Dis.topHad","BprimeGen.topHad")+"<0.4 && "+deltaRstring("Chi2Dis.wLep","BprimeGen.wLep")+"<0.4 && BprimeGen.topHad.pt()>0","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && Chi2Dis.recoTyp==12 )","6,-.5,5.5","X^2 top_{had} W_{lep} matched");
  treehists.Draw(deltaRstring("WTagDis.topHad","BprimeGen.topHad")+"<0.4 &&"+deltaRstring("WTagDis.wLep","BprimeGen.wLep")+"<0.4 && BprimeGen.topHad.pt()>0","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && WTagDis.recoTyp==62)","6,-0.5,5.5","W-tag top_{had} W_{lep} matched");

  treehists.Draw(deltaRstring("Chi2Dis.topLep","BprimeGen.topLep")+"<0.4 && "+deltaRstring("Chi2Dis.wHad","BprimeGen.wHad")+"<0.4 && BprimeGen.topLep.pt()>0","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && Chi2Dis.recoTyp==11 )","6,-0.5,5.5","X^2 top_{lep} W_{had} matched");
  treehists.Draw(deltaRstring("WTagDis.topLep","BprimeGen.topLep")+"<0.4 && "+deltaRstring("WTagDis.wHad","BprimeGen.wHad")+"<0.4 && BprimeGen.topLep.pt()>0","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && WTagDis.recoTyp==61)","6,-0.5,5.5","W-tag top_{lep} W_{had} matched");
  
  
  return 0;
}
