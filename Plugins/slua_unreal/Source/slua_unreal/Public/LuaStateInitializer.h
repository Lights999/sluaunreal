#pragma once

#include "LuaState.h"

namespace NS_SLUA
{
    class SLUA_UNREAL_API LuaStateInitializer
    {

    public:
        LuaStateInitializer(const char* name = nullptr);
 /*       virtual ~LuaStateInitializer();*/

        void Init();
        void Release();

    private:
        void LuaStateInitCallback();

        // create global state, freed on app exit
        NS_SLUA::LuaState state;
    };
}
