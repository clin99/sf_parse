#ifndef SPEF_HPP_
#define SPEF_HPP_

//#include <boost/spirit/include/classic.hpp>
//#include <boost/spirit/include/qi.hpp> 

#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>
#include <cassert>
//#include <optional>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <array>
#include <map>

namespace spef{


// ------------------------------------------------------------------------------------------------ 
/*
enum class ConnectionType {
  INTERNAL,
  EXTERNAL
};

std::string to_string(ConnectionType);

enum class ConnectionDirection {
  INPUT,
  OUTPUT,
  INOUT
};

std::string to_string(ConnectionDirection);

// Struct: Connection
struct Connection {

  std::string name;
  ConnectionType type;
  ConnectionDirection direction;

  Connection(const std::string&, ConnectionType, ConnectionDirection);

  Connection() = default;
  Connection(Connection&&) = default;

  Connection& operator = (Connection&&) = default;
};

// ------------------------------------------------------------------------------------------------

// Struct: Net
struct Net {

  std::string name;
  float lcap;
  std::vector<Connection> connections;
  std::vector<std::tuple<std::string, float>> caps;
  std::vector<std::tuple<std::string, std::string, float>> ress;

  void scale_capacitance(float);
  void scale_resistance(float);

  Net() = default;
  Net(Net&&) = default;

  Net& operator = (Net&&) = default;
};

// Operator: <<
std::ostream& operator << (std::ostream&, const Net&);

// ------------------------------------------------------------------------------------------------

// Struct: Spef 
struct Spef {
  
  std::optional<char> divider;
  std::optional<char> delimiter;
  std::optional<TimeUnit> time_unit;
  std::optional<CapacitanceUnit> capacitance_unit;
  std::optional<ResistanceUnit> resistance_unit;

  std::unordered_map<std::string, std::string> name_map;

  std::vector<Net> nets;


  void to_capacitance_unit(const CapacitanceUnit&);
  void to_resistance_unit(const ResistanceUnit&);

  Spef() = default;
  Spef(Spef&&) = default;

  Spef& operator = (Spef&&) = default;
};
*/

//  end of spef data structure. --------------------------------------------------------------------


/*
 *  A typical SPEF file will have 4 main sections:
 *  
 *    a header section,
 *    a name map section,
 *    a top level port section, and
 *    the main parasitic description section.
 */


struct Unit : boost::spirit::x3::symbols<double>
{   
	Unit(){   
		add("K" , 1e3)("M" , 1e-3)("U" , 1e-6)("N" , 1e-9)("P" , 1e-12)("F" , 1e-15);   
	}   
} Units;


//const inline auto rule_quoted_string {'"' >> *boost::spirit::x3::char_  >> '"'};

//boost::spirit::x3::rule<class rule_quoted_string, std::string> const rule_quoted_string("Rule Quoted String");
//const inline auto rule_quoted_string_def {'"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::char_ - '"')] >> '"'};

//const inline auto rule_bus_delimiter = boost::spirit::x3::rule<class Bus, std::vector<char>>()  
//                   = "*BUS_DELIMITER" >> boost::spirit::x3::char_ >> boost::spirit::x3::char_;


const inline auto rule_comment = boost::spirit::x3::space | 
  boost::spirit::x3::lexeme[ "//" >> *(boost::spirit::x3::char_ - boost::spirit::x3::eol) >> boost::spirit::x3::eol];

const inline auto rule_quoted_string = boost::spirit::x3::rule<class QuotedString, std::string>() 
                                     = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::char_ - '"')] >> '"';

