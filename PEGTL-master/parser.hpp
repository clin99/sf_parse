#ifndef SPEF_HPP_
#define SPEF_HPP_

// TODO:
// current_net => Net* _current_net;
// atof.... => use float 
// make std::vector<std::string_view> private and shared
// add constructor for sub structs so we can fully utilize the try_emplace

#include <tao/pegtl.hpp>
#include <iostream>
#include <charconv>
#include <cstring>
#include <algorithm>
#include <utility>
#include <cassert>
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

static const bool disable = false;


// TODO:
void split_on_space(
  const char* beg,
  const char* end,  
  std::vector<std::string_view>& tokens
) {

  tokens.clear();
  size_t len {0};
  const char* itr {beg};
  while(itr != end and *itr != 0){
    if(std::isspace(*itr)) {
      // Consume space
      while(++itr != end and *itr != 0 and std::isspace(*itr)){
      }
    }
    else{
      len = 1;
      // Consume non-space
      while(itr != end and *itr != 0 and not std::isspace(*itr)){
        itr ++;
        len ++;
      }
      tokens.push_back({itr-len+1, len});
    }
  }
}


// Function: split_on_space 
/*
std::vector<std::string_view>& split_on_space(const char* beg, const char* end) {

  // Parse the token.
  const char *token {nullptr};
  size_t len {0};
  static std::vector<std::string_view> tokens;

  tokens.clear();

  for(const char* itr = beg; itr != end && *itr != 0; ++itr) {
    if(std::isspace(*itr)) {
      if(len > 0) {                            // Add the current token.
        tokens.push_back({token, len});
        token = nullptr;
        len = 0;
      }
    } else {
      if(len == 0) {
        token = itr;
      }
      ++len;
    }
  }

  if(len > 0) {
    tokens.push_back({token, len});
  } 
	
  return tokens;
}
*/






enum class ConnectionType {
  INTERNAL,
  EXTERNAL
};

std::ostream& operator<<(std::ostream& os, const ConnectionType& c)
{
	switch(c){
    case ConnectionType::INTERNAL: os << "INTERNAL";    break;
    case ConnectionType::EXTERNAL: os << "EXTERNAL";    break;
		default    : os.setstate(std::ios_base::failbit);
	}
	return os;
}


enum class ConnectionDirection {
  INPUT,
  OUTPUT,
  INOUT
};

std::ostream& operator<<(std::ostream& os, const ConnectionDirection& c)
{
	switch(c){
    case ConnectionDirection::INPUT  : os << "INPUT";     break;
    case ConnectionDirection::OUTPUT : os << "OUTPUT";    break;
    case ConnectionDirection::INOUT  : os << "INOUT";     break;
		default    : os.setstate(std::ios_base::failbit);
	}
	return os;
}


struct Port{
  Port() = default;
  std::string name;
  ConnectionDirection direction;  // I, O, B 
  char type;  // C, L or S
  std::vector<float> values;

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
  return os;  
}  


struct Connection {

  std::string name;
  ConnectionType type;
  ConnectionDirection direction;

  std::optional<std::pair<float, float>> coordinate;
  std::optional<float> load;    
  std::string driving_cell;

  //Connection(const std::string&, ConnectionType, ConnectionDirection);

  Connection() = default;
  //Connection(Connection&&) = default;

  //Connection& operator = (Connection&&) = default;
};

std::ostream& operator<<(std::ostream& os, const Connection& c)
{
  os << c.type << ' ' << c.name << ' ' << c.direction;
  if(c.coordinate.has_value()){
    os << " *C " << std::get<0>(*c.coordinate) << ' ' << std::get<1>(*c.coordinate);
  }
  if(c.load.has_value()){
    os << " *L " << *c.load;
  }
  if(not c.driving_cell.empty()){
    os << " *D " << c.driving_cell;
  }
  return os;  
}



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

  private:

};


std::ostream& operator<<(std::ostream& os, const Net& n)
{
  os << "*D_NET " << n.name << ' ' << n.lcap << '\n';
  if(not n.connections.empty()){
    os << "*CONN\n";
  }
  for(const auto& c: n.connections){
    os << c << '\n';
  }
  if(not n.caps.empty()){
    os << "*CAP\n";
  }
  for(const auto& c: n.caps){
    os << std::get<0>(c);
    if(not std::get<1>(c).empty()){
      os << ' ' << std::get<1>(c);
    }
    os << ' ' << std::get<2>(c) << '\n';
  }
  if(not n.ress.empty()){
    os << "*RES\n";
  }
  for(const auto& r: n.ress){
    os << std::get<0>(r) << ' ' << std::get<1>(r) << ' ' << std::get<2>(r) << '\n';
  }
  return os;  
}



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

