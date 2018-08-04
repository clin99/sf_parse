#ifndef SPEF_HPP_
#define SPEF_HPP_

//#include <boost/spirit/include/classic.hpp>
//#include <boost/spirit/include/qi.hpp> 
//#include <boost/spirit/home/x3.hpp>
#include <tao/pegtl.hpp>
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
#include <cctype>

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



/*
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
*/


class SpefParser{

  public:
  private: 

    void test();


};



namespace double_
{
   using namespace tao::TAO_PEGTL_NAMESPACE;  // NOLINT

   struct plus_minus : opt< one< '+', '-' > > {}; 
   struct dot : one< '.' > {}; 

   struct inf : seq< istring< 'i', 'n', 'f' >,
                     opt< istring< 'i', 'n', 'i', 't', 'y' > > > {}; 

   struct nan : seq< istring< 'n', 'a', 'n' >,
                     opt< one< '(' >,
                          plus< alnum >,
                          one< ')' > > > {}; 

   template< typename D > 
   struct number : if_then_else< dot,
                                 plus< D >,
                                 seq< plus< D >, opt< dot, star< D > > > > {}; 

   struct e : one< 'e', 'E' > {}; 
   struct p : one< 'p', 'P' > {}; 
   struct exponent : seq< plus_minus, plus< digit > > {}; 

   struct decimal : seq< number< digit >, opt< e, exponent > > {}; 
   struct hexadecimal : seq< one< '0' >, one< 'x', 'X' >, number< xdigit >, opt< p, exponent > > {}; 

   struct grammar : seq< plus_minus, sor< hexadecimal, decimal, inf, nan > > {}; 
};


enum class State{
  NONE,
  // header
  STANDARD,
  DESIGN_NAME,
  DATE,
  VENDOR,
  PROGRAM,
  VERSION,
  DESIGN_FLOW,
  DIVIDER,
  DELIMITER,
  BUS_DELIMITER 
};

struct Data{
  State state {State::NONE};
  std::string standard;
  std::string design_name;
  std::string date;
  std::string vendor;
  std::string program;
  std::string version;
  std::string design_flow;
  std::string divider;
  std::string delimiter;
  std::string bus_delimiter;

  void add_header(std::string&&);

  void show();
};

inline void Data::show(){
  std::cout 
    << "Standard: " << standard << "\n" 
    << "Design name: " << design_name << "\n" 
    << "Date: " << date << "\n" 
    << "Vendor: " << vendor << "\n"
    << "Program: " << program << "\n"
    << "Version: " << version << "\n"
    << "Design Flow: " << design_flow << "\n"
    << "Divider: " << divider << "\n"
    << "Delimiter: " << delimiter << "\n"
    << "Bus Delimiter: " << bus_delimiter << "\n"
  ;
}

inline void Data::add_header(std::string&& s){
  switch(state){
    case State::NONE:
      standard = s;
      state = State::STANDARD;
      break;
    case State::STANDARD:
      design_name = s;
      state = State::DESIGN_NAME;
      break;
    case State::DESIGN_NAME:
      date = s;
      state = State::DATE;
      break;
    case State::DATE:
      vendor = s;
      state = State::VENDOR;
      break;
    case State::VENDOR:
      program = s;
      state = State::PROGRAM;
      break;
    case State::PROGRAM:
      version = s;
      state = State::VERSION;
      break;
    case State::VERSION:
      design_flow = s;
      state = State::DESIGN_FLOW;
      break;
    case State::DESIGN_FLOW:
      divider = s;
      state = State::DIVIDER;
      break;
    case State::DIVIDER:
      delimiter = s;
      state = State::DELIMITER;
      break;
    case State::DELIMITER:
      bus_delimiter = s;
      state = State::BUS_DELIMITER;
      break;
    case State::BUS_DELIMITER:
      bus_delimiter = s;
      state = State::NONE;
      break;
    default:
      break;
  }
}


namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

template<typename T>
struct action: pegtl::nothing<T>
{};




