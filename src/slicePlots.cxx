#include "slicePlots.h"

using namespace std;




slicePlots::slicePlots(string saveName){
  resultFile=saveName;
  TH1::AddDirectory(kFALSE);
  infinity= numeric_limits<double>::infinity();
  resultMeanYmax=infinity; resultMeanYmin=infinity;
  resultRMSYmax=infinity; resultRMSYmin=infinity;
  binTitle="";
  sliceLogy = false;
  GraphLogy = false;GraphLogx = false;
  sliceXRangeMin=0; sliceXRangeMax=0;
  
  // general appearance and style
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->UseCurrentStyle();




}
slicePlots::~slicePlots(){
 
}



void slicePlots::sliceHists(string histname, unsigned int numSlices){

  int ibin;
  number_of_Slices = numSlices;

  for(auto fileDir : filedirs){
    TFile* file = new TFile(fileDir.c_str());
    TH2F * twoDhist = (TH2F*) file->Get(histname.c_str());

    ibin = twoDhist->GetYaxis()->GetNbins()/numSlices;
    
    HistoSlices tmpSlices;
    tmpSlices.b_denom =0;

    for(unsigned int i=1; i<numSlices+1; ++i){
      TH1D* tmpHist = twoDhist->ProjectionX("", ibin*(i-1)+1, ibin*(i));
      stringstream ss;
      ss<<binTitle<<": "<<setprecision(5)<<twoDhist->GetYaxis()->GetBinLowEdge(ibin*(i-1)+1)<<" - "<<twoDhist->GetYaxis()->GetBinUpEdge(ibin*(i))<<endl;
      tmpHist->SetTitle(ss.str().c_str());
      tmpSlices.slices.push_back(tmpHist);
      tmpSlices.sliceMean.push_back(twoDhist->GetYaxis()->GetBinCenter(ibin*(i))-twoDhist->GetYaxis()->GetBinCenter(ibin)*0.5);
    }    

    histos.push_back(tmpSlices);
    
    delete file;
  }
  /*
  if(denominator!=""){
    TFile* file = new TFile(denominator);
    TH2F * twoDhist = (TH2F*) file->Get(histname);

    breaks= twoDhist->GetYaxis()->GetXmax();
    ibin = twoDhist->GetYaxis()->GetXmax()/numSlices;

    HistoSlices tmpSlices;
    tmpSlices.b_denom =1;

    for(unsigned int i=0; i<numSlices; ++i){
      tmpSlices.slices.push_back(ProjectionY("", ibin*i, ibin*(i+1)));
      tmpSlices.sliceMean.push_back((twoDhist->GetBinCenter(ibin*(i+1))-twoDhist->GetBinCenter(ibin*i))*0.5)
    }

    histos.push_back(tmpSlices);
    delete file;
  }
  */
}


void slicePlots::plotHists(int options){
  
  TMultiGraph * resultMeanGraphs = new TMultiGraph();
  TMultiGraph * resultRMSGraphs = new TMultiGraph();


  for(unsigned int m = 0; m < histos.size(); ++m ){
    Double_t x[number_of_Slices], mean[number_of_Slices], sigma[number_of_Slices], meanError[number_of_Slices],rmsError[number_of_Slices];

    for(unsigned int i = 0; i<histos[m].slices.size(); ++i){
      TH1D* slice = histos[m].slices.at(i);
      x[i] = histos[m].sliceMean[i];
      mean[i] = slice->GetMean();
      sigma[i] = slice->GetRMS();
      if(options==1){
	meanError[i] = slice->GetMeanError();
	rmsError[i] = slice->GetRMSError();
      }
      else{
	meanError[i] = 0;
	rmsError[i]  = 0;
      }

      cout<<"x: "<<x[i]<<" mean: "<<mean[i]<<" mean Error: "<<meanError[i] <<" sigma: "<<sigma[i]<<" rms Error: "<<rmsError[i]<<endl;
    }
    TGraphErrors* meangr = new TGraphErrors(number_of_Slices,x,mean,meanError);
    TGraphErrors* rmsgr = new TGraphErrors(number_of_Slices,x,sigma, rmsError);     

    meangr->SetMarkerStyle(21+m);
    meangr->SetMarkerColor(1+m);
    rmsgr->SetMarkerStyle(21+m);
    rmsgr->SetMarkerColor(1+m);

    resultMeanGraphs->Add(meangr);
    resultRMSGraphs->Add(rmsgr);
  }
   
  TCanvas* can = new TCanvas("can", "can", 600, 700); 
  can->cd();
  can->Print(resultFile+"[");





  if(sliceLogy) can->SetLogy();

  for(unsigned int i = 0; i<histos[0].slices.size(); ++i){
    for(unsigned int m = 0; m < histos.size(); ++m ){
       histos[m].slices[i]->SetLineColor(1+m);
       if(sliceXRangeMin!=sliceXRangeMax)histos[m].slices[i]->GetXaxis()->SetRangeUser(sliceXRangeMin,sliceXRangeMax);

       if(m==0){
	 histos[m].slices[i]->SetMaximum(histos[m].slices[i]->GetMaximum()*1.5);
	 histos[m].slices[i]->Draw();
       }
       else histos[m].slices[i]->Draw("same"); 
       
       if(m+1 >= histos.size()) can->Print(resultFile);     
    }
  }
  if(sliceLogy) can->SetLogy(0);


  if(GraphLogy) can->SetLogy();
  if(GraphLogx) can->SetLogx();

  if(resultMeanYmax!=infinity) resultMeanGraphs->SetMaximum(resultMeanYmax);
  if(resultMeanYmin!=infinity) resultMeanGraphs->SetMinimum(resultMeanYmin);

  resultMeanGraphs->SetTitle("Mean");
  resultMeanGraphs->Draw("ap");//should be ap have to change marker style
  can->Print(resultFile);


  if(resultRMSYmax!=infinity) resultRMSGraphs->SetMaximum(resultRMSYmax);
  if(resultRMSYmin!=infinity) resultRMSGraphs->SetMinimum(resultRMSYmin);

  resultRMSGraphs->SetTitle("RMS");
  resultRMSGraphs->Draw("ap");
  can->Print(resultFile);  


  if(GraphLogy) can->SetLogy(0);
  if(GraphLogx) can->SetLogx(0);

  can->Print(resultFile+"]");
  //delete can;
}