std::ostream& operator<<(std::ostream& os, const State& s)
{
	switch(s){
    case State::NONE           : os << "NONE";           break;
    case State::STANDARD       : os << "STANDARD";       break;
    case State::DESIGN_NAME    : os << "DESIGN_NAME";    break;
    case State::DATE           : os << "DATE";           break;
    case State::VENDOR         : os << "VENDOR";         break;
    case State::PROGRAM        : os << "PROGRAM";        break;
    case State::VERSION        : os << "VERSION";        break;
    case State::DESIGN_FLOW    : os << "DESIGN_FLOW";    break;
    case State::DIVIDER        : os << "DIVIDER";        break;
    case State::DELIMITER      : os << "DELIMITER";      break;
    case State::BUS_DELIMITER  : os << "BUS_DELIMITER";  break;
    case State::NAME_MAP       : os << "NAME_MAP";       break;
    case State::PORTS          : os << "PORTS";          break;
	  default             : os.setstate(std::ios_base::failbit);
	}
	return os;
}
 

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


  void add_header(const std::string&);

  std::string dump() const;

  template <typename T>
  friend struct action;

  friend void split_on_space(const char*, const char*, std::vector<std::string_view>&);

  private:
    
    // TODO:
    Net* _current_net {nullptr};
    std::vector<std::string_view> _tokens;
};

inline std::string Data::dump() const {
  std::ostringstream os;
  os 
    << "Standard:" << standard << "\n" 
    << "Design name:" << design_name << "\n" 
    << "Date:" << date << "\n" 
    << "Vendor:" << vendor << "\n"
    << "Program:" << program << "\n"
    << "Version:" << version << "\n"
    << "Design Flow:" << design_flow << "\n"
    << "Divider:" << divider << "\n"
    << "Delimiter:" << delimiter << "\n"
    << "Bus Delimiter:" << bus_delimiter << "\n"
    << "T Unit:" << t_unit << "\n"
    << "C Unit:" << c_unit << "\n"
    << "R Unit:" << r_unit << "\n"
    << "L Unit:" << l_unit << "\n"
  ;
  os << '\n';
  if(not name_map.empty()){
    os << "*NAME_MAP\n";
  }
  for(const auto& [k,v]: name_map){
    os << k << ' ' << v << '\n';
  }
  os << '\n';
  for(const auto iter: ports){
    os << iter.second << '\n';
  }
  os << '\n';
  for(const auto iter: nets){
    os << iter.second << '\n';
  }
  return os.str();
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
      assert(false);
    //  bus_delimiter = s;
    //  state = State::NONE;
      break;
    default:
      break;
  }
}


namespace pegtl = tao::TAO_PEGTL_NAMESPACE;
using token = pegtl::until<pegtl::at<pegtl::space>>;
using delimiter = pegtl::plus<pegtl::space>;

template<typename T>
struct action: pegtl::nothing<T>
{};



//struct Comment: pegtl::must<TAO_PEGTL_KEYWORD("//"), pegtl::until<pegtl::eol>>
struct Comment: pegtl::seq<TAO_PEGTL_STRING("//"), pegtl::until<pegtl::eol>>
{};

//struct DontCare: pegtl::plus<pegtl::sor<pegtl::eol, pegtl::plus<pegtl::space>, Comment>>
//struct DontCare: pegtl::plus<pegtl::sor<pegtl::space, Comment>>
struct DontCare: pegtl::plus<pegtl::space>
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
    if(disable)
      return ; 
    d.add_header(in.string());
  };
};


struct SpefDivider: pegtl::any
{};
template<>
struct action<SpefDivider>  
{
  template <typename Input>
  static bool apply(const Input& in, Data& d){
    if(disable)
      return true; 

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
    if(disable)
      return true; 

    if(in.size() != 1){
      return false;
    }
    d.add_header(in.string());
    return true;
  };
};

