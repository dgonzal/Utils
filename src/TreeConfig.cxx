#include "TreeConfig.h"

Draw::Draw(std::vector<std::string> input, std::string factors){
  if(input.size()==0)
    std::cout<<"draw command not found"<<std::endl;
  if(input.size()>0)
    command = input[0];
  if(input.size()>1){
    if(factors.empty())
      weight = input[1];
    else
      weight = factors+"*"+input[1];
  }
  if(input.size()>2)
    binning = input[2];
  if(input.size()>3)
    x_axis = input[3];
  if(input.size()>4)
    y_axis = input[4];
  if(input.size()>5){
    //std::cout<<input[5]<<std::endl;
    legend = string_bool(input[5]);
  }
  if(input.size()>6)
    data_option = input[6];
  if(input.size()>7)
    logy= string_bool(input[7]);
}
void Draw::print(){
  std::cout<<" command "<<command<<" weight "<<weight<<" binning "<<binning<<" x axis "<<x_axis<<" y axis "<<y_axis<<" data option "<<data_option<<std::endl;
}

Data::Data(std::vector<std::string> input){
  if(input.size()==0)
    std::cout<<"file dir missing"<<std::endl;
  //for(auto i : input)
  //  std::cout<<i<<std::endl;
  if(input.size()>0)
     filedir = input[0];
  if(input.size()>1)
     hist_draw_option = input[1];
  if(input.size()>2)
    color = atoi(input[2].c_str());
  if(input.size()>3)
    marker = atoi(input[3].c_str());
  if(input.size()>4){
    //std::cout<<input[4]<<std::endl;
    stack = string_bool(input[4]);
  }
  if(input.size()>5)
    nickname = input[5];
  if(input.size()>6){
    //std::cout<<input[6]<<std::endl;
    uncer = stod(input[6]);
  }
  if(input.size()>7){
    //std::cout<<input[7]<<std::endl;
    scalefactor= stod(input[7]); 
  }
}

