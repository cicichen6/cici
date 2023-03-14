#ifndef appComponent_hpp
#define appComponent_hpp

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"

class appComponent {
	public:
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
	return std::make_shared<oatpp::async::Executor>(
		4,
		1,
		1
	);
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
	return oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 8000});
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
	return oatpp::web::server::HttpRouter::createShared();
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
	return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
	return oatpp::parser::json::mapping::ObjectMapper::createShared();
	}());

};

#endif

