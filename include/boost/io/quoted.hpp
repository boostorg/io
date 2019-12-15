/*
Copyright 2010 Beman Dawes

Copyright 2019 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef BOOST_IO_QUOTED_HPP
#define BOOST_IO_QUOTED_HPP

#include <boost/io/ios_state.hpp>
#include <ios>
#include <iterator>
#include <string>

namespace boost {
namespace io {
namespace detail {

template<class String, class Char>
class quoted_proxy {
public:
    quoted_proxy(String string_, Char escape_, Char delim_)
        : string(string_)
        , escape(escape_)
        , delim(delim_) { }

    String string;
    Char escape;
    Char delim;

private:
    quoted_proxy& operator=(const quoted_proxy&);
};

template<class Char, class Traits>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
    const quoted_proxy<const Char*, Char>& proxy)
{
    os << proxy.delim;
    for (const Char* it = proxy.string; *it; ++it) {
        if (*it == proxy.delim || *it == proxy.escape) {
            os << proxy.escape;
        }
        os << *it;
    }
    os << proxy.delim;
    return os;
}

template<class Char, class Traits, class Alloc>
inline std::basic_ostream<Char, Traits>&
quoted_output(std::basic_ostream<Char, Traits>& os,
    const std::basic_string<Char, Traits, Alloc>& string, Char escape,
    Char delim)
{
    os << delim;
    typename std::basic_string<Char, Traits,
        Alloc>::const_iterator end = string.end();
    for (typename std::basic_string<Char, Traits,
        Alloc>::const_iterator it = string.begin(); it != end; ++it) {
        if (*it == delim || *it == escape) {
            os << escape;
        }
        os << *it;
    }
    os << delim;
    return os;
}

template <class Char, class Traits, class Alloc>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
    const quoted_proxy<const std::basic_string<Char, Traits, Alloc>&,
        Char>& proxy)
{
    return boost::io::detail::quoted_output(os, proxy.string, proxy.escape,
        proxy.delim);
}

template<class Char, class Traits, class Alloc>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
    const quoted_proxy<std::basic_string<Char, Traits, Alloc>&, Char>& proxy)
{
    return boost::io::detail::quoted_output(os, proxy.string, proxy.escape,
        proxy.delim);
}

template<class Char, class Traits, class Alloc>
inline std::basic_istream<Char, Traits>&
operator>>(std::basic_istream<Char, Traits>& is,
    const quoted_proxy<std::basic_string<Char, Traits, Alloc>&, Char>& proxy)
{
    proxy.string.clear();
    Char ch;
    if (!(is >> ch).good()) {
        return is;
    }
    if (ch != proxy.delim) {
        is.unget();
        is >> proxy.string;
        return is;
    }
    {
        boost::io::ios_flags_saver ifs(is);
        std::noskipws(is);
        while ((is >> ch).good() && ch != proxy.delim) {
            if (ch == proxy.escape && !(is >> ch).good()) {
                break;
            }
            proxy.string.push_back(ch);
        }
    }
    return is;
}

} /* detail */

template<class Char, class Traits, class Alloc>
inline detail::quoted_proxy<const std::basic_string<Char, Traits, Alloc>&,
    Char>
quoted(const std::basic_string<Char, Traits, Alloc>& s, Char escape='\\',
    Char delim='\"')
{
    return detail::quoted_proxy<const std::basic_string<Char, Traits, Alloc>&,
        Char>(s, escape, delim);
}

template<class Char, class Traits, class Alloc>
inline detail::quoted_proxy<std::basic_string<Char, Traits, Alloc>&, Char>
quoted(std::basic_string<Char, Traits, Alloc>& s, Char escape='\\',
    Char delim='\"')
{
    return detail::quoted_proxy<std::basic_string<Char, Traits, Alloc>&,
        Char>(s, escape, delim);
}

template<class Char>
inline detail::quoted_proxy<const Char*, Char>
quoted(const Char* s, Char escape='\\', Char delim='\"')
{
    return detail::quoted_proxy<const Char*, Char>(s, escape, delim);
}

} /* io */
} /* boost */

#endif
