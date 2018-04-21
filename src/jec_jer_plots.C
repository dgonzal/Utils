#include <iostream>

#include "TreeHists.h"
#include "TChainHists.h"
#include "simplePlots.h"
#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"


int main(int argc, char** argv){
  string resultfile = "plots/jec_jer/";//"moneyplots.pdf";
  bool single = true; 


  TreeHists treehists(resultfile,single);
  //if(single) treehists.switch_singleplots(true);
  //treehists.set_ignorePages(15);
  //
  string sample = "BprimeB-1100_LH*";
  string nick = "B+b, M_{B} (1.1 TeV), LH";
  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/";   
  string nominel = dir+"EleSigSel_v1";
  string jer_jec_up = dir+"sig_jec_jer_up_MuSigSelUNC/workdir.sig_jec_jer_up_MuSigSelUNC";
  string jer_jec_down = dir+"sig_jec_jer_down_MuSigSelUNC/workdir.sig_jec_jer_down_MuSigSelUNC";
  string jer_up_jec_down = dir+"jec_jer_up_MuSigSelUNC/workdir.jec_jer_up_MuSigSelUNC";
  string jer_down_jec_up = dir+"jec_jer_down_up_MuSigSelUNC/workdir.jec_jer_down_up_MuSigSelUNC";
  string jer_up = dir+"sig_jer_up_MuSigSelUNC/workdir.sig_jer_up_MuSigSelUNC";
  string jer_down = dir+"sig_jer_up_MuSigSelUNC/workdir.sig_jer_down_MuSigSelUNC";
  string jec_up = dir+"sig_jec_up_MuSigSelUNC/workdir.sig_jec_up_MuSigSelUNC";
  string jec_down = dir+"sig_jec_down_MuSigSelUNC/workdir.sig_jec_down_MuSigSelUNC";
  string jec_2up = dir+"sig_jec_2up_MuSigSelUNC/workdir.sig_jec_2up_MuSigSelUNC";
  string jec_2down = dir+"sig_jec_2down_MuSigSelUNC/workdir.sig_jec_2down_MuSigSelUNC";
  string jec_3up = dir+"sig_jec_3up_MuSigSelUNC/workdir.sig_jec_3up_MuSigSelUNC";
  string jec_3down = dir+"sig_jec_3down_MuSigSelUNC/workdir.sig_jec_3down_MuSigSelUNC";
  string jec_4up = dir+"sig_jec_4up_MuSigSelUNC/workdir.sig_jec_4up_MuSigSelUNC";
  string jec_4down = dir+"sig_jec_4down_MuSigSelUNC/workdir.sig_jec_4down_MuSigSelUNC";
  string jer_2jec_up = dir+"sig_2jec_jer_up_MuSigSelUNC/workdir.sig_2jec_jer_up_MuSigSelUNC";
  string jer_2jec_down = dir+"sig_2jec_jer_down_MuSigSelUNC/workdir.sig_2jec_jer_down_MuSigSelUNC";

  treehists.addFile(nominel+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root","hist",2,-1,false,"B+b, M_{B} (0.8 TeV), LH");
  treehists.addFile(nominel+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1100_LH.root","hist",3,-1,false,"B+b, M_{B} (1.1 TeV), LH");
  treehists.addFile(nominel+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1700_LH.root","hist",4,-1,false,"B+b, M_{B} (1.7 TeV), LH");
  

  
  //treehists.addFile(jer_up_jec_down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",6,-1,false,"JER JEC (1,-1)");
  //treehists.addFile(jer_down_jec_up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",7,-1,false,"JER JEC (-1,1)");
  //treehists.addFile(jer_up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",8,-1,false,"JER JEC (1,0)");
  //treehists.addFile(jer_down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",9,-1,false,"JER JEC (-1,0)");
  /*/
  treehists.addFile(jec_up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",3,-1,false,"JER JEC (0,1)", 0, 1., make_pair("slimmedJets","jet_jec_up"));
  treehists.addFile(jec_2up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",4,-1,false,"JER JEC (0,2)", 0, 1., make_pair("slimmedJets","jet_jec_2up"));
  treehists.addFile(jec_3up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",5,-1,false,"JER JEC (0,3)", 0, 1., make_pair("slimmedJets","jet_jec_3up"));
  treehists.addFile(jec_4up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",6,-1,false,"JER JEC (0,4)", 0, 1., make_pair("slimmedJets","jet_jec_4up"));
  
  treehists.addFile(jec_down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root" ,"hist",3,-1,false,"JER JEC (0,-1)", 0, 1., make_pair("slimmedJets","jet_jec_down"));
  treehists.addFile(jec_2down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",4,-1,false,"JER JEC (0,-2)", 0, 1., make_pair("slimmedJets","jet_jec_2down"));
  //treehists.addFile(jec_3down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",5,-1,false,"JER JEC (0,-3)", 0, 1., make_pair("slimmedJets","jet_jec_3down"));
  //treehists.addFile(jec_4down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",6,-1,false,"JER JEC (0,-4)", 0, 1., make_pair("slimmedJets","jet_jec_4down"));
  /*/
  //treehists.addFile(jer_jec_up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",4,-1,false,"JER JEC (1,1)");
  //treehists.addFile(jer_jec_down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",5,-1,false,"JER JEC (-1,-1)");
  
  //treehists.addFile(jer_2jec_up+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",6,-1,false,"JER JEC (1,2)");
  //treehists.addFile(jer_2jec_down+"/uhh2.AnalysisModuleRunner.MC."+sample+".root","hist",7,-1,false,"JER JEC (-1,-2)");
  
  
  treehists.set_ratiolimits(1.79,0.39);
  treehists.mcratio_only(true);
  treehists.SetTree("AnalysisTree");
  treehists.switch_ratio(false);
  //treehists.draw_single_ratios(true);
  //treehists.set_ratioTitle("Ratio");
  treehists.SetLegend(0.4, 0.5, 0.8, 0.86);

  TLatex *cms_text = new TLatex(0.9, 0.95,"e channel 35.9 fb^{-1} (13 TeV)");
  cms_text->SetNDC();
  cms_text->SetTextAlign(33);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.04);
  TLatex *preliminary_text = new TLatex(0.1, 0.95,"CMS #it{Preliminary}");
  preliminary_text->SetNDC();
  preliminary_text->SetTextAlign(13);
  preliminary_text->SetTextFont(42);
  preliminary_text->SetTextSize(0.045); 
  TLatex *chi2_text = new TLatex(0.8, 0.3,"");
  TLatex *topmatch_text = new TLatex(0.8, 0.3,"");
  TLatex *wmatch_text = new TLatex(0.8, 0.3,"");
    
  string eta = "2.4";
  string chi2_central_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) >="+eta;

  string factors = "";//"((Chi2Dis.num_top+Chi2Dis.num_whad)==1)*";

  string matching_radius = "0.4";

  
  //string wtag_scalefactor = "";
  for(int i = 1; i<3; i++){
    factors = "((Chi2Dis.num_top+Chi2Dis.num_whad)=="+to_string(i)+")*";
    if(i==0)
      factors ="";
    else{
      chi2_text = new TLatex(0.8, 0.3,(to_string(i)+" ak4 jets ").c_str());
      chi2_text->SetNDC();
      chi2_text->SetTextAlign(33);
      chi2_text->SetTextFont(42);
      chi2_text->SetTextSize(0.05);

      topmatch_text = new TLatex(0.85, 0.3,"matched ak4 tops");
      topmatch_text->SetNDC();
      topmatch_text->SetTextAlign(33);
      topmatch_text->SetTextFont(42);
      topmatch_text->SetTextSize(0.05);

      wmatch_text = new TLatex(0.8, 0.3,"matched ak4 W");
      wmatch_text->SetNDC();
      wmatch_text->SetTextAlign(33);
      wmatch_text->SetTextFont(42);
      wmatch_text->SetTextSize(0.05);
    }
    treehists.clearText();
    treehists.addText(cms_text);
    treehists.addText(preliminary_text);
    treehists.addText(chi2_text);

    /*/
    treehists.Draw("slimmedJets.slimmedJets.m_pt",factors+"weight*("+chi2_forward_string+")","40,30,600","ak4 p_{T} [GeV]");
    treehists.Draw("Chi2Dis.wHad.pt()",factors+"weight*("+chi2_forward_string+")","40,0,1600","W p_{T} [GeV]");
    treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+")","40,0,3500","m_{reco} [GeV]");
    treehists.Draw("Chi2Dis.bprime.pt()",factors+"weight*("+chi2_forward_string+")","40,0,1800","p_{T,reco} [GeV]");
    treehists.Draw("BprimeGen.bprime.pt()",factors+"weight*("+chi2_forward_string+")","40,0,1800","p_{T,gen} [GeV]");
    treehists.Draw("Chi2Dis.bprime.pt()/BprimeGen.bprime.pt()",factors+"weight*("+chi2_forward_string+")","40,-0.5,8","p_{T,reco}/p_{T,gen} [GeV]");

    treehists.Draw("1*("+deltaRstring("BprimeGen.topHad","Chi2Dis.topHad")+"<"+matching_radius+" && BprimeGen.topHad.pt() > 0 && Chi2Dis.topHad.pt()>0) || ("+deltaRstring("BprimeGen.wHad","Chi2Dis.wHad")+"<"+matching_radius+" && BprimeGen.topLep.pt() > 0 && Chi2Dis.topLep.pt()>0)",factors+"weight","4,-0.5,3.5","matched");
    /*/
    // matched top
    treehists.clearText();
    treehists.addText(cms_text);
    treehists.addText(preliminary_text);
    treehists.addText(topmatch_text);
    if(i==1)treehists.Draw("("+deltaRstring("BprimeGen.topHad","Chi2Dis.topHad")+"<0.4 && BprimeGen.topHad.pt() > 0 && Chi2Dis.topHad.pt()>0 && "+deltaRstring("BprimeGen.wHad","Chi2Dis.wHad")+"<"+matching_radius+" && "+deltaRstring("BprimeGen.bquark[0]","Chi2Dis.topHad")+"<0.4 && "+deltaRstring("BprimeGen.whad_daughters[0]","Chi2Dis.topHad")+"<0.4 && "+deltaRstring("BprimeGen.whad_daughters[1]","Chi2Dis.topHad")+"<0.4)",factors+"weight","4,-0.5,3.5","[bool]");
    //treehists.Draw("Chi2Dis.mass",factors+"weight*("+deltaRstring("BprimeGen.topHad","Chi2Dis.topHad")+"<"+matching_radius+" && BprimeGen.topHad.pt() > 0 && Chi2Dis.topHad.pt()>0 && "+deltaRstring("BprimeGen.wHad","Chi2Dis.wHad")+"<"+matching_radius+")","40,30,3500","m_{reco} matched [GeV]");
    //treehists.Draw("Chi2Dis.bprime.pt()/BprimeGen.bprime.pt()",factors+"weight*("+deltaRstring("BprimeGen.topHad","Chi2Dis.topHad")+"<"+matching_radius+" && BprimeGen.topHad.pt() > 0 && Chi2Dis.topHad.pt()>0 && "+deltaRstring("BprimeGen.wHad","Chi2Dis.wHad")+"<"+matching_radius+")","40,30,3500","m_{reco} matched [GeV]");

    
    // matched W
    treehists.clearText();
    treehists.addText(cms_text);
    treehists.addText(preliminary_text);
    treehists.addText(wmatch_text);
    if(i==2)treehists.Draw("("+deltaRstring("BprimeGen.wHad","Chi2Dis.wHad")+"<"+matching_radius+" && BprimeGen.topLep.pt() > 0 && Chi2Dis.topLep.pt()>0 && "+deltaRstring("BprimeGen.whad_daughters[0]","Chi2Dis.topHad")+"<0.4 && "+deltaRstring("BprimeGen.whad_daughters[1]","Chi2Dis.topHad")+"<0.4)",factors+"weight","4,-0.5,3.5","[bool]");
    //treehists.Draw("Chi2Dis.mass",factors+"weight*("+deltaRstring("BprimeGen.wHad","Chi2Dis.wHad")+"<"+matching_radius+" && BprimeGen.topLep.pt() > 0 && Chi2Dis.topLep.pt()>0)","40,30,3500","m_{reco} matched [GeV]");
    //treehists.Draw("Chi2Dis.bprime.pt()/BprimeGen.bprime.pt()",factors+"weight*("+deltaRstring("BprimeGen.wHad","Chi2Dis.wHad")+"<"+matching_radius+" && BprimeGen.topLep.pt() > 0 && Chi2Dis.topLep.pt()>0)","40,30,3500","m_{reco} matched [GeV]");
    
		 
  }
  //treehists.Draw("BprimeGen.bprime.M()","weight","100,1550,1650","mass [GeV]");
  
  return 0;
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","B mass [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) < 2.4 )","50,500,3000","B mass [GeV]","Events");
  treehists.Draw("TopTagDis.mass",factors+"weight*("+toptag_central_string+")","30,500,2000","B mass [GeV]");

  return 0;

}