struct SpefBusDelimiter: pegtl::must<pegtl::any, pegtl::star<pegtl::space>, pegtl::any>
{};
template<>
struct action<SpefBusDelimiter>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
       return ; 

    std::string str {in.string()};
    str.erase(std::remove_if(str.begin(), str.end(), [](auto c){return std::isspace(c);}), str.end());
    d.add_header(std::move(str));
  };
};





//  Header Section --------------------------------------------------------------------------------

struct rule_standard: pegtl::must<TAO_PEGTL_STRING("*SPEF"), 
  Header, DontCare>
  //pegtl::plus<pegtl::seq<DontCare, QuotedString>>, DontCare> 
  //pegtl::plus<pegtl::blank>, QuotedString, DontCare>
{};

struct rule_design: pegtl::must<TAO_PEGTL_STRING("*DESIGN"), 
  Header, DontCare>
{};

struct rule_date: pegtl::must<TAO_PEGTL_STRING("*DATE"), 
  Header, DontCare>
{};

struct rule_vendor: pegtl::must<TAO_PEGTL_STRING("*VENDOR"), 
  Header, DontCare>
{};

struct rule_program: pegtl::must<TAO_PEGTL_STRING("*PROGRAM"), 
  Header, DontCare>
{};

struct rule_version: pegtl::must<TAO_PEGTL_STRING("*VERSION"), 
  Header, DontCare>
{};

struct rule_design_flow: pegtl::must<TAO_PEGTL_STRING("*DESIGN_FLOW"), 
  Header, DontCare>
{};

struct rule_divider: pegtl::must<TAO_PEGTL_STRING("*DIVIDER"), 
  delimiter, SpefDivider, DontCare>
{};

struct rule_delimiter: pegtl::must<TAO_PEGTL_STRING("*DELIMITER"), delimiter, SpefDelimiter, DontCare>
{};

struct rule_bus_delimiter: pegtl::must<TAO_PEGTL_STRING("*BUS_DELIMITER"), delimiter, SpefBusDelimiter, DontCare>
{};

// TODO: bol
struct rule_unit: pegtl::seq<TAO_PEGTL_STRING("*"), pegtl::one<'T','C','R','L'>,
  TAO_PEGTL_STRING("_UNIT"), delimiter, double_::rule, 
  delimiter, pegtl::opt<pegtl::one<'K','M','U','N','P','F'>>, 
  pegtl::sor<TAO_PEGTL_STRING("HENRY"), TAO_PEGTL_STRING("OHM"), pegtl::one<'S','F','H'>>>
{};
template <>
struct action<rule_unit>  
{
  template <typename Input>
  static bool apply(const Input& in, Data& d){
    if(disable)
      return true; 

    //auto vec = split_on_space(str);

    //auto& vec = split_on_space(in.begin(), in.end());
    split_on_space(in.begin(), in.end(), d._tokens);

    return true;

    /*double scale {1.0};
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
    return true;*/
  }
};


struct rule_name_map_beg: pegtl::seq<TAO_PEGTL_STRING("*NAME_MAP"), DontCare>
{};
template <>
struct action<rule_name_map_beg>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ; 
    d.state = State::NAME_MAP;
  }
};

struct rule_name_map: pegtl::seq<
  pegtl::not_at<TAO_PEGTL_STRING("*PORTS")>, pegtl::not_at<TAO_PEGTL_STRING("*D_NET")>, 
  TAO_PEGTL_STRING("*"), token, delimiter, token
>
{};
template <>
struct action<rule_name_map>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ;
    split_on_space(in.begin(), in.end(), d._tokens); 
    //d.name_map.insert({vec[0], vec[1]});
    d.name_map.try_emplace( std::string{d._tokens[0]}, std::string{d._tokens[1]} );
  }
};


struct rule_port_beg: pegtl::seq<TAO_PEGTL_STRING("*PORTS"), DontCare>
{};
template <>
struct action<rule_port_beg>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ; 
    d.state = State::PORTS;
  }
};

