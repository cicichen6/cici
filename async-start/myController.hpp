#ifndef myController_hpp
#define myController_hpp

#include "dtos.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class myController : public oatpp::web::server::api::ApiController {
	private:
		typedef myController _ControllerType;

	public:
		myController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
		{}
	public:
		ENDPOINT_ASYNC("GET", "/hello", Root) {
			ENDPOINT_ASYNC_INIT(Root)
			
			Action act() override {
				auto dto = MessageDto::createShared();
		        	dto->statusCode = 200;
				dto->message = "hello world async!";
				return _return(controller->createDtoResponse(Status::CODE_200, dto));	
			}
		};
};

#include OATPP_CODEGEN_BEGIN(ApiController)

#endif
