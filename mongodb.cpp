#include <cstdlib>
#include <iostream>
#include <string>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/uri.hpp>

namespace {

class logger final : public mongocxx::logger {
   public:
    explicit logger(std::ostream* stream) : _stream(stream) {}

    void operator()(mongocxx::log_level level,
                    bsoncxx::stdx::string_view domain,
                    bsoncxx::stdx::string_view message) noexcept override {
        if (level >= mongocxx::log_level::k_trace)
            return;
        *_stream << '[' << mongocxx::to_string(level) << '@' << domain << "] " << message << '\n';
    }

   private:
    std::ostream* const _stream;
};

}  // namespace

int main(int argc, char* argv[]) {
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;

    mongocxx::instance inst{bsoncxx::stdx::make_unique<logger>(&std::cout)};

    try {
        const auto uri = mongocxx::uri{(argc >= 2) ? argv[1] : mongocxx::uri::k_default_uri};

        mongocxx::options::client client_options;
        if (uri.tls()) {
            mongocxx::options::tls tls_options;
            client_options.tls_opts(tls_options);
        }

        auto client = mongocxx::client{uri, client_options};

        auto admin = client["admin"];

        auto result = admin.run_command(make_document(kvp("ping", 1)));

        std::cout << bsoncxx::to_json(result) << "\n";

        return EXIT_SUCCESS;

    } catch (const std::exception& xcp) {
        std::cout << "connection failed: " << xcp.what() << "\n";
        return EXIT_FAILURE;
    }
}
