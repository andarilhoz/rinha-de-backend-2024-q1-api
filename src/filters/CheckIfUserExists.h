/**
 *
 *  CheckIfUserExists.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
#include <drogon/HttpController.h>
#include "models/Clientes.h"


using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::db;

class CheckIfUserExists : public HttpFilter<CheckIfUserExists>
{
  public:
    CheckIfUserExists() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

