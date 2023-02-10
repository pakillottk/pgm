#pragma once

#include "../../Commands.h"

namespace PGM::Renderer::API::Backend
{

struct OpenGlCommands : public Commands
{
    void clear(ClearBufferMask mask, Color clearColor = Colors::Black) const override;
};

} // namespace PGM::Renderer::API