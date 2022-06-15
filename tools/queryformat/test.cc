#include <iostream>

#include "Parser.h"

#include <zypp/misc/DefaultLoadSystem.h>
#include <zypp/ResPool.h>

using std::cout;
using std::cerr;
using std::endl;

//////////////////////////////////////////////////////////////////
namespace zypp
{
  ///////////////////////////////////////////////////////////////////
  namespace qf
  {




    ///////////////////////////////////////////////////////////////////
    struct Renderer
    {
      using value_type = Token;
      using ptr_type = std::shared_ptr<value_type>;

      Renderer()
      {}

      Renderer( Format && format_r )
      : format { std::move(format_r) }
      {}





      const Format format;
    };



  } // namespace qf
} // namespace zypp

///////////////////////////////////////////////////////////////////
void process( std::string_view qf_r )
{
  using namespace zypp;
  qf::Renderer render;
  {
    qf::Format format;
    if ( qf::parse( qf_r, format ) )
      cout << "Parsed:>>>" << format << "<<<" << endl;
    else
      cout << "Oopsed:>>>" << format << "<<<" << endl;
    render = qf::Renderer(std::move(format));
  }

  static const bool once __attribute__ ((__unused__)) = [](){
    zypp::misc::defaultLoadSystem( zypp::misc::LS_NOREFRESH );
    return true;
  }();

  unsigned max = 10;
  for ( const auto & el : zypp::ResPool::instance() ) {
    if ( not max-- ) break;
    cout << qf.render(el);
  }
}

///////////////////////////////////////////////////////////////////
int main( int argc, const char ** argv )
{
  using namespace zypp;
  ++argv,--argc;

  qf::Format format;
  cout << "Parsed:>>>" << format << "<<<" << endl;
  qf::parse( "%{NAME} %{SIZE}\\n", format );
  cout << "Parsed:>>>" << format << "<<<" << endl;
  qf::parse( "%{NAME}\\n", format );
  cout << "Parsed:>>>" << format << "<<<" << endl;
  return 13;

  if ( argc ) {
    while ( argc ) {
      process( std::string_view(*argv) );
      ++argv,--argc;
    }
  }
  else {
    process( std::string_view( "%{NAME}-%{EDITION}.%{ARCH}\n" ) );
  }
#if 0
  else
  {
    for ( const char * qf : {
           "", "\\", "\\\\", "\n", "\\n", "\\%", "\\x",
           "%{NAME} %{SIZE}\\n",
           "%-{NAME} %-0009{SIZE}\\n",
           "[%-05{FILENAMES} %09{FILESIZES}\n]",
           "[%{NAME} %{FILENAMES}\\n]",
           "[%{=NAME} %{FILENAMES}\\n]",
           "%{NAME} %{INSTALLTIME:date}\\n",
           "[%{FILEMODES:perms} %{FILENAMES}\\n]",
           "[%{REQUIRENAME} %{REQUIREFLAGS:depflags} %{REQUIREVERSION}\\n]",
           "\\%\\x\\n%|SOMETAG?{present}:{missing}|",
    } )
    {
      qf::Format result;
      if ( qf::parse( qf, result ) )
        cout << "Parsed: " << result << endl;
    }
  }
#endif
  return 0;
}


