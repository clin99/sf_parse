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
#include <regex>
#include <string_view>
#include <optional>

namespace spef{


// ------------------------------------------------------------------------------------------------ 
/*


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

   struct rule : seq< plus_minus, sor< hexadecimal, decimal, inf, nan > > {}; 
};


inline std::vector<std::string> split_on_space(const std::string& s){
  const static std::regex ws_re {"\\s+"};
  return std::vector<std::string>(std::sregex_token_iterator(s.begin(), s.end(), ws_re, -1), {});
}

//inline std::vector<std::string_view> split_on_space(std::string_view s){
//  const static std::regex ws_re {"\\s+"};
//  const static std::regex_token_iterator<std::string_view::iterator> end_of_buf;
//  std::regex_token_iterator<std::string_view::iterator> iter(s.begin(), s.end(), ws_re, -1);
//  std::vector<std::string_view> vec;
//  while(iter != end_of_buf){
//    vec.emplace_back(iter->str());
//    iter ++;
//  }
//  return vec;
//}




enum class ConnectionType {
  INTERNAL,
  EXTERNAL
};


enum class ConnectionDirection {
  INPUT,
  OUTPUT,
  INOUT
};


struct Port{
  Port() = default;
  std::string name;
  ConnectionDirection direction;  // I, O, B 
  char type;  // C, L or S
  std::vector<double> values;

  friend std::ostream& operator<<(std::ostream&, const Port&);
};

std::ostream& operator<<(std::ostream& os, const Port& p)  
{  
  os << p.name << ' ';
  switch(p.direction){
    case ConnectionDirection::INPUT:
      os << 'I' << ' ';
      break;
    case ConnectionDirection::OUTPUT:
      os << 'O' << ' ';
      break;
    case ConnectionDirection::INOUT:
      os << 'B' << ' ';
      break;
    default: break;
  }
  os << p.type << ' ';
  for(const auto&v : p.values){
    os << v << ' ';
  }
  os << '\n';
  return os;  
}  


struct Connection {

  std::string name;
  ConnectionType type;
  ConnectionDirection direction;

  std::optional<std::pair<double, double>> coordinate;
  std::optional<double> load;    
  std::optional<std::string> driving_cell;


  //Connection(const std::string&, ConnectionType, ConnectionDirection);

  Connection() = default;
  //Connection(Connection&&) = default;

  //Connection& operator = (Connection&&) = default;
};



struct Net {

  std::string name;
  float lcap;
  std::vector<Connection> connections;
  std::vector<std::tuple<std::string, std::string, float>> caps;
  std::vector<std::tuple<std::string, std::string, float>> ress;

  //void scale_capacitance(float);
  //void scale_resistance(float);

  Net() = default;
  //Net(Net&&) = default;

  //Net& operator = (Net&&) = default;
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
  BUS_DELIMITER,
  NAME_MAP,
  PORTS
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

  const std::unordered_map<char, double> scale = {
    {'K', 1e3},
    {'M', 1e-3},
    {'U', 1e-6},
    {'N', 1e-9},
    {'P', 1e-12},
    {'F', 1e-15}
  };

  double t_unit {0.0};
  double c_unit {0.0};
  double r_unit {0.0};
  double l_unit {0.0};

  std::unordered_map<std::string, std::string> name_map;
  std::unordered_map<std::string, Port> ports;
  std::unordered_map<std::string, Net> nets;

  std::string current_net;

  void add_header(const std::string&);

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
    << "T Unit: " << t_unit << "\n"
    << "C Unit: " << c_unit << "\n"
    << "R Unit: " << r_unit << "\n"
    << "L Unit: " << l_unit << "\n"
  ;
  std::cout << "*NAME_MAP\n";
  for(const auto& [k,v]: name_map){
    std::cout << k << ' ' << v << '\n';
  }
  for(const auto& [k,v]: ports){
    std::cout << "PORT[" << k << "] " << v;
  }
  for(const auto& [k,v]: nets){
    std::cout << "NET[" << k << "] " << '\n';
  }
}

inline void Data::add_header(const std::string& s){
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



//struct Comment: pegtl::must<TAO_PEGTL_KEYWORD("//"), pegtl::until<pegtl::eol>>
struct Comment: pegtl::seq<TAO_PEGTL_STRING("//"), pegtl::until<pegtl::eol>>
{};

//struct DontCare: pegtl::plus<pegtl::sor<pegtl::eol, pegtl::plus<pegtl::space>, Comment>>
//struct DontCare: pegtl::plus<pegtl::sor<pegtl::space, Comment>>
struct DontCare: pegtl::plus<pegtl::sor<pegtl::space, pegtl::eof>>
{};



struct Quote: pegtl::string<'"'>
{};
//struct QuotedString: pegtl::must<Quote, pegtl::plus<pegtl::not_at<Quote>, pegtl::any>, Quote>
struct QuotedString: pegtl::seq<Quote, pegtl::until<Quote>>
//struct QuotedString: pegtl::must<Quote, pegtl::plus<pegtl::alpha>, Quote>
{};
//template<>
//struct action<QuotedString>  
//{
//  template <typename Input>
//  static void apply(const Input& in, Data& d){
//    auto str {in.string()};
//    d.add_header(str);
//    //d.add_header(str.substr(1, str.size()-2));
//  };
//};


struct Header: pegtl::plus<pegtl::seq<DontCare, QuotedString>>
{};
template<>
struct action<Header>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    auto str {in.string()};
    std::cout << "H = " << str << std::endl;
    d.add_header(str);
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





//  Header Section --------------------------------------------------------------------------------

struct rule_standard: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*SPEF"), 
  Header, DontCare>
  //pegtl::plus<pegtl::seq<DontCare, QuotedString>>, DontCare> 
  //pegtl::plus<pegtl::blank>, QuotedString, DontCare>
{};

struct rule_design: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*DESIGN"), 
  Header, DontCare>
{};

struct rule_date: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*DATE"), 
  Header, DontCare>
{};

struct rule_vendor: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*VENDOR"), 
  Header, DontCare>
{};

struct rule_program: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*PROGRAM"), 
  Header, DontCare>
{};

struct rule_version: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*VERSION"), 
  Header, DontCare>
{};

struct rule_design_flow: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*DESIGN_FLOW"), 
  Header, DontCare>
{};

struct rule_divider: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*DIVIDER"), 
  pegtl::plus<pegtl::space>, SpefDivider, DontCare>
{};

struct rule_delimiter: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*DELIMITER"), pegtl::plus<pegtl::blank>, SpefDelimiter, DontCare>
{};

struct rule_bus_delimiter: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*BUS_DELIMITER"), pegtl::plus<pegtl::blank>, SpefBusDelimiter, DontCare>
{};

struct rule_unit: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*"), pegtl::one<'T','C','R','L'>,TAO_PEGTL_STRING("_UNIT"), 
  pegtl::plus<pegtl::blank>, pegtl::plus<pegtl::digit>, pegtl::plus<pegtl::blank>, 
  pegtl::opt<pegtl::one<'K','M','U','N','P','F'>>, 
  pegtl::sor<TAO_PEGTL_STRING("HENRY"), TAO_PEGTL_STRING("OHM"), pegtl::one<'S','F','H'>>>
{};
template <>
struct action<rule_unit>  
{
  template <typename Input>
  static bool apply(const Input& in, Data& d){
    std::string str {in.string()};
    auto vec = split_on_space(str);

    double scale {1.0};
    if(vec[2].size() > 1 and d.scale.find(vec[2][0]) != d.scale.end()){
      scale = d.scale.at(vec[2][0]);
    }

    switch(vec[0][1]){
      case 'T':
        d.t_unit = std::stod(vec[1])*scale;
        break;
      case 'C':
        d.c_unit = std::stod(vec[1])*scale;
        break;
      case 'R':
        d.r_unit = std::stod(vec[1])*scale;
        break;
      case 'L':
        d.l_unit = std::stod(vec[1])*scale;
        break;
      default:
        break;
    }
    return true;
  }
};


struct rule_name_map_beg: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*NAME_MAP"), DontCare>
{};
template <>
struct action<rule_name_map_beg>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    d.state = State::NAME_MAP;
  }
};

struct rule_name_map: pegtl::seq<
  pegtl::bol, pegtl::not_at<TAO_PEGTL_STRING("*PORTS")>, TAO_PEGTL_STRING("*"),
  pegtl::until<pegtl::at<pegtl::blank>>,
  pegtl::plus<pegtl::blank>,
  pegtl::until<pegtl::at<pegtl::space>>
>
{};
template <>
struct action<rule_name_map>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    std::string str {in.string()};
    auto vec = split_on_space(str); 
    d.name_map.insert({vec[0], vec[1]});
  }
};


struct rule_port_beg: pegtl::must<pegtl::bol, TAO_PEGTL_STRING("*PORTS"), DontCare>
{};
template <>
struct action<rule_port_beg>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    d.state = State::PORTS;
  }
};

struct rule_port: pegtl::seq<
  pegtl::bol, pegtl::not_at<TAO_PEGTL_STRING("*D_NET")>, TAO_PEGTL_STRING("*"),
  pegtl::until<pegtl::at<pegtl::blank>>, 
  pegtl::blank,
  pegtl::must<pegtl::one<'I','O','B'>>,
  pegtl::opt<
    pegtl::seq<
      pegtl::blank,
      pegtl::sor<
        pegtl::seq<TAO_PEGTL_STRING("*C"), pegtl::blank, double_::rule, pegtl::blank, double_::rule>,
        pegtl::seq<TAO_PEGTL_STRING("*L"), pegtl::blank, double_::rule>,
        pegtl::seq<TAO_PEGTL_STRING("*S"), pegtl::blank, double_::rule, pegtl::blank, double_::rule>
      >
    >
  >
>
{};
template <>
struct action<rule_port>  
{
  template <typename Input>
  static bool apply(const Input& in, Data& d){
    std::string str {in.string()};
    auto vec = split_on_space(str); 
    d.ports.insert({vec[0], {}});
    auto& p = d.ports.at(vec[0]);

    // Set up name 
    p.name = vec[0];

    // Set up port direction
    switch(vec[1][0]){
      case 'O':
        p.direction = ConnectionDirection::OUTPUT;
        break;
      case 'I':
        p.direction = ConnectionDirection::INPUT;
        break;
      case 'B':
        p.direction = ConnectionDirection::INOUT;
        break;
      default:
        std::cout << "Unknown port type!\n";
        return false;
        break;
    }

    // Set up type 
    if(vec.size() > 2){
      p.type = vec[2][1];
    }

    // Insert values
    for(size_t i=3; i<vec.size(); i++){
      p.values.emplace_back(std::stod(vec[i]));
    }
    return true;
  }
};



struct rule_conn_beg: pegtl::seq<
  pegtl::bol, TAO_PEGTL_STRING("*CONN")
>
{};
template <>
struct action<rule_conn_beg>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    //std::cout << "Conn begin\n";
  }
};

struct rule_conn: pegtl::seq<
  pegtl::bol, pegtl::sor<TAO_PEGTL_STRING("*P"), TAO_PEGTL_STRING("*I")>, pegtl::plus<pegtl::space>,
  pegtl::until<pegtl::at<pegtl::space>>, pegtl::plus<pegtl::space>,
  pegtl::must<pegtl::one<'I','O','B'>>, 

  pegtl::opt<pegtl::plus<pegtl::space>, pegtl::seq<TAO_PEGTL_STRING("*C"), 
  pegtl::plus<pegtl::space>, double_::rule, pegtl::plus<pegtl::space>, double_::rule>>,

  pegtl::opt<pegtl::plus<pegtl::space>, pegtl::seq<TAO_PEGTL_STRING("*L"), 
  pegtl::plus<pegtl::space>, double_::rule>>,

  pegtl::opt<pegtl::plus<pegtl::space>, pegtl::seq<TAO_PEGTL_STRING("*D"), 
  pegtl::plus<pegtl::space>, pegtl::plus<pegtl::identifier_other>>>
>
{};
template <>
struct action<rule_conn>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    //std::cout << "Conn : " << in.string() << '\n';
    auto &n = d.nets.at(d.current_net);
    auto &c = n.connections.emplace_back();

    auto vec = split_on_space(in.string());
    c.type = vec[0][1] == 'P' ? ConnectionType::EXTERNAL : ConnectionType::INTERNAL;
    c.name = vec[1];
    switch(vec[2][0]){
      case 'I':
        c.direction = ConnectionDirection::INPUT;
        break;
      case 'O':
        c.direction = ConnectionDirection::OUTPUT;
        break;
      default:
        c.direction = ConnectionDirection::INOUT;
        break;
    }

    for(size_t i=3; i<vec.size(); i++){
      if(vec[i].compare("*C") == 0){
        c.coordinate = std::make_pair(std::stod(vec[i+1]), std::stod(vec[i+2]));
        i += 2;
      }
      else if(vec[i].compare("*L") == 0){
        c.load = std::stod(vec[i+1]);
        i += 1;
      }
      else if(vec[i].compare("*D") == 0){
        c.driving_cell = vec[i+1];
        i += 1;
      }
      else{
        std::cout << "Unknown connection information!\n";
      }
    }
  }
};




struct rule_cap_beg: pegtl::seq<
  pegtl::bol, TAO_PEGTL_STRING("*CAP")
>
{};
template <>
struct action<rule_cap_beg>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    std::cout << "CAP begin\n";
  }
};



struct rule_cap_ground: pegtl::seq<
  pegtl::bol, pegtl::plus<pegtl::digit>, pegtl::plus<pegtl::space>,
  pegtl::until<pegtl::at<pegtl::space>>, pegtl::plus<pegtl::space>,
  double_::rule
>
{};
template <>
struct action<rule_cap_ground>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    //std::cout << "CAP GROUND =" << in.string() << '\n';
    auto vec = split_on_space(in.string());
    auto &n = d.nets.at(d.current_net);
    n.caps.emplace_back(std::make_tuple(vec[1], "", std::stof(vec[2])));
  }
};


struct rule_cap_couple: pegtl::seq<
  pegtl::bol, pegtl::plus<pegtl::digit>, pegtl::plus<pegtl::space>,
  pegtl::until<pegtl::at<pegtl::space>>, pegtl::plus<pegtl::space>,
  pegtl::until<pegtl::at<pegtl::space>>, pegtl::plus<pegtl::space>,
  double_::rule
>
{};
template <>
struct action<rule_cap_couple>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    //std::cout << "CAP COUPLE =" << in.string() << '\n';
    auto vec = split_on_space(in.string());
    auto &n = d.nets.at(d.current_net);
    n.caps.emplace_back(std::make_tuple(vec[1], vec[2], std::stof(vec[3])));
  }
};



struct rule_res_beg: pegtl::seq<
  pegtl::bol, TAO_PEGTL_STRING("*RES")
>
{};
template <>
struct action<rule_res_beg>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    //std::cout << "RES begin\n";
  }
};

struct rule_res: pegtl::seq<
  pegtl::bol, pegtl::plus<pegtl::digit>, pegtl::plus<pegtl::space>,
  pegtl::until<pegtl::at<pegtl::space>>, pegtl::plus<pegtl::space>,
  pegtl::until<pegtl::at<pegtl::space>>, pegtl::plus<pegtl::space>,
  double_::rule
>
{};
template <>
struct action<rule_res>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    //std::cout << "RES =" << in.string() << '\n';
    auto vec = split_on_space(in.string());
    auto &n = d.nets.at(d.current_net);
    n.ress.emplace_back(std::make_tuple(vec[1], vec[2], std::stof(vec[3])));
  }
};





struct rule_net_beg: pegtl::seq<
  pegtl::bol, TAO_PEGTL_STRING("*D_NET"),
  pegtl::blank, pegtl::until<pegtl::at<pegtl::blank>>,
  pegtl::blank, double_::rule
>
{};
template <>
struct action<rule_net_beg>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    std::string str {in.string()};
    auto vec = split_on_space(str); 

    d.nets.insert({vec[1], {}});
    auto &n = d.nets.at(vec[1]);
    n.lcap = std::stof(vec[2]);
    std::swap(d.current_net, vec[1]);
  }
};


struct rule_net_end: pegtl::seq<
  pegtl::bol, TAO_PEGTL_STRING("*END")
>
{};
template <>
struct action<rule_net_end>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    std::cout << "*END\n\n";
  }
};




//-------------------------------------------------------------------------------------------------
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
  rule_bus_delimiter,
  rule_unit, DontCare,
  rule_unit, DontCare,
  rule_unit, DontCare,
  rule_unit, DontCare,
  rule_name_map_beg,
  pegtl::opt<pegtl::star<pegtl::seq<rule_name_map, DontCare>>>,
  rule_port_beg,
  pegtl::plus<pegtl::seq<rule_port, DontCare>>,
  pegtl::star<
    rule_net_beg, DontCare,
    pegtl::if_must<pegtl::seq<rule_conn_beg, DontCare>, pegtl::plus<pegtl::seq<rule_conn, DontCare>>>,
    pegtl::if_must<pegtl::seq<rule_cap_beg,  DontCare>, 
      pegtl::plus<pegtl::seq<pegtl::sor<rule_cap_ground, rule_cap_couple>, DontCare>>>,
    pegtl::if_must<pegtl::seq<rule_res_beg,  DontCare>, pegtl::plus<pegtl::seq<rule_res, DontCare>>>, 
    rule_net_end, DontCare
  >
>
{};


};    // end of namespace spef. --------------------------------------------------------------------

#endif
