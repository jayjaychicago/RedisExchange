#include "random_command/random_command.hpp"
#include "fcs/timestamp/cereal.hpp"
#include "exch/order_book.hpp"
#include <boost/program_options.hpp>

namespace fcs {
namespace app {
namespace random_command_repository {
  struct Program_options
  {
    Program_options(int argc, char** argv) {
      using namespace boost::program_options;
      variables_map parsed_options;
      store(parse_command_line(argc, argv, description()), parsed_options);
      if(parsed_options.count("help") > 0) {
        help_ = true;
        return;
      }

      seed_ = parsed_options["seed"]
        .as< int >();

      center_price_ = parsed_options["center-price"]
        .as< int >();

      valid_submits_ = parsed_options["valid-submits"]
        .as< int >();

      invalid_submits_ = parsed_options["invalid-submits"]
        .as< int >();

      valid_cancels_ = parsed_options["valid-cancels"]
        .as< int >();

      invalid_cancels_ = parsed_options["invalid-cancels"]
        .as< int >();

      valid_replaces_ = parsed_options["valid-replaces"]
        .as< int >();

      invalid_replaces_ = parsed_options["invalid-replaces"]
        .as< int >();

    }

    static boost::program_options::options_description const& description() {
      using namespace boost::program_options;
      static options_description options {
        R"(
    Uses the random_command library to create a flat file with sequences of random
    commands to push through the exchange for one market.


    Allowed Options)"
      };
      if(options.options().empty()) {
        options.add_options()
        ("help,h", "Display help information")
        ("seed,s", value< int >()->default_value(42),
          "Random seed to allow determinism")
        ("center-price,p", value< int >()->default_value(100),
          "Center price for generated random prices")
        ("valid-submits", value< int >()->default_value(1000),
          "Number of valid submits to generate")
        ("invalid-submits", value< int >()->default_value(10),
          "Number of invalid submits to generate")
        ("valid-cancels", value< int >()->default_value(50),
          "Number of valid cancels to generate")
        ("invalid-cancels", value< int >()->default_value(10),
          "Number of invalid cancels to generate")
        ("valid-replaces", value< int >()->default_value(50),
          "Number of valid replaces to generate")
        ("invalid-replaces", value< int >()->default_value(10),
          "Number of invalid replaces to generate");
      }
      return options;
    }

    static void show_help(std::ostream& out) {
      out << description();
      out.flush();
    }

    //! getter for help_ (access is Ro)
    bool help() const { return help_; }

    //! getter for seed_ (access is Ro)
    int seed() const { return seed_; }

    //! getter for center_price_ (access is Ro)
    int center_price() const { return center_price_; }

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
                             Program_options const& item) {
      out << '\n' << "help:" << item.help_;
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
    bool help_ {};
    int seed_ {};
    int center_price_ {};
    int valid_submits_ {};
    int invalid_submits_ {};
    int valid_cancels_ {};
    int invalid_cancels_ {};
    int valid_replaces_ {};
    int invalid_replaces_ {};

  };


} // namespace random_command_repository
} // namespace app
} // namespace fcs

int main(int argc, char** argv) {
  using namespace fcs::app::random_command_repository;
  try{
    Program_options options = { argc, argv };
    if(options.help()) {
      Program_options::show_help(std::cout);
      return 0;
    }

    // custom <main>

    using namespace random_command;
    using namespace exch;

    Market_commands_spec spec {
      static_cast<unsigned int>(options.seed()),
        options.center_price(),
        options.valid_submits(),
        options.invalid_submits(),
        options.valid_cancels(),
        options.invalid_cancels(),
        options.valid_replaces(),
        options.invalid_replaces()
        };

    Command_generator generator { spec };

    generator.generate_commands(std::cout);

    Submit_req r {
      1, 2, 3, Bid_side_e, 100, 200
    };

    r.serialize_to_json(std::cout);

    std::string s { R"(

{
                                 "req_id": 1,



    "price": 99100,


    "market_id": 2,
    "user_id": 3,
    "side": 1,
    "quantity": 2300
}

)"
                      };
    std::istringstream ss { s };
    r.serialize_from_json(ss);
    std::cout << r << std::endl;

    // {
    //   using namespace fcs::timestamp;
    //   using namespace cereal;
    //   auto ts = current_time();
    //   std::stringstream s;
    //   {
    //     JSONOutputArchive ar__(s);
    //     save(ar__, ts);
    //   }

    //   std::string guts { s.str() };
    //   std::cout << "guts is:" << guts << std::endl;
    //   std::cout << "TS is " << ts << std::endl;
    //   ts = current_time();
    //   std::cout << "TS is now " << ts << std::endl;

    //   {
    //     std::istringstream is { s.str() };
    //     JSONInputArchive ari__(is);
    //     load(ari__, ts);
    //   }
    //   std::cout << "TS is read " << ts << std::endl;

    // }


    Fill f {
      123, fcs::timestamp::current_time(), 1, Ask_side_e, 2321, 4123
    };
    f.serialize_to_json(std::cout);

    // end <main>

  } catch(std::exception const& e) {
    std::cout << "Caught exception: " << e.what() << std::endl;
    Program_options::show_help(std::cout);
    return -1;
  }

  return 0;
}
