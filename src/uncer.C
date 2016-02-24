//main routine call everything else from here.
//so i do not have to change my makefile and everything compiles

#include "slicePlots.h"
#include "simplePlots.h"
#include "effiPlots.h"
#include "cutFlow.h"

#include "TFile.h"
#include "TKey.h"
#include "TString.h"

#include <iostream>
#include <string>

using namespace std;

int main(){

  TFile * file = new TFile("/nfs/dust/cms/user/gonvaq/fast_limits/RecoBp.root","READ");
 
  simplePlots pdfPlots("plots/PDFPlots.ps");
  simplePlots scalePlots("plots/ScalePlots.ps");
  pdfPlots.addFile("/nfs/dust/cms/user/gonvaq/fast_limits/RecoBp.root");
  scalePlots.addFile("/nfs/dust/cms/user/gonvaq/fast_limits/RecoBp.root");
  TIter next(file->GetListOfKeys());
  
  TKey *key;
  while ((key = (TKey*)next())) {
    TString histnameCheck(key->GetName());
    if(histnameCheck.Contains("DATA") || histnameCheck.Contains("Data") )
      continue;
    if(histnameCheck.Contains("scale__plus")){
      histnameCheck.ReplaceAll("__scale__plus","");
      string name (histnameCheck.Data());
      //cout<<histnameCheck<<endl;
      scalePlots.loadHists(name,name);
      scalePlots.loadHists(name+"__scale__plus");
      scalePlots.loadHists(name+"__scale__minus");
      scalePlots.plotHists(2,false);
      scalePlots.clearHists();
      scalePlots.clearLegend();
    }
    if(histnameCheck.Contains("pdf__plus")){
      histnameCheck.ReplaceAll("__pdf__plus","");
      string name (histnameCheck.Data());
      pdfPlots.loadHists(name,name);
      pdfPlots.loadHists(name+"__pdf__plus");
      pdfPlots.loadHists(name+"__pdf__minus");
      pdfPlots.plotHists(2,false);
      pdfPlots.clearHists();
      pdfPlots.clearLegend();
    }

      
  }

  cout<<"I'm done with the uncertainties"<<endl;
  return 0;
}
