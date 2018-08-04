// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_analyze.hpp"
#include "verify_rule.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      void unit_test()
      {
         verify_analyze< opt_must< any, any > >( __LINE__, __FILE__, false, false );
         verify_analyze< opt_must< eof, any > >( __LINE__, __FILE__, false, false );
         verify_analyze< opt_must< opt< any >, any > >( __LINE__, __FILE__, false, false );
         verify_analyze< opt_must< any, opt< any > > >( __LINE__, __FILE__, false, false );
         verify_analyze< opt_must< any, eof > >( __LINE__, __FILE__, false, false );
         verify_analyze< opt_must< opt< any >, opt< any > > >( __LINE__, __FILE__, false, false );
         verify_analyze< opt_must< eof, eof > >( __LINE__, __FILE__, false, false );

         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "", result_type::SUCCESS );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "a", result_type::GLOBAL_FAILURE, 0 );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "b", result_type::SUCCESS, 1 );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ba", result_type::SUCCESS, 2 );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ab", result_type::SUCCESS );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aba", result_type::SUCCESS, 1 );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abb", result_type::SUCCESS, 1 );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abab", result_type::SUCCESS, 2 );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ac", result_type::GLOBAL_FAILURE, 1 );
         verify_rule< opt_must< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "acb", result_type::GLOBAL_FAILURE, 2 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "", result_type::SUCCESS );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "b", result_type::SUCCESS, 1 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "bc", result_type::SUCCESS, 2 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "a", result_type::GLOBAL_FAILURE, 1 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "ab", result_type::GLOBAL_FAILURE, 2 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "ac", result_type::GLOBAL_FAILURE, 2 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "abb", result_type::GLOBAL_FAILURE, 3 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "acc", result_type::GLOBAL_FAILURE, 3 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "acb", result_type::GLOBAL_FAILURE, 3 );
         verify_rule< opt_must< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "abc", result_type::SUCCESS, 0 );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
