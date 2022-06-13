#include <iostream>
#include <boost/program_options.hpp>
#include "Server.hpp"
#include "Proxy.hpp"
#include "Log.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    try {
        po::options_description desc("Babel Server\n\n $ openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem\n\nOptions");

        desc.add_options()
            ("help,h", "Help message")
            ("port", po::value<int>()->default_value(5000), "Server IP/TCP Port")
            ("tls-key", po::value<std::string>()->default_value("key.pem"), "TLS key file")
            ("tls-cert", po::value<std::string>()->default_value("cert.pem"), "TLS cert file")
            ("proxy-start", po::value<int>()->default_value(5001), "Proxy (for Audio) start Port")
            ("proxy-end", po::value<int>()->default_value(5004), "Proxy (for Audio) end Port")
        ;

        auto parsed = po::command_line_parser(argc, argv)
            .options(desc).positional({}).run();

        po::variables_map opts;
        po::store(parsed, opts);
        po::notify(opts);

        if (opts.count("help")) {
            std::cout << desc << std::endl;
            return (0);
        }

        std::string server_tls_cert_path = opts["tls-cert"].as<std::string>();
        std::string server_tls_key_path = opts["tls-key"].as<std::string>();
        int server_port = opts["port"].as<int>();

        int proxy_port_start = opts["proxy-start"].as<int>();
        int proxy_port_end = opts["proxy-end"].as<int>();

        asio::io_context io_context;
        babel::Instance instance;

        INFO("Server starting...");

        for (int port = proxy_port_start; port < proxy_port_end; port++) {
            asio::ip::udp::endpoint proxy_endpoint(asio::ip::udp::v4(), port);

            INFO("Proxy (audio):", proxy_endpoint);
            instance.join(std::make_shared<babel::Proxy>(io_context, proxy_endpoint));
        }

        asio::ip::tcp::endpoint server_endpoint(asio::ip::tcp::v4(), server_port);
        babel::Server server(instance, io_context, server_endpoint,
                server_tls_key_path, server_tls_cert_path);

        INFO("Server started on:", server_endpoint);

        asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){ io_context.stop(); });

        io_context.run();

        INFO("Server shutting down...");
    } catch (std::exception &ex) {
        ERROR(ex.what());
        return (84);
    }
    return (0);
}
