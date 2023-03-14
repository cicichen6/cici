#include "myController.hpp"
#include "appComponent.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {
	appComponent components;

	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
	router->addController(std::make_shared<myController>());
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);
	oatpp::network::Server server(connectionProvider, connectionHandler);
	OATPP_LOGI("Myapp", "Server running on port %s", connectionProvider->getProperty("port").getData());
	server.run();

}

int main() {
	oatpp::base::Environment::init();
	run();
	std::cout << "\nEnvironment:\n";
	std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
	std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
	oatpp::base::Environment::destroy();

	return 0;
}
