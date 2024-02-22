/**
 *
 *  TransacoesValidator.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class TransacoesValidator : public HttpFilter<TransacoesValidator>
{
  public:
    TransacoesValidator() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

