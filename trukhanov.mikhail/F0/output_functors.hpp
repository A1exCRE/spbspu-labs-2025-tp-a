#ifndef OUTPUT_FUNCTORS_HPP
#define OUTPUT_FUNCTORS_HPP

#include <string>
#include <iosfwd>
#include <map>
#include <set>
#include <vector>
#include "index.hpp"

namespace trukhanov
{
  struct ShowEntry
  {
    std::ostream& out;
    void operator()(const std::pair< std::string, std::size_t >& pair) const;
  };

  struct MergeLinesFunctor
  {
    const std::vector< std::string >& lines1;
    const std::vector< std::string >& lines2;
    std::size_t i;
    std::string operator()();
  };

  struct FormatIndexEntry
  {
    std::string operator()(const std::pair< const std::string, ConcordanceIndex >& pair) const;
  };

  struct AddOffset
  {
    std::size_t offset;
    std::size_t operator()(std::size_t val) const;
  };

  struct MergeWithOffset
  {
    ConcordanceIndex& dest;
    std::size_t offset;
    void operator()(const std::pair<const std::string, std::set< std::size_t > >& pair) const;
  };

  struct PairToWordEntry
  {
    WordEntry operator()(const IndexMap::value_type& p) const;
  };

  struct ShowEntryToString
  {
    std::string operator()(const std::pair< std::string, std::size_t >& p) const;
  };
}

#endif
