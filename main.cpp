#include "parser.hpp"

#include <fstream>
#include <typeinfo>

struct output : public boost::static_visitor<>
{
  void operator()(int d) const { std::cout << "Int = " << d << '\n'; }
  void operator()(double d) const { std::cout << "Double = " << d << '\n'; }
  void operator()(char c) const { std::cout << "Char = " << c << '\n'; }
  void operator()(std::string s) const { std::cout << s << '\n'; }
};


int main(){
  std::ifstream ifs("./simple.spef");
  ifs.seekg(0, std::ios::end);
  size_t size = ifs.tellg();
  std::string buffer(size, ' ');
  ifs.seekg(0);
  ifs.read(&buffer[0], size); 
  ifs.close();

  std::string name;
  std::string design;
  std::string date;
  std::string vendor;
  std::string program;
  std::string version;
  std::string design_flow;
  char divider;
  char delimiter;
  char suffix_delim;
  char postfix_delim;

  auto f1 = [&](auto &t){name = boost::spirit::x3::_attr(t);};
  auto f2 = [&](auto &t){design = boost::spirit::x3::_attr(t);};
  auto f3 = [&](auto &t){date = boost::spirit::x3::_attr(t);};
  auto f4 = [&](auto &t){vendor = boost::spirit::x3::_attr(t);};
  auto f5 = [&](auto &t){program = boost::spirit::x3::_attr(t);};
  auto f6 = [&](auto &t){version = boost::spirit::x3::_attr(t);};
  auto f7 = [&](auto &t){design_flow = boost::spirit::x3::_attr(t);};
  auto f8 = [&](auto &t){divider = boost::spirit::x3::_attr(t);};
  auto f9 = [&](auto &t){delimiter = boost::spirit::x3::_attr(t);};

  auto f10 = [&](auto &t){ 
    auto& v = boost::spirit::x3::_attr(t);
    suffix_delim = boost::fusion::at_c<0>(v);
    postfix_delim = boost::fusion::at_c<1>(v);
     //std::cout << "F9 = " << boost::spirit::x3::_attr(t) << '\n';
  //  char c = std::get<0>(boost::spirit::x3::_attr(t));
  };

  double t_unit;
  auto f11 = [&](auto &t){
    auto& v = boost::spirit::x3::_attr(t);
    t_unit = boost::fusion::at_c<0>(v)*boost::fusion::at_c<1>(v);
  };

  double c_unit;
  auto f12 = [&](auto &t){
    auto& v = boost::spirit::x3::_attr(t);
    c_unit = boost::fusion::at_c<0>(v)*boost::fusion::at_c<1>(v);
  };

  double r_unit;
  auto f13 = [&](auto &t){
    auto& v = boost::spirit::x3::_attr(t);
    r_unit = boost::fusion::at_c<0>(v)*boost::fusion::at_c<1>(v);
  };

  double l_unit;
  auto f14 = [&](auto &t){
    auto& v = boost::spirit::x3::_attr(t);
    l_unit = boost::fusion::at_c<0>(v)*boost::fusion::at_c<1>(v);
  };

  std::unordered_map<int, std::string> name_map;
  auto f15 = [&](auto &t){
    auto& v = boost::spirit::x3::_attr(t);
    for(auto &g: v){
      name_map.insert({boost::fusion::at_c<0>(g), boost::fusion::at_c<1>(g)});
    }
  };


  auto f16 = [&](auto &t){
    //boost::apply_visitor(
    //  output{}, boost::spirit::x3::_attr(t)
    //);

    //auto& v = boost::spirit::x3::_attr(t);
    //auto v = std::move(boost::spirit::x3::_attr(t));

    //boost::variant::apply_visitor(
    //  boost::spirit::x3::_attr(t),
    //  [](auto &t){}
    //);
    
    //std::cout << "V size = " << v.size() << '\n';
    //for(const auto &g: v){
      //auto name = boost::fusion::at_c<0>(g);
      //auto type = boost::fusion::at_c<1>(g);
      //std::cout << name << " = " << type << '\n';
      //std::cout << "Fusion size = " << boost::fusion::size(g) << "\n";
      //std::cout << boost::fusion::at_c<0>(g) << '\n';
      //std::cout << boost::fusion::at_c<1>(g) << '\n';
      //name_map.insert({boost::fusion::at_c<0>(g), boost::fusion::at_c<1>(g)});
    //}
  };



  auto r1 = spef::rule_standard[f1];
  auto r2 = spef::rule_design_name[f2];
  auto r3 = spef::rule_data[f3];
  auto r4 = spef::rule_vendor[f4];
  auto r5 = spef::rule_program[f5];
  auto r6 = spef::rule_version[f6];
  auto r7 = spef::rule_design_flow[f7];

  auto ret = boost::spirit::x3::phrase_parse(buffer.begin(), buffer.end(), 
      spef::rule_standard[f1]    >> 
      spef::rule_design_name[f2] >>
      spef::rule_data[f3]        >> 
      spef::rule_vendor[f4]      >>
      spef::rule_program[f5]     >>
      spef::rule_version[f6]     >> 
      spef::rule_design_flow[f7] >>
      spef::rule_divider[f8]     >>
      spef::rule_delimiter[f9]   >>
      //"*BUS_DELIMITER" >> boost::spirit::x3::char_ //>> boost::spirit::x3::char_
      //"*BUS_DELIMITER" >> boost::spirit::x3::char_[f9] >> boost::spirit::x3::char_[f10]
      //spef::rule_bus_delimiter[f9]
      spef::rule_bus_delimiter[f10] >>
      spef::rule_t_unit[f11] >> 
      spef::rule_c_unit[f12] >> 
      spef::rule_r_unit[f13] >> 
      spef::rule_l_unit[f14] >>
     *spef::rule_name_map[f15] >>
     *spef::rule_port
 //    *spef::rule_port[f16]
    , spef::rule_comment);
      //, boost::spirit::x3::ascii::space);

  //boost::spirit::x3::phrase_parse(buffer.begin(), buffer.end(), 
  //    spef::rule_standard    >> 
  //    spef::rule_design_name 
  //    , boost::spirit::x3::ascii::space, name, design);


  std::cout << "Parse success? " << std::boolalpha << ret << '\n' << '\n';

  std::cout << name << "\n";
  std::cout << design << "\n";
  std::cout << date << "\n";
  std::cout << vendor << "\n";
  std::cout << program << "\n";
  std::cout << version << "\n";
  std::cout << design_flow << "\n";
  std::cout << divider << "\n";
  std::cout << delimiter << "\n";
  std::cout << suffix_delim << postfix_delim << "\n";
  std::cout << t_unit << "\n";
  std::cout << c_unit << "\n";
  std::cout << r_unit << "\n";
  std::cout << l_unit << "\n";

  for(const auto& [k, v]: name_map){
    std::cout << k << ' ' << v << '\n';
  }
  exit(1); 


//  spef::SpefParser parser;
//
//  std::string str("GG ads");
//  auto _quoted_string {'"' >> *boost::spirit::x3::char_  >> '"'};
//
//  //x3::rule<class QQ> qq = '"' >> *boost::spirit::x3::char_  >> '"';
//  x3::rule<class QQ> const qq = "gg"; //'"' >> *boost::spirit::x3::char_  >> '"');
//  auto const QQ_def = '"' >> *boost::spirit::x3::char_  >> '"';
//  auto zz {'"' >> *x3::char_  >> '"'};
//
//  auto f = [](auto& ctx){  };
  //boost::spirit::x3::parse(str.begin(), str.end(), '{' >> boost::spirit::x3::int_[ [](auto &t){} ] >> '}');

//    boost::spirit::x3::rule<class R1> const r1 = "R1";
//    auto R1_def = '"' >> *boost::spirit::x3::char_  >> '"';

 // auto Z = boost::spirit::x3::rule<class Standard>(){(boost::spirit::x3::int_)};
 //            = (boost::spirit::x3::int_) ;


  //using x3::lexeme;
  //using x3::raw;
  //using x3::char_;
  //using x3::ascii::alnum;
  //using x3::ascii::alpha;
  //                 
  //x3::rule<class identifier, std::string> const identifier = "identifier";
  //auto const identifier_def = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];
}
