#include <string>

std::string forwardfit(std::string antitoptag){
  return "((1-isRealData)*((1-0.04*fabs(Chi2Dis.forwardJet.eta()))*(fabs(Chi2Dis.forwardJet.eta())>=3&&("+antitoptag+"))+ (fabs(Chi2Dis.forwardJet.eta())<3 || !("+antitoptag+"))) + isRealData)*";
}
