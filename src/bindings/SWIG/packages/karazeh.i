%module karazeh
%{
  #include "karazeh/karazeh.hpp"
  typedef std::string string_t;
  typedef unsigned long uint64_t;
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"