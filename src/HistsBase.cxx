#include "HistsBase.h"


using namespace std;



HistsBase::HistsBase(string saveName){
  resultFile=saveName;
  TH1::AddDirectory(kFALSE);

  if(!saveName.empty()){
    can = gROOT->MakeDefCanvas();
    /*
    if(!gROOT->GetListOfCanvases()->IsEmpty())
      can = gROOT->MakeDefCanvas();
    else
      can = new TCanvas("can", "can", 800, 700); 
    */
    can->cd();
    can->Print(resultFile+"[");

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
  else
    cout<<"No Name to Save the Histograms given"<<endl;
}



HistsBase::~HistsBase(){
  if(!resultFile.EqualTo("")){
    can->Print(resultFile+"]");
    delete can;
  }
}



void HistsBase::addFolder(const string dir, const string contains, const string ends){

}

void HistsBase::addFile(string filedir, string hist_draw_option, int color, int marker, bool stack, string nickname){
  if(!boost::filesystem::exists(filedir)){
    cout<<"File does not exist "<<filedir<<endl;
    cout<<"Skipping File"<<endl;
  }
  else{
    filedirs.push_back(filedir);
    hist_draw_options.push_back(hist_draw_option);
    hist_colors.push_back(color);
    hist_stack.push_back(stack);
    hist_marker.push_back(marker);
    nicknames.push_back(nickname);
  }
}