// Header section
const inline auto rule_standard     {"*SPEF"        >> rule_quoted_string};
const inline auto rule_design_name  {"*DESIGN"      >> rule_quoted_string};
const inline auto rule_data         {"*DATE"        >> rule_quoted_string};
const inline auto rule_vendor       {"*VENDOR"      >> rule_quoted_string};
const inline auto rule_program      {"*PROGRAM"     >> rule_quoted_string};
const inline auto rule_version      {"*VERSION"     >> rule_quoted_string};
const inline auto rule_design_flow  {"*DESIGN_FLOW" >> rule_quoted_string};
const inline auto rule_divider      {"*DIVIDER"     >> boost::spirit::x3::char_};
const inline auto rule_delimiter    {"*DELIMITER"   >> boost::spirit::x3::char_};
const inline auto rule_bus_delimiter{"*BUS_DELIMITER" >> boost::spirit::x3::char_ >> boost::spirit::x3::char_};
const inline auto rule_t_unit       {"*T_UNIT"      >> boost::spirit::x3::double_ >> Units >> 'S'};
const inline auto rule_c_unit       {"*C_UNIT"      >> boost::spirit::x3::double_ >> Units >> 'F'};
const inline auto rule_r_unit       {"*R_UNIT"      >> boost::spirit::x3::double_ >> Units >> "OHM"};
const inline auto rule_l_unit       {"*L_UNIT"      >> boost::spirit::x3::double_ >> Units >> 'H'};


// Name map
//const inline auto rule_net_name {boost::spirit::x3::lexeme[+(boost::spirit::x3::char_ - boost::spirit::x3::eol)]};
const inline auto rule_name {boost::spirit::x3::lexeme[+(boost::spirit::x3::char_("a-zA-Z0-9[]/:_"))]};
const inline auto rule_name_map {"*NAME_MAP" >> *('*' >> boost::spirit::x3::int_ >> rule_name)};

// Port 
const inline auto rule_port {"*PORTS" >> *('*' >> rule_name >> boost::spirit::x3::char_("IOB") >> 
  *(("*C" >> boost::spirit::x3::double_ >> boost::spirit::x3::double_)  | 
    ("*L" >> boost::spirit::x3::double_) |  
    ("*S" >> boost::spirit::x3::double_ >> boost::spirit::x3::double_))
  )
};




//const inline auto rule_namemap_entry {"*NAME_MAP" >> boost::spirit::x3::eol};
//boost::spirit::x3::lexeme[+boost::spirit::x3::digit] >> rule_net_name};
//const inline auto rule_quoted_string = boost::spirit::x3::rule<class QuotedString, std::string>() 
//                                     = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::char_ - '"')] >> '"';
//const inline auto rule_standard = boost::spirit::x3::rule<class Standard, std::string>()
//                                = ("*SPEF" >> rule_quoted_string) ;
//const inline auto rule_design_name = boost::spirit::x3::rule<class Design, std::string>()
//                                   = ("*DESIGN" >> rule_quoted_string) ;


class SpefParser{

  public:
  private: 

    void test();

    //std::string name;
    //static const auto set_name = [&](auto &s){ name = s; };
    //void tt(){
    //  auto r1 = rule_standard[[](auto &t){}];
    //}

    //boost::spirit::x3::rule<class R1> const r1 = "R1";
    //auto const R1_def = '"' >> *boost::spirit::x3::char_  >> '"';

   //static auto name = boost::spirit::x3::rule<class name, std::string>()
   //   = boost::spirit::x3::alpha >> *boost::spirit::x3::alnum;

    // Header section 

  //static auto _quoted_string = boost::spirit::x3::rule<class _quoted_string, std::string>() = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::char_ - '"')] >> '"';

  //static auto _quoted_string = '"' >> boost::spirit::x3::no_skip[*(boost::spirit::x3::char_ - '"')] >> '"';
  //auto _quoted_string {'"' >> *boost::spirit::x3::char_ >> '"'};
  //x3::rule<std::string::iterator, std::vector<int>(),
  //        ascii::space_type> values = x3::int_ % ',';
    //static auto _standard {"*SPEF" >> quoted_string };

};


void SpefParser::test(){
  auto f1 = [](auto &t){};
  auto spef_file = '"' >> rule_design_name[f1];
}

};    // end of namespace spef. --------------------------------------------------------------------

#endif
