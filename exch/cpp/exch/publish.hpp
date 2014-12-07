#ifndef __EXCH_PUBLISH_HPP__
#define __EXCH_PUBLISH_HPP__

#include "exch/exch.hpp"

namespace exch {
  class Market_publisher
  {
  public:
    virtual ~Market_publisher() {}
    // custom <ClsPublic Market_publisher>

    virtual void publish_fill() = 0;

    // end <ClsPublic Market_publisher>

  };


  class Redis_publisher :
    public Market_publisher
  {
  };


} // namespace exch
#endif // __EXCH_PUBLISH_HPP__