struct Quote: pegtl::string<'"'>
{};
//struct QuotedString: pegtl::must<Quote, pegtl::plus<pegtl::not_at<Quote>, pegtl::any>, Quote>
struct QuotedString: pegtl::must<Quote, pegtl::until<Quote>>
//struct QuotedString: pegtl::must<Quote, pegtl::plus<pegtl::alpha>, Quote>
{};
template<>
struct action<QuotedString>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    auto str {in.string()};
    d.add_header(str.substr(1, str.size()-2));
  };
};


struct SpefDivider: pegtl::any
{};
template<>
struct action<SpefDivider>  
{
  template <typename Input>
  static bool apply(const Input& in, Data& d){
    if(in.size() != 1){
      return false;
    }
    d.add_header(in.string());
    return true;
  };
};


struct SpefDelimiter: pegtl::any
{};
template<>
struct action<SpefDelimiter>  
{
  template <typename Input>
  static bool apply(const Input& in, Data& d){
    if(in.size() != 1){
      return false;
    }
    d.add_header(in.string());
    return true;
  };
};

struct SpefBusDelimiter: pegtl::must<pegtl::plus<pegtl::not_at<pegtl::eol>, pegtl::any>>
{};
template<>
struct action<SpefBusDelimiter>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    std::string str {in.string()};
    str.erase(std::remove_if(str.begin(), str.end(), [](auto c){return std::isspace(c);}), str.end());
    d.add_header(std::move(str));
  };
};



//struct Comment: pegtl::must<TAO_PEGTL_KEYWORD("//"), pegtl::until<pegtl::eol>>
struct Comment: pegtl::seq<TAO_PEGTL_STRING("//"), pegtl::until<pegtl::eol>>
{};

struct DontCare: pegtl::plus<pegtl::sor<pegtl::eol, pegtl::plus<pegtl::space>, Comment>>
{};

struct rule_standard: pegtl::must<TAO_PEGTL_STRING("*SPEF"), pegtl::plus<pegtl::space>, QuotedString, DontCare> 
{};

struct rule_design: pegtl::must<TAO_PEGTL_STRING("*DESIGN"), pegtl::plus<pegtl::space>, QuotedString, DontCare>
{};

struct rule_date: pegtl::must<TAO_PEGTL_STRING("*DATE"), pegtl::plus<pegtl::space>, QuotedString, DontCare>
{};

struct rule_vendor: pegtl::must<TAO_PEGTL_STRING("*VENDOR"), pegtl::plus<pegtl::space>, QuotedString, DontCare>
{};

struct rule_program: pegtl::must<TAO_PEGTL_STRING("*PROGRAM"), pegtl::plus<pegtl::space>, QuotedString, DontCare>
{};

struct rule_version: pegtl::must<TAO_PEGTL_STRING("*VERSION"), pegtl::plus<pegtl::space>, QuotedString, DontCare>
{};

struct rule_design_flow: pegtl::must<TAO_PEGTL_STRING("*DESIGN_FLOW"), pegtl::plus<pegtl::space>, QuotedString, DontCare>
{};

struct rule_divider: pegtl::must<TAO_PEGTL_STRING("*DIVIDER"), pegtl::plus<pegtl::space>, SpefDivider, DontCare>
{};

struct rule_delimiter: pegtl::must<TAO_PEGTL_STRING("*DELIMITER"), pegtl::plus<pegtl::space>, SpefDelimiter, DontCare>
{};

struct rule_bus_delimiter: pegtl::must<TAO_PEGTL_STRING("*BUS_DELIMITER"), pegtl::plus<pegtl::space>, SpefBusDelimiter, DontCare>
{};


struct rule_spef: pegtl::seq<
  rule_standard, 
  rule_design, 
  rule_date, 
  rule_vendor,
  rule_program,
  rule_version,
  rule_design_flow,
  rule_divider,
  rule_delimiter,
  rule_bus_delimiter
>
{};


};    // end of namespace spef. --------------------------------------------------------------------

#endif
