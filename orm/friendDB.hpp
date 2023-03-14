#ifndef FRIENDDB_HPP
#define FRIENDDB_HPP

#include "dto/friendDto.hpp"
#include "oatpp-sqlite/orm.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient)

class friendDb : public oatpp::orm::Dbclient {
public:
	friendDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
		: oatpp::orm::Dbclient(executor)
	{ }

QUERY(createFriend,
      "insert into friendDb"
      "(friendName, sex) VALUES" 
      "(:friend.friendName, :friend.friendSex);",
	PARAM(oatpp::Object<friendDto>, friend))

};

#include OATPP_CODEGEN_END(DbClient)

#endif	
