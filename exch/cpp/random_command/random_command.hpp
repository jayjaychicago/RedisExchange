#ifndef __RANDOM_COMMAND_RANDOM_COMMAND_HPP__
#define __RANDOM_COMMAND_RANDOM_COMMAND_HPP__

#include "exch/requests.hpp"
#include <iosfwd>
#include <map>
#include <random>
#include <utility>
#include <vector>

namespace random_command {
  using Price_t = exch::Price_t;
  using Price_range_t = std::pair< Price_t, Price_t >;

  class Market_commands_spec
  {
  public:
    Market_commands_spec(
      unsigned int seed,
      Price_t center_price,
      int valid_submits,
      int invalid_submits,
      int valid_cancels,
      int invalid_cancels,
      int valid_replaces,
      int invalid_replaces) :
      seed_ { seed },
      center_price_ { center_price },
      valid_submits_ { valid_submits },
      invalid_submits_ { invalid_submits },
      valid_cancels_ { valid_cancels },
      invalid_cancels_ { invalid_cancels },
      valid_replaces_ { valid_replaces },
      invalid_replaces_ { invalid_replaces } {
    }

    //! getter for seed_ (access is Ro)
    unsigned int seed() const { return seed_; }

    //! getter for center_price_ (access is Ro)
    Price_t center_price() const { return center_price_; }

    //! getter for valid_submits_ (access is Ro)
    int valid_submits() const { return valid_submits_; }

    //! getter for invalid_submits_ (access is Ro)
    int invalid_submits() const { return invalid_submits_; }

    //! getter for valid_cancels_ (access is Ro)
    int valid_cancels() const { return valid_cancels_; }

    //! getter for invalid_cancels_ (access is Ro)
    int invalid_cancels() const { return invalid_cancels_; }

    //! getter for valid_replaces_ (access is Ro)
    int valid_replaces() const { return valid_replaces_; }

    //! getter for invalid_replaces_ (access is Ro)
    int invalid_replaces() const { return invalid_replaces_; }
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Market_commands_spec const& item) {
      out << '\n' << "seed:" << item.seed_;
      out << '\n' << "center_price:" << item.center_price_;
      out << '\n' << "valid_submits:" << item.valid_submits_;
      out << '\n' << "invalid_submits:" << item.invalid_submits_;
      out << '\n' << "valid_cancels:" << item.valid_cancels_;
      out << '\n' << "invalid_cancels:" << item.invalid_cancels_;
      out << '\n' << "valid_replaces:" << item.valid_replaces_;
      out << '\n' << "invalid_replaces:" << item.invalid_replaces_;
      return out;
    }

  private:
    unsigned int const seed_;
    Price_t const center_price_;
    int const valid_submits_;
    int const invalid_submits_;
    int const valid_cancels_;
    int const invalid_cancels_;
    int const valid_replaces_;
    int const invalid_replaces_;

  };


  class Command_generator
  {
  public:
    Command_generator(
      Market_commands_spec const & spec) :
      spec_ { spec },
      engine_ { spec_.seed() } {
    }

    // custom <ClsPublic Command_generator>

    void generate_commands(std::ostream &out) {
      out << "Generating commands based on: " << spec_ << std::endl;
      for(int i=0; i<100; i++) {

      }
    }

    Price_t price() const {
      auto x = dist_(engine_);
      return Price_t(x);
    }

    // end <ClsPublic Command_generator>

  private:
    Market_commands_spec spec_;
    mutable std::mt19937 engine_ {};
    Price_t const min_price_ { int(0.5 * spec_.center_price()) };
    Price_t const max_price_ { int(2.0 * spec_.center_price()) };
    mutable std::uniform_int_distribution< Price_t > dist_ { min_price_, max_price_ };

  };


  // custom <FcbEndNamespace random_command>
  // end <FcbEndNamespace random_command>

} // namespace random_command
#endif // __RANDOM_COMMAND_RANDOM_COMMAND_HPP__
