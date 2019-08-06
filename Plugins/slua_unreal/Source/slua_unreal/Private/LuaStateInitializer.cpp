#include "LuaStateInitializer.h"

namespace NS_SLUA
{
    // read file content
    static uint8* ReadFile(IPlatformFile& PlatformFile, FString path, uint32& len) {
        IFileHandle* FileHandle = PlatformFile.OpenRead(*path);
        if (FileHandle) {
            len = (uint32)FileHandle->Size();
            uint8* buf = new uint8[len];

            FileHandle->Read(buf, len);

            // Close the file again
            delete FileHandle;

            return buf;
        }

        return nullptr;
    }

    static int32 PrintLog(NS_SLUA::lua_State *L)
    {
        FString str;
        size_t len;
        const char* s = luaL_tolstring(L, 1, &len);
        if (s) str += UTF8_TO_TCHAR(s);
        NS_SLUA::Log::Log("PrintLog %s", TCHAR_TO_UTF8(*str));
        return 0;
    }

    LuaStateInitializer::LuaStateInitializer(const char* name) :state(name)
    {
    }

    void LuaStateInitializer::Init()
    {
        state.onInitEvent.AddLambda([this]()
        {
            this->LuaStateInitCallback();
        });

        state.init();

        /*TODO: zhang-yang
        * Lua目录方案1：替换为函数模板,从捕获列表传入Lua根目录 
        * Lua目录方案2：从资源管理器获取Lua的根目录
        */
        state.setLoadFileDelegate([](const char* fn, uint32& len, FString& filepath)->uint8* {

            IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
            //TODO:从资源管理器动态获取正确的目录 zhang-yang
            FString path = FPaths::ProjectContentDir();
            FString filename = UTF8_TO_TCHAR(fn);
            FString fRootName = UTF8_TO_TCHAR("Lua");//Lua的根目录 zhang-yang
            path /= fRootName;
            path /= filename.Replace(TEXT("."), TEXT("/"));

            // 兼容.lua和.luac两种后缀
            TArray<FString> luaExts = { UTF8_TO_TCHAR(".lua"), UTF8_TO_TCHAR(".luac") };
            for (auto& it : luaExts) {
                auto fullPath = path + *it;
                auto buf = ReadFile(PlatformFile, fullPath, len);
                if (buf) {
                    fullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*fullPath);
                    filepath = fullPath;
                    return buf;
                }
            }

            return nullptr;
        });
    }

    void LuaStateInitializer::Release()
    {
        state.close();
    }

    void LuaStateInitializer::LuaStateInitCallback()
    {
        NS_SLUA::lua_State *L = state.getLuaState();
        lua_pushcfunction(L, PrintLog);
        lua_setglobal(L, "PrintLog");
    }
}


