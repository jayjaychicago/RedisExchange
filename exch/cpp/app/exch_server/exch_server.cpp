#include "exch/exchange.hpp"
#include "exch/redis_support.hpp"
#include "hiredis/adapters/libuv.h"
#include <boost/asio/ip/address.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace exch_server {
struct Program_options {
  Program_options(int argc, char** argv) {
    using namespace boost::program_options;
    variables_map parsed_options;
    store(parse_command_line(argc, argv, description()), parsed_options);
    if (parsed_options.count("help") > 0) {
      help_ = true;
      return;
    }

    redis_address_ = parsed_options["redis-address"].as<std::string>();

    redis_port_ = parsed_options["redis-port"].as<int>();
  }

  static boost::program_options::options_description const& description() {
    using namespace boost::program_options;
    char const* descr = R"(
    Server supporting dynamic creation of markets exchanges


    AllowedOptions)";

    static options_description options{descr};

    if (options.options().empty()) {
      options.add_options()("help,h", "Display help information")(
          "redis-address,a", value<std::string>()->default_value("127.0.0.1"),
          "Address of redis - used for pub/sub and some persistence")(
          "redis-port,p", value<int>()->default_value(6379), "Port for redis");
    }
    return options;
  }

  static void show_help(std::ostream& out) {
    out << description();
    out.flush();
  }

  //! getter for help_ (access is Ro)
  bool help() const { return help_; }

  //! getter for redis_address_ (access is Ro)
  std::string const& redis_address() const { return redis_address_; }

  //! getter for redis_port_ (access is Ro)
  int redis_port() const { return redis_port_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Program_options const& item) {
    out << '\n' << "help:" << item.help_;
    out << '\n' << "redis_address:" << item.redis_address_;
    out << '\n' << "redis_port:" << item.redis_port_;
    return out;
  }

 private:
  bool help_{};
  std::string redis_address_{};
  int redis_port_{};
};

}  // namespace exch_server

int main(int argc, char** argv) {
  using namespace exch_server;
  try {
    Program_options options = {argc, argv};
    if (options.help()) {
      Program_options::show_help(std::cout);
      return 0;
    }

    // custom <main>

    using namespace exch;
    uv_loop_t* loop = uv_default_loop();

    redisAsyncContext* listenContext = redisAsyncConnect(
        options.redis_address().c_str(), options.redis_port());
    redisLibuvAttach(listenContext, loop);

    redisContext* bootstrapContext =
        redisConnect(options.redis_address().c_str(), options.redis_port());

    redisAsyncContext* writeContext = redisAsyncConnect(
        options.redis_address().c_str(), options.redis_port());
    redisLibuvAttach(writeContext, loop);

    // redisSetTcpNoDelay(listenContext);
    // redisSetTcpNoDelay(writeContext);

    Redis_bootstrap_listener bootstrap_redis_listener{*bootstrapContext};
    Redis_listener redis_listener{*listenContext};
    Redis_persister redis_persister{*writeContext};
    Redis_publisher redis_publisher{*writeContext};
    auto config = Exchange_config{};

    Exchange exchange{config, bootstrap_redis_listener, redis_listener,
                      redis_persister, redis_publisher,
                      [&]() { uv_stop(loop); }};

    std::cout << "Running with: " << config
              << "\nExchange server waiting for requests" << std::endl;

    uv_run(loop, UV_RUN_DEFAULT);

    // end <main>
  }
  catch (std::exception const& e) {
    std::cout << "Caught exception: " << e.what() << std::endl;
    Program_options::show_help(std::cout);
    return -1;
  }

  return 0;
}
