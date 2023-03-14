#ifndef friendDto_hpp
#define friendDto_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(level, v_int32,
		VALUE(BEST, 0, "best friends"),
		VALUE(NORMAL, 1, "normal persons")
)
class friendDto : public oatpp:DTO {
    
	DTO_INIT(friendDto, DTO);

	DTO_FIELD(Int32, id);
	DTO_FIELD(String, friendName);
	DTO_FIELD(Boolean, sex);
	DTO_FIELD(Enum<level>::AsString, level);
};

#include OATPP_CODEGEN_END(DTO)

#endif