// TODO: All sections are optional
struct rule_port: pegtl::seq<
  pegtl::not_at<TAO_PEGTL_STRING("*D_NET")>, TAO_PEGTL_STRING("*"),
  token, delimiter,
  pegtl::must<pegtl::one<'I','O','B'>>,
  pegtl::opt<
    pegtl::seq<
      delimiter,
      pegtl::sor<
        pegtl::seq<TAO_PEGTL_STRING("*C"), delimiter, double_::rule, delimiter, double_::rule>,
        pegtl::seq<TAO_PEGTL_STRING("*L"), delimiter, double_::rule>,
        pegtl::seq<TAO_PEGTL_STRING("*S"), delimiter, double_::rule, delimiter, double_::rule>
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
    if(disable)
      return true; 

    split_on_space(in.begin(), in.end(), d._tokens); 
    //d.ports.insert({vec[0], {}});
    //auto& p = d.ports.at(vec[0]);

    auto& p = d.ports[std::string{d._tokens[0]}];

    // Set up name 
    p.name = d._tokens[0];

    // Set up port direction
    switch(d._tokens[1][0]){
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
    if(d._tokens.size() > 2){
      p.type = d._tokens[2][1];
    }

    // Insert values
    for(size_t i=3; i<d._tokens.size(); i++){
      p.values.emplace_back(std::strtof(d._tokens[i].data(), nullptr));
    }
    return true;
  }
};



struct rule_conn_beg: pegtl::seq<TAO_PEGTL_STRING("*CONN")>
{};
template <>
struct action<rule_conn_beg>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
  }
};


struct rule_conn: pegtl::seq<
  pegtl::sor<TAO_PEGTL_STRING("*P"), TAO_PEGTL_STRING("*I")>, 
  delimiter, token, delimiter, pegtl::must<pegtl::one<'I','O','B'>>, 

  pegtl::opt<delimiter, pegtl::seq<TAO_PEGTL_STRING("*C"), delimiter, double_::rule, 
    delimiter, double_::rule>>,

  pegtl::opt<delimiter, pegtl::seq<TAO_PEGTL_STRING("*L"), delimiter, double_::rule>>,

  pegtl::opt<delimiter, pegtl::seq<TAO_PEGTL_STRING("*D"), delimiter, 
    pegtl::plus<pegtl::identifier_other>>>
>
{};
template <>
struct action<rule_conn>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ;

    auto &c = d._current_net->connections.emplace_back();

    split_on_space(in.begin(), in.end(), d._tokens);

    c.type = d._tokens[0][1] == 'P' ? ConnectionType::EXTERNAL : ConnectionType::INTERNAL;
    c.name = d._tokens[1];
    switch(d._tokens[2][0]){
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

    for(size_t i=3; i<d._tokens.size(); i++){
      if(d._tokens[i].compare("*C") == 0){
        c.coordinate = std::make_pair(
          std::strtof(d._tokens[i+1].data(), nullptr), std::strtof(d._tokens[i+2].data(), nullptr)
        );
        i += 2;
      }
      else if(d._tokens[i].compare("*L") == 0){
        c.load = std::strtof(d._tokens[i+1].data(), nullptr);
        i += 1;
      }
      else if(d._tokens[i].compare("*D") == 0){
        c.driving_cell = d._tokens[i+1];
        i += 1;
      }
      else{
        std::cout << "Unknown connection information!\n";
      }
    }
  }
};




struct rule_cap_beg: pegtl::seq<TAO_PEGTL_STRING("*CAP")>
{};
template <>
struct action<rule_cap_beg>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){}
};



struct rule_cap_ground: pegtl::seq<
  pegtl::plus<pegtl::digit>, delimiter, token, delimiter, double_::rule
>
{};
template <>
struct action<rule_cap_ground>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ; 

    split_on_space(in.begin(), in.end(), d._tokens);
    // TODO: verify...?
    d._current_net->caps.emplace_back(
      std::forward_as_tuple(d._tokens[1], "", std::strtof(d._tokens[2].data(), nullptr))
    );
  }
};


struct rule_cap_couple: pegtl::seq<
  pegtl::plus<pegtl::digit>, delimiter, token, delimiter, token, delimiter, double_::rule
>
{};
template <>
struct action<rule_cap_couple>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ; 
    split_on_space(in.begin(), in.end(), d._tokens);
    d._current_net->caps.emplace_back(
      std::forward_as_tuple(d._tokens[1], d._tokens[2], std::strtof(d._tokens[3].data(), nullptr))
    );
  }
};



struct rule_res_beg: pegtl::seq<TAO_PEGTL_STRING("*RES")>
{};
template <>
struct action<rule_res_beg>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){}
};

