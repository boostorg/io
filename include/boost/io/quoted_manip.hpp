//  Copyright Beman Dawes 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//--------------------------------------------------------------------------------------// 

#ifndef BOOST_QUOTE_MANIP
#define BOOST_QUOTE_MANIP

#include <iosfwd>
#include <string>
#include <boost/io/ios_state.hpp>

namespace boost
{
  namespace io
  {
    namespace detail { template <class String, class Char> struct quoted_proxy; }

    //  ------------  public interface  ------------------------------------------------//

    template <class Char, class Traits, class Alloc>
      detail::quoted_proxy<std::basic_string<Char, Traits, Alloc> const &, Char>
        quoted(const std::basic_string<Char, Traits, Alloc>& s,
               Char escape='\\', Char delim='\"');

    template <class Char>
      detail::quoted_proxy<const Char*, Char>
        quoted(const Char* s, Char escape='\\', Char delim='\"');

    template <class Char, class Traits, class Alloc>
      detail::quoted_proxy<std::basic_string<Char, Traits, Alloc> &, Char>
        quoted(std::basic_string<Char, Traits, Alloc>& s,
               Char escape='\\', Char delim='\"');

    //  -----------  implementation details  -------------------------------------------//

    namespace detail
    {

      template <class String, class Char>
      struct quoted_proxy
      {
        String  string;
        Char    escape;
        Char    delim;

        quoted_proxy(String s_, Char escape_, Char delim_)
          : string(s_), escape(escape_), delim(delim_) {}
      };

      template <class Char, class Traits, class Alloc>
      std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, 
        const quoted_proxy<std::basic_string<Char, Traits, Alloc> const &, Char>& proxy)
      {
        os << proxy.delim;
        std::basic_string<Char, Traits, Alloc>::const_iterator end_it = proxy.string.end();
        for (std::basic_string<Char, Traits, Alloc>::const_iterator it = proxy.string.begin();
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

      template <class Char, class Traits, class Alloc>
      inline
      std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, 
        const quoted_proxy<std::basic_string<Char, Traits, Alloc>&, Char>& proxy)
      {
        return os <<
          *reinterpret_cast<const quoted_proxy<std::basic_string
            <Char, Traits, Alloc> const &, Char>*>(&proxy);
      }

      template <class Char, class Traits>
      std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, 
        const quoted_proxy<const Char*, Char>& proxy)
      {
        os << proxy.delim;
        for (const Char* it = proxy.string;
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

      template <class Char, class Traits, class Alloc>
      std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& is, 
        const quoted_proxy<std::basic_string<Char, Traits, Alloc>&, Char>& proxy)
      {
        Char c;
        is >> c;
        if (c != proxy.delim)
        {
          proxy.string = c;
          is >> proxy.string;
          return is;
        }
        proxy.string.clear();
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
            proxy.string += c;
          }
        }
        return is;
      }

    }  // namespace detail

    //  manipulator implementations

    template <class Char, class Traits, class Alloc>
    inline detail::quoted_proxy<std::basic_string<Char, Traits, Alloc> const &, Char>
    quoted(const std::basic_string<Char, Traits, Alloc>& s, Char escape, Char delim)
    {
      return detail::quoted_proxy<std::basic_string<Char, Traits, Alloc> const &, Char>
        (s, escape, delim);
    }

    template <class Char>
    inline detail::quoted_proxy<const Char*, Char>
    quoted(const Char* s, Char escape, Char delim)
    {
      return detail::quoted_proxy<const Char*, Char> (s, escape, delim);
    }


    template <class Char, class Traits, class Alloc>
    inline detail::quoted_proxy<std::basic_string<Char, Traits, Alloc> &, Char>
    quoted(std::basic_string<Char, Traits, Alloc>& s, Char escape, Char delim)
    {
      return detail::quoted_proxy<std::basic_string<Char, Traits, Alloc>&, Char>
        (s, escape, delim);
    }

  }  // namespace io
}  // namespace boost

#endif // BOOST_QUOTE_MANIP
