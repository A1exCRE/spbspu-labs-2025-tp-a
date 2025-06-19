#include "structs.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <iterator>
#include "streamguard.hpp"
#include "delimiter.hpp"

std::istream& kushekbaev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard s(in);
  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';'} >> point.y >> DelimiterIO{ ')' };
  return in;
}

std::istream& kushekbaev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard s(in);
  size_t num_of_inputed_vertices = 0;
  size_t MIN_NUM_OF_VERTICES_IN_POLYGON = 3;
  if (!(in >> num_of_inputed_vertices) || num_of_inputed_vertices < MIN_NUM_OF_VERTICES_IN_POLYGON)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > tmp;
  std::copy_n(std::istream_iterator< Point >(in), num_of_inputed_vertices, tmp.begin());
  if (in)
  {
    polygon.points = std::move(tmp);
  }
  return in;
}

std::ostream& kushekbaev::operator<<(std::ostream& out, const Point& point)
{
  return out << "(" << point.x << ";" << point.y << ")";
}

std::ostream& kushekbaev::operator<<(std::ostream& out, const Polygon& polygon)
{
  out << polygon.points[0];
  for (size_t i = 1; i < polygon.points.size(); ++i)
  {
    out << " " << polygon.points[i];
  }
  return out << "\n";
}