struct rule_res: pegtl::seq<
  pegtl::plus<pegtl::digit>, delimiter,
  token, delimiter, token, delimiter, double_::rule
>
{};
template <>
struct action<rule_res>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ; 

    split_on_space(in.begin(), in.end(), d._tokens);
    d._current_net->ress.emplace_back(
      std::forward_as_tuple(d._tokens[1], d._tokens[2], std::strtof(d._tokens[3].data(), nullptr))
    );
  }
};





struct rule_net_beg: pegtl::seq<
  TAO_PEGTL_STRING("*D_NET"), delimiter, token, delimiter, double_::rule
>
{};
template <>
struct action<rule_net_beg>  
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
    if(disable)
      return ; 
    split_on_space(in.begin(), in.end(), d._tokens);
    
    // TODO: replace current_net with raw pointer?
    // d._current_net = &(d.nets[std::string{vec[1]}]);
    d._current_net = &(d.nets[{std::string{d._tokens[1]}}]);
    //d.current_net = d.nets.insert({std::string{vec[1]}, {}}).first;
    //d.current_net = d.nets.try_emplace(std::string{vec[1]}, {}).first;

    d._current_net->name = d._tokens[1];
    d._current_net->lcap = std::strtof(d._tokens[2].data(), nullptr);
  }
};


struct rule_net_end: pegtl::seq<TAO_PEGTL_STRING("*END")>
{};
template <>
struct action<rule_net_end>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){}
};


//-------------------------------------------------------------------------------------------------
struct rule_spef: pegtl::must<
  pegtl::star<pegtl::space>,
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
  pegtl::rep_max<4, pegtl::seq<rule_unit, DontCare>>,
  // TODO: opt no need star
  pegtl::opt<rule_name_map_beg, pegtl::star<pegtl::seq<rule_name_map, DontCare>>>,
  // TODO: opt port 
  pegtl::opt<rule_port_beg, pegtl::star<pegtl::seq<rule_port, DontCare>>>,
  pegtl::star<rule_net_beg, DontCare,
    // change plus to star
    pegtl::if_must<pegtl::seq<rule_conn_beg, DontCare>, pegtl::star<pegtl::seq<rule_conn, DontCare>>>,
    pegtl::if_must<pegtl::seq<rule_cap_beg,  DontCare>, 
      pegtl::star<pegtl::seq<pegtl::sor<rule_cap_ground, rule_cap_couple>, DontCare>>>,
    pegtl::if_must<pegtl::seq<rule_res_beg,  DontCare>, pegtl::star<pegtl::seq<rule_res, DontCare>>>, 
    rule_net_end, DontCare
  >
>
{};



//-------------------------------------------------------------------------------------------------
//struct rule_qq: pegtl::must<pegtl::plus<pegtl::digit, pegtl::blank, pegtl::alpha, pegtl::space, pegtl::digit>>
struct rule_qq: pegtl::must<pegtl::plus<pegtl::seq<pegtl::digit, pegtl::blank>>, pegtl::alpha>
{};
template <>
struct action<rule_qq>
{
  template <typename Input>
  static void apply(const Input& in, Data& d){
  }
};

template<typename Rule>
struct my_control : tao::pegtl::normal<Rule>
{
   static const std::string error_message;

   template< typename Input, typename... States >
   static void raise(const Input& in, States&&...)
   {
     throw tao::pegtl::parse_error(error_message, in);
   }
};
//template<typename T> const std::string my_control<pegtl::plus<T>>::error_message  = "Plus error"; 
//template<typename T> const std::string my_control<T>::error_message  = "Plus error";
template<> const std::string my_control<pegtl::alpha>::error_message = "Alpha error";
//template<> const std::string my_control<pegtl::digit>::error_message = "Digit error";
//template<> const std::string my_control<pegtl::blank>::error_message = "Blank error";
//template<> const std::string my_control<rule_qq>::error_message = "Rule THIS IS CUSTOM ERROR!";
template<> const std::string my_control<pegtl::plus<pegtl::digit>>::error_message = "Hello";
//template<> const std::string my_control<rule_spef>::error_message = "Rule spef parse error!";
template<> const std::string my_control<pegtl::plus<pegtl::seq<pegtl::digit, pegtl::blank>>>::error_message = "AA";



};    // end of namespace spef. --------------------------------------------------------------------

#endif
