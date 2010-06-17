//  Copyright Beman Dawes 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//--------------------------------------------------------------------------------------// 

#ifndef BOOST_QUOTE_MANIP
#define BOOST_QUOTE_MANIP

#include <istream>
#include <ostream>
#include <string>
#include <boost/io/ios_state.hpp>

namespace boost
{
  namespace io
  {
    namespace detail { // forward declare the helpers
      template <class Char, class Traits, class Alloc> struct quote_proxy;
      template <class Char> struct c_str_quote_proxy;
      template <class Char, class Traits, class Alloc> struct unquote_proxy;
    }

    //  ------------  public interface  ------------------------------------------------//

    template <class Char, class Traits, class Alloc>
      detail::quote_proxy<Char, Traits, Alloc>
        quote(const std::basic_string<Char, Traits, Alloc>& s,
              Char escape='\\', Char delim='\"');

    template <class Char>
      detail::c_str_quote_proxy<Char>
        quote(const Char* s, Char escape='\\', Char delim='\"');

    template <class Char, class Traits, class Alloc>
      detail::unquote_proxy<Char, Traits, Alloc>
        unquote(std::basic_string<Char, Traits, Alloc>& s,
                Char escape='\\', Char delim='\"');

    //  -----------  implementation details  -------------------------------------------//

    namespace detail
    {
      //  string inserter helpers

      template <class Char, class Traits, class Alloc>
      struct quote_proxy
      {
        const std::basic_string<Char, Traits, Alloc>& s;
        Char escape;
        Char delim;

        quote_proxy(const std::basic_string<Char, Traits, Alloc>& s_,
          Char escape_, Char delim_)
          : s(s_), escape(escape_), delim(delim_) {}
      };

      template <class Char, class Traits, class Alloc>
      std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, 
        quote_proxy<Char, Traits, Alloc>& proxy)
      {
        os << proxy.delim;
        std::basic_string<Char, Traits, Alloc>::const_iterator end_it = proxy.s.end();
        for (std::basic_string<Char, Traits, Alloc>::const_iterator it = proxy.s.begin();
          it != end_it;
          ++it )
        {
          if (*it == proxy.delim || *it == proxy.escape)
            os << proxy.escape;
          os << *it;
        }
        os << proxy.delim;
        return os;
      }

      //  c_str inserter helpers

      template <class Char>
      struct c_str_quote_proxy
      {
        const Char* s;
        Char escape;
        Char delim;

        c_str_quote_proxy(const Char* s_, Char escape_, Char delim_)
          : s(s_), escape(escape_), delim(delim_) {}
      };

      template <class Char, class Traits>
      std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, 
        c_str_quote_proxy<Char>& proxy)
      {
        os << proxy.delim;
        for (const Char* it = proxy.s;
          *it;
          ++it )
        {
          if (*it == proxy.delim || *it == proxy.escape)
            os << proxy.escape;
          os << *it;
        }
        os << proxy.delim;
        return os;
      }

      //  string extractor helpers

      template <class Char, class Traits, class Alloc>
      struct unquote_proxy
      {
        std::basic_string<Char, Traits, Alloc>& s;
        Char escape;
        Char delim;

        unquote_proxy(std::basic_string<Char, Traits, Alloc>& s_,
          Char escape_, Char delim_)
          : s(s_), escape(escape_), delim(delim_) {}
      };

      template <class Char, class Traits, class Alloc>
      std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& is, 
        unquote_proxy<Char, Traits, Alloc>& proxy)
      {
        Char c;
        is >> c;
        if (c != proxy.delim)
        {
          proxy.s = c;
          is >> proxy.s;
          return is;
        }
        proxy.s.clear();
        {
          boost::io::ios_flags_saver ifs(is);
          is >> std::noskipws;
          for (;;)
          {
            is >> c;
            if (c == proxy.escape)
              is >> c;
            else if (c == proxy.delim)
              break;
            proxy.s += c;
          }
        }
        return is;
      }

    }  // namespace detail

    //  manipulator implementations

    template <class Char, class Traits, class Alloc>
    inline detail::quote_proxy<Char, Traits, Alloc>
      quote(const std::basic_string<Char, Traits, Alloc>& s,  Char escape, Char delim)
    {
      return detail::quote_proxy<Char, Traits, Alloc>(s, escape, delim);
    }

    template <class Char>
    inline detail::c_str_quote_proxy<Char>
      quote(const Char* s,  Char escape, Char delim)
    {
      return detail::c_str_quote_proxy<Char>(s, escape, delim);
    }

    template <class Char, class Traits, class Alloc>
    inline detail::unquote_proxy<Char, Traits, Alloc>
      unquote(std::basic_string<Char, Traits, Alloc>& s,  Char escape, Char delim)
    {
      return detail::unquote_proxy<Char, Traits, Alloc>(s, escape, delim);
    }

  }  // namespace io
}  // namespace boost

#endif // BOOST_QUOTE_MANIP
