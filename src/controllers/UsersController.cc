/**
 *
 *  UsersController.cc
 *  This file is generated by drogon_ctl
 *
 */

#include "UsersController.h"
#include <string>


void UsersController::getOne(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback,
                             std::string &&id)
{
    auto offset = req->getOptionalParameter<int>("offset").value_or(0);
    auto limit = req->getOptionalParameter<int>("limit").value_or(25);
    auto sortField = req->getOptionalParameter<std::string>("sort_field").value_or("id");
    auto sortOrder = req->getOptionalParameter<std::string>("sort_order").value_or("asc");
    auto sortOrderEnum = sortOrder == "asc" ? SortOrder::ASC : SortOrder::DESC;

    auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));
    auto dbClientPtr = drogon::app().getDbClient("db");
    
    if(!dbClientPtr){
        LOG_ERROR << "DB not initialized";
    }
    Mapper<Transacoes> mapper(dbClientPtr);

    mapper.orderBy(sortField, sortOrderEnum).offset(offset).limit(limit).findAll(
        [callbackPtr](const std::vector<Transacoes> &transacao) {
            Json::Value ret{};
            for (auto t : transacao) {
                ret.append(t.toJson());
            }
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k200OK);
            (*callbackPtr)(resp);
        },
        [callbackPtr](const DrogonDbException &e) {
            LOG_ERROR << e.base().what();
            auto resp = HttpResponse::newHttpJsonResponse("database error");
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            (*callbackPtr)(resp);
        });
}