TreeConfig::TreeConfig(std::vector<std::string> configfiles){
  bool files    = false;
  bool config   = false;
  bool commands = false;
  //search config files for additional files
  std::vector<std::string> final_configfiles = configfiles;
  for(auto file : configfiles){
    std::ifstream infile(file);
    std::string str;
    while (std::getline(infile, str)) {
      if(boost::algorithm::contains(str,"load config files")){
	str.erase(str.begin(), str.begin()+17); 
	boost::trim_if(str, boost::is_any_of("\t "));
	std::vector<std::string> split_strs;	
	boost::split(split_strs,str,boost::is_any_of("\t "),boost::token_compress_on);
	for(auto item : split_strs){
	  if (std::find(final_configfiles.begin(), final_configfiles.end(), item) != final_configfiles.end())
	    continue;
	  final_configfiles.push_back(item);
	}
      }
    }
    infile.close();
  }
  for(auto file : final_configfiles){
    std::cout<<"Entering config file "<<file<<std::endl;
    std::ifstream infile(file);
    std::string str;
    while (std::getline(infile, str)) {
      // output the line
      if(str.empty())continue;
      if(boost::algorithm::contains(str,"//"))continue;
      if(boost::algorithm::contains(str,"load config files"))continue;
      std::cout << str << std::endl;
      if(boost::algorithm::contains(str,"*****")){
	files=false; config=false; commands=false;
      }
      if(boost::algorithm::contains(str,"###File")){
	files = true; config=false; commands=false;
	continue;
      }
      else if(boost::algorithm::contains(str,"###Draw")){
        std::cout<<"found draw command"<<std::endl;
	commands = true; files=false; config=false; 
	continue;
      }
      else if(boost::algorithm::contains(str,"###Config")){
	commands = false; files=false; config=true; 
	continue;
      }
      
      std::vector<std::string> split_strs;	
      if(commands){
	if(boost::algorithm::contains(str,"Factor")){
	  boost::split(split_strs,str,boost::is_any_of("\t "),boost::token_compress_on);
	  factors = split_strs[1];
	  continue;
	}
        std::cout<<"found draw command"<<std::endl;
	boost::trim_if(str, boost::is_any_of(";"));
	boost::split(split_strs,str,boost::is_any_of(";"));
	draw_commands.push_back(Draw(split_strs,factors));
        draw_commands[draw_commands.size()-1].print();
      }
      if(files){
	if(boost::algorithm::contains(str,"Prefix")){
	  boost::split(split_strs,str,boost::is_any_of("\t "),boost::token_compress_on);
	  if(split_strs.size()>1)
	    dir_prefix = split_strs[1];
	  continue; 
	}
	boost::trim_if(str, boost::is_any_of(";"));
	boost::split(split_strs,str,boost::is_any_of(";"));
	split_strs[0] = dir_prefix+split_strs[0];
	file_dirs.push_back(Data(split_strs));
      }
      if(config){
	boost::trim_if(str, boost::is_any_of("\t "));
	boost::split(split_strs,str,boost::is_any_of("\t "),boost::token_compress_on);
	split_strs.erase(split_strs.begin());
	if(boost::algorithm::contains(str,"ErrWeight"))
	  errors.push_back(split_strs);
	if(boost::algorithm::contains(str,"ErrFolder"))
	  error_folders.push_back(split_strs);
	if(boost::algorithm::contains(str,"Alias"))
	  alias.push_back(split_strs);
	if(boost::algorithm::contains(str,"Name"))
	  name = split_strs[0];
	if(boost::algorithm::contains(str,"Tree"))
	  analysistree = split_strs[0];
	if(boost::algorithm::contains(str,"RatioLimits")){
	  if(split_strs.size()<2){
	    std::cout<<"Less than two entries for the ratio, taking the default values 0.39-1.69"<<std::endl;
	    continue;
	  }
	  ratio_up = stod(split_strs[0]);
	  ratio_down = stod(split_strs[1]);
	}
	if(boost::algorithm::contains(str,"MCRatioOnly"))
	  mcratioonly = string_bool(split_strs[0]);
	if(boost::algorithm::contains(str,"DrawRatio")){
	  ratio= string_bool(split_strs[0]);
	}
	if(boost::algorithm::contains(str,"Single"))
	  single = string_bool(split_strs[0]);
	if(boost::algorithm::contains(str,"TLatex"))
	  fig_text.push_back(make_tlatex(split_strs));
	if(boost::algorithm::contains(str,"ForwardCorr"))
	  forward_corr = string_bool(split_strs[0]);
	if(boost::algorithm::contains(str,"MuTrkCorr"))
	  mutrk = string_bool(split_strs[0]);
	if(boost::algorithm::contains(str,"EleTrigCorr"))
	  eletrigger = string_bool(split_strs[0]);
 
      }          
      // now we loop back and get the next line in 'str'
    }
    infile.close();
  }
}


// the order is: text x_position y_position size font align 
TLatex* TreeConfig::make_tlatex(std::vector<std::string> tlatex_input){
  if(tlatex_input.size()<2){
    std::cout<<"Not enoughs arguments for TLatex object to ber created. Nothing will be printed on the canvas"<<std::endl;
    return new TLatex(3.5,24,"");
  }
  std::string text_content = tlatex_input[0];
  boost::algorithm::replace_all(text_content,"~"," ");
  boost::algorithm::replace_all(text_content,"SPACE"," ");
  boost::algorithm::replace_all(text_content,"space"," ");
  
  TLatex *text = new TLatex(3.5, 24,text_content.c_str()); //CMS Preliminary
  text->SetNDC();
  text->SetX(stod(tlatex_input[1]));
  if(tlatex_input.size()>2) text->SetY(stod(tlatex_input[2]));
  else text->SetY(.95);
  if(tlatex_input.size()>3) text->SetTextSize(stod(tlatex_input[3]));
  else text->SetTextSize(0.04);
  if(tlatex_input.size()>4) text->SetTextFont(stod(tlatex_input[4]));
  else text->SetTextFont(42);
  if(tlatex_input.size()>5) text->SetTextAlign(stod(tlatex_input[5]));
  else text->SetTextAlign(33);
  return text;
}

bool string_bool(std::string input){
  boost::replace_all(input," ","");
  if(boost::iequals(input,"true") || boost::iequals(input,"1")){
    return true;
  }
  return false;
}
