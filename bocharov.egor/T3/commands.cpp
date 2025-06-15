#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <map>
#include <streamGuard.hpp>

namespace
{
  void getAreaEven(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    bocharov::StreamGuard guard(out);
    std::vector< bocharov::Polygon > filtered;
    filtered.reserve(polygons.size());
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), bocharov::isEven);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), bocharov::getPolygonArea);
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaOdd(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    bocharov::StreamGuard guard(out);
    std::vector< bocharov::Polygon > filtered;
    filtered.reserve(polygons.size());
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), bocharov::isOdd);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), bocharov::getPolygonArea);
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaMean(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::runtime_error("ERROR: there are no polygons");
    }
    bocharov::StreamGuard guard(out);
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), bocharov::getPolygonArea);
    double result = std::accumulate(areas.begin(), areas.end(), 0.0) / areas.size();
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaVertexes(std::ostream & out, const std::vector< bocharov::Polygon > & polygons, size_t num)
  {
    bocharov::StreamGuard guard(out);
    std::vector< bocharov::Polygon > filtered;
    filtered.reserve(polygons.size());
    auto pred = std::bind(bocharov::hasNVertexes, std::placeholders::_1, num);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), bocharov::getPolygonArea);
    double result = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxArea(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    bocharov::StreamGuard guard(out);
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), bocharov::getPolygonArea);
    double result = *std::max_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMaxVertexes(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    bocharov::StreamGuard guard(out);
    auto result = *std::max_element(polygons.begin(), polygons.end(), bocharov::compareVertexes);
    out << std::fixed << std::setprecision(1) << result.points.size() << '\n';
  }

  void getMinArea(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    bocharov::StreamGuard guard(out);
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), bocharov::getPolygonArea);
    double result = *std::min_element(areas.begin(), areas.end());
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getMinVertexes(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    bocharov::StreamGuard guard(out);
    auto result = *std::min_element(polygons.begin(), polygons.end(), bocharov::compareVertexes);
    out << std::fixed << std::setprecision(1) << result.points.size() << '\n';
  }

  void getCountEven(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), bocharov::isEven) << '\n';
  }

  void getCountOdd(std::ostream & out, const std::vector< bocharov::Polygon > & polygons)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), bocharov::isOdd) << '\n';
  }

  void getCountVertexes(std::ostream & out, const std::vector< bocharov::Polygon > & polygons, size_t num)
  {
    auto pred = std::bind(bocharov::hasNVertexes, std::placeholders::_1, num);
    out << std::count_if(polygons.cbegin(), polygons.cend(), pred) << '\n';
  }
}

void bocharov::getArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getAreaEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getAreaOdd, std::ref(out), std::cref(polygons));
  subcmds["MEAN"] = std::bind(getAreaMean, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &)
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    getAreaVertexes(out, polygons, num);
  }
}

void bocharov::getMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMaxArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMaxVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void bocharov::getMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getMinArea, std::ref(out), std::cref(polygons));
  subcmds["VERTEXES"] = std::bind(getMinVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void bocharov::getCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getCountEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getCountOdd, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &)
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    getCountVertexes(out, polygons, num);
  }
}
