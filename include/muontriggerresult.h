#include <string>

std::string eletriggerscale(int i=0){
if(i==0) return "(((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=60.)*1.007352)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>60.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=80.)*0.991597)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>80.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=100.)*1.005632)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>100.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=120.)*1.019723)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>120.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=140.)*0.991459)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>140.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=160.)*1.025740)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>160.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=180.)*1.019588)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>180.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=200.)*0.974326)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>200.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=400.)*0.953999)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>400.)*1.000544))";

else if(i==1) return"(((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=60.)*1.027622)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>60.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=80.)*1.000412)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>80.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=100.)*1.014557)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>100.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=120.)*1.028828)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>120.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=140.)*1.000330)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>140.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=160.)*1.034964)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>160.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=180.)*1.028693)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>180.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=200.)*0.983138)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>200.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=400.)*0.962855)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>400.)*1.013021))";

else if(i==2) return"(((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=60.)*0.987036)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>60.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=80.)*0.982762)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>80.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=100.)*0.996687)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>100.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=120.)*1.010596)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>120.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=140.)*0.982568)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>140.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=160.)*1.016494)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>160.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=180.)*1.010461)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>180.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=200.)*0.965493)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>200.&& slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]<=400.)*0.945125)+((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>400.)*0.988039))";

else return " Some error ocurred, this option is not valid for the ele scale factor";
}