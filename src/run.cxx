//main routine call everything else from here.
//so i do not have to change my makefile and everything compiles

#include "slicePlots.h"
#include "simplePlots.h"

#include <iostream>


using namespace std;

int main(){

  
  slicePlots newTuneNoNucs("plots/newTuneNoNucs.ps");
  newTuneNoNucs.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fast_val_hists.root");
  newTuneNoNucs.sliceHists("ak4PFJet_genJet_energy",15);
  newTuneNoNucs.plotHists(1);

  slicePlots oldFast_Full("plots/Comp_oldFast_Full.ps");
  oldFast_Full.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOld_val_hists.root");
  oldFast_Full.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/full_val_hists.root");
  oldFast_Full.setupRMSAxis(0.6,0.);
  oldFast_Full.setupMeanAxis(1.1,0.5);
  oldFast_Full.sliceHists("ak4PFJet_genJet_energy",15);
  oldFast_Full.plotHists(1);

  slicePlots newTuneNoNucs_Full("plots/Comp_newTuneNoNucs_Full.ps");
  newTuneNoNucs_Full.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fast_val_hists.root");
  newTuneNoNucs_Full.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/full_val_hists.root");
  newTuneNoNucs_Full.setupRMSAxis(0.6,0.);
  newTuneNoNucs_Full.setupMeanAxis(1.1,0.5);
  newTuneNoNucs_Full.sliceHists("ak4PFJet_genJet_energy",15);
  newTuneNoNucs_Full.plotHists(1);

  slicePlots newTuneNoNucs_oldFast("plots/Comp_newTuneNoNucs_oldFast.ps");
  newTuneNoNucs_oldFast.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fast_val_hists.root");
  newTuneNoNucs_oldFast.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOld_val_hists.root");
  newTuneNoNucs_oldFast.setupRMSAxis(0.6,0.);
  newTuneNoNucs_oldFast.setupMeanAxis(1.1,0.5);
  newTuneNoNucs_oldFast.sliceHists("ak4PFJet_genJet_energy",15);
  newTuneNoNucs_oldFast.plotHists(1);

  
  slicePlots low_energies_Full_Nucs_noNucs("plots/Comp_Full_Nucs_noNucs_0_200.ps");
  low_energies_Full_Nucs_noNucs.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_full_val_hists.root");
  low_energies_Full_Nucs_noNucs.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_fastOldTune_val_hists.root");
  low_energies_Full_Nucs_noNucs.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_fastOld_val_hists.root");
  low_energies_Full_Nucs_noNucs.setupRMSAxis(0.6,0.);
  low_energies_Full_Nucs_noNucs.setupMeanAxis(1.3,0.5);
  low_energies_Full_Nucs_noNucs.setBinTitle("ak4genJet Energy");
  low_energies_Full_Nucs_noNucs.sliceHists("ak4PFJet_genJet_energy",20);
  low_energies_Full_Nucs_noNucs.plotHists(1);



  slicePlots Full_Nucs_noNucs("plots/Comp_Full_Nucs_noNucs.ps");
  Full_Nucs_noNucs.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/full_val_hists.root");
  Full_Nucs_noNucs.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOldTune_val_hists.root");
  Full_Nucs_noNucs.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOld_val_hists.root");
  Full_Nucs_noNucs.setupRMSAxis(0.6,0.);
  Full_Nucs_noNucs.setupMeanAxis(1.1,0.5);
  Full_Nucs_noNucs.setSliceXRange(0,2.5);
  Full_Nucs_noNucs.setBinTitle("ak4genJet Energy");
  Full_Nucs_noNucs.sliceHists("ak4PFJet_genJet_energy",20);
  Full_Nucs_noNucs.setSlicesLogy();
  Full_Nucs_noNucs.plotHists(1);


  slicePlots Full_Nucs_noNucs_barrel("plots/Comp_Full_Nucs_noNucs_barrel.ps");
  Full_Nucs_noNucs_barrel.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/full_val_hists_eta_0_1.4.root");
  Full_Nucs_noNucs_barrel.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOldTune_val_hists_eta_0_1.4.root");
  Full_Nucs_noNucs_barrel.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOld_val_hists_eta_0_1.4.root");
  Full_Nucs_noNucs_barrel.setupRMSAxis(0.6,0.);
  Full_Nucs_noNucs_barrel.setupMeanAxis(1.1,0.5);
  Full_Nucs_noNucs_barrel.setSlicesLogy();
  Full_Nucs_noNucs_barrel.setBinTitle("ak4genJet Energy");
  Full_Nucs_noNucs_barrel.sliceHists("ak4PFJet_genJet_energy",10);
  Full_Nucs_noNucs_barrel.plotHists(1);
  Full_Nucs_noNucs_barrel.clearHists();
  // Full_Nucs_noNucs_barrel.setBinTitle("ak4genJet pT");
  //Full_Nucs_noNucs_barrel.sliceHists("ak4PFJet_genJet_pT",10);
  //Full_Nucs_noNucs_barrel.plotHists(1);



  slicePlots Full_Nucs_noNucs_endcaps("plots/Comp_Full_Nucs_noNucs_endcaps.ps");
  Full_Nucs_noNucs_endcaps.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/full_val_hists_eta_1.4_2.5.root");
  Full_Nucs_noNucs_endcaps.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOldTune_val_hists_eta_1.4_2.5.root");
  Full_Nucs_noNucs_endcaps.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/fastOld_val_hists_eta_1.4_2.5.root");
  Full_Nucs_noNucs_endcaps.setupRMSAxis(0.6,0.);
  Full_Nucs_noNucs_endcaps.setupMeanAxis(1.1,0.5);
  Full_Nucs_noNucs_endcaps.setBinTitle("ak4genJet Energy");
  Full_Nucs_noNucs_endcaps.sliceHists("ak4PFJet_genJet_energy",10);
  Full_Nucs_noNucs_endcaps.setSlicesLogy();
  Full_Nucs_noNucs_endcaps.plotHists(1);


  slicePlots Full_Nucs_noNucs_barrel_low("plots/Comp_Full_Nucs_noNucs_barrel_low.ps");
  Full_Nucs_noNucs_barrel_low.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_full_val_hists_eta_0_1.4.root");
  Full_Nucs_noNucs_barrel_low.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_fastOldTune_val_hists_eta_0_1.4.root");
  Full_Nucs_noNucs_barrel_low.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_fastOld_val_hists_eta_0_1.4.root");
  Full_Nucs_noNucs_barrel_low.setupRMSAxis(0.6,0.);
  Full_Nucs_noNucs_barrel_low.setupMeanAxis(1.1,0.5);
  Full_Nucs_noNucs_barrel_low.setBinTitle("ak4genJet Energy");
  Full_Nucs_noNucs_barrel_low.sliceHists("ak4PFJet_genJet_energy",10);
  Full_Nucs_noNucs_barrel_low.setSlicesLogy();
  //Full_Nucs_noNucs_barrel_low.setSliceXRange(0,3);
  Full_Nucs_noNucs_barrel_low.plotHists(1);


  slicePlots Full_Nucs_noNucs_endcaps_low("plots/Comp_Full_Nucs_noNucs_endcaps_low.ps");
  Full_Nucs_noNucs_endcaps_low.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_full_val_hists_eta_1.4_2.5.root");
  Full_Nucs_noNucs_endcaps_low.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_fastOldTune_val_hists_eta_1.4_2.5.root");
  Full_Nucs_noNucs_endcaps_low.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/0_200_fastOld_val_hists_eta_1.4_2.5.root");
  Full_Nucs_noNucs_endcaps_low.setupRMSAxis(0.6,0.);
  Full_Nucs_noNucs_endcaps_low.setupMeanAxis(1.5,0.5);
  Full_Nucs_noNucs_endcaps_low.setBinTitle("ak4genJet Energy");
  Full_Nucs_noNucs_endcaps_low.sliceHists("ak4PFJet_genJet_energy",10);
  Full_Nucs_noNucs_endcaps_low.setSlicesLogy();
  Full_Nucs_noNucs_endcaps_low.plotHists(1);

  vector<string> range; range.push_back(""); 

  for(auto prefix : range){
    
    simplePlots fractions("plots/"+prefix+"comp_fraction.ps");
    fractions.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/"+prefix+"full_val_hists.root");
    fractions.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/"+prefix+"fastOldTune_val_hists.root");
    fractions.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_0_pre1/src/val/RECLevelVal/python/"+prefix+"fastOld_val_hists.root");
    
    //fractions.addLegendEntry("FullSim");
    //fractions.addLegendEntry("FastSim No Nucs");
    //fractions.addLegendEntry("FastSim vanilla");
    //fractions.setLegend(0.3,.6,.6,.8);

    fractions.loadHists("low Energies/ak4PFJets_neutralHadronFraction_low");
    fractions.plotHists(1,1);
    fractions.clearHists();
    fractions.loadHists("middle Energies/ak4PFJets_neutralHadronFraction_middle");
    fractions.plotHists(1,1);
    fractions.clearHists();
    fractions.loadHists("high Energies/ak4PFJets_neutralHadronFraction_high");
    fractions.plotHists(1,1);
    fractions.clearHists();
    
    fractions.loadHists("low Energies/ak4PFJets_chargedHadronFraction_low");
    fractions.plotHists(1,1);
    fractions.clearHists();
    fractions.loadHists("middle Energies/ak4PFJets_chargedHadronFraction_middle");
    fractions.plotHists(1,1);
    fractions.clearHists();
    fractions.loadHists("high Energies/ak4PFJets_chargedHadronFraction_high");
    fractions.plotHists(1,1);
    fractions.clearHists();
    
    fractions.loadHists("low Energies/ak4PFJets_emFraction_low");
    fractions.plotHists(1,1);
    fractions.clearHists();
    fractions.loadHists("middle Energies/ak4PFJets_emFraction_middle");
    fractions.plotHists(1,1);
    fractions.clearHists();
    fractions.loadHists("high Energies/ak4PFJets_emFraction_high");
    fractions.plotHists(1,1);
    fractions.clearHists();
    
  }

 
  vector<string> energies; energies.push_back("1");energies.push_back("5");energies.push_back("20");energies.push_back("100");energies.push_back("1000");
  vector<string> eta_bins; eta_bins.push_back("0.1_0.2");eta_bins.push_back("1.4_1.5");eta_bins.push_back("2.8_2.9");

  //cout<<eta_bins[0]+"_"+energies[0]<<endl;

  
  for(auto energy : energies){
    for(auto eta_bin : eta_bins){

      simplePlots newcompPlots("plots/full_fastsim_energy_"+energy+"_eta_"+eta_bin+".ps");
      newcompPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/Substructure/SubstrucAnalyzer/python/full_timePlots/newPlotsTimeEMenergy_response_pdgId_211_eta_"+eta_bin+"_E_"+energy+".root");
      newcompPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/Substructure/SubstrucAnalyzer/python/fast_timePlots/newPlotsTimeEMenergy_response_pdgId_211_eta_"+eta_bin+"_E_"+energy+".root");
      //newcompPlots.addLegendEntry("FullSim");
      //newcompPlots.addLegendEntry("FastSim");

      newcompPlots.loadHists("calo_time_eweight");
      newcompPlots.plotHists(1,1);
      newcompPlots.clearHists();
      newcompPlots.loadHists("hcal_time_eweight");
      newcompPlots.plotHists(1,1);
      newcompPlots.clearHists();
      newcompPlots.loadHists("ecal_time_eweight");
      newcompPlots.plotHists(1,1);
      newcompPlots.clearHists();
      newcompPlots.loadHists("calo_time");
      newcompPlots.plotHists(1,1);
      newcompPlots.clearHists();
      newcompPlots.loadHists("hcal_time");
      newcompPlots.plotHists(1,1);
      newcompPlots.clearHists();
      newcompPlots.loadHists("ecal_time");
      newcompPlots.plotHists(1,1);
      newcompPlots.clearHists();
      newcompPlots.loadHists("ratioEcalHcal_energy");
      newcompPlots.plotHists(1,1);
      newcompPlots.clearHists();
      newcompPlots.loadHists("ratioEMTotal_energy");
      newcompPlots.plotHists(1,1);

    }
  }
 

  
 

  cout<<"hello world, I'm done"<<endl;
  return 0;
}


/**
 * PCaloHit::energyEM() / PCaloHit::energy()
 * - PCaloHit::time()
 *- PCaloHit::depth()
 *- ratio of (sum_ECAL simhit.energy() / sum_HCAL simhit.energy())
 */
    
