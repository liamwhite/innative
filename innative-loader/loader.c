// Copyright (c)2019 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in innative.h

#include "innative/export.h"
#include <stdio.h>

#ifdef IR_PLATFORM_WIN32
#include "../innative/win32.h"

struct WinPass
{
  IRExports* exports;
  Environment* env;
  int* err;
};

BOOL CALLBACK CountResource(__in_opt HMODULE hModule, __in LPCSTR lpType, __in LPSTR lpName, __in LONG_PTR lParam)
{
  *((unsigned int*)lParam) += 1;
  return TRUE;
}

BOOL CALLBACK EnumHandler(__in_opt HMODULE hModule, __in LPCSTR lpType, __in LPSTR lpName, __in LONG_PTR lParam, void(*handler)(struct WinPass*, uint8_t*, DWORD, const char*))
{
  struct WinPass* pass = (struct WinPass*)lParam;
  HRSRC res = FindResourceA(hModule, lpName, lpType);
  if(res != NULL)
  {
    HGLOBAL buf = LoadResource(hModule, res);
    if(buf)
    {
      uint8_t* data = LockResource(buf);
      if(data)
        (*handler)(pass, data, SizeofResource(hModule, res), lpName);
      return TRUE;
    }
  }
  *pass->err = ERR_FATAL_NULL_POINTER;
  return FALSE;
}

void EnumEnvironmentHandler(struct WinPass* pass, uint8_t* data, DWORD sz, const char* name)
{
  (*pass->exports->AddEmbedding)(pass->env, data[0], data + 1, sz - 1);
}

void EnumModuleHandler(struct WinPass* pass, uint8_t* data, DWORD sz, const char* name)
{
  (*pass->exports->AddModule)(pass->env, data, sz, name, pass->err);
}
void EnumWhitelistHandler(struct WinPass* pass, uint8_t* data, DWORD sz, const char* name)
{
  *pass->err = 0;
  (*pass->exports->AddWhitelist)(pass->env, name, data);
}

BOOL CALLBACK EnumEnvironment(__in_opt HMODULE hModule, __in LPCSTR lpType, __in LPSTR lpName, __in LONG_PTR lParam)
{
  return EnumHandler(hModule, lpType, lpName, lParam, &EnumEnvironmentHandler);
}
BOOL CALLBACK EnumModule(__in_opt HMODULE hModule, __in LPCSTR lpType, __in LPSTR lpName, __in LONG_PTR lParam)
{
  return EnumHandler(hModule, lpType, lpName, lParam, &EnumModuleHandler);
}
BOOL CALLBACK EnumWhitelist(__in_opt HMODULE hModule, __in LPCSTR lpType, __in LPSTR lpName, __in LONG_PTR lParam)
{
  return EnumHandler(hModule, lpType, lpName, lParam, &EnumWhitelistHandler);
}

#elif defined(IR_PLATFORM_POSIX)
#error TODO
#else
#error unknown platform!
#endif

// This is a generic cross-platform WASM runtime loader. It uses the runtime stub
// that was compiled with it and loads whatever payloads it was configured with.
int main(int argc, char** argv)
{
  // First we load the runtime using whatever runtime function was linked.
  // This will either be an embedded runtime, or a stub function that dynamically loads a DLL.
  IRExports exports;
  innative_runtime(&exports);
  unsigned int maxthreads = 0;
  void* assembly = (*exports.LoadAssembly)("out.cache");

  // Before doing anything, check if we have a cached version available
  if(!assembly)
  {
    // Count WASM module payloads.
#ifdef IR_PLATFORM_WIN32
    unsigned int modules = 0;
    if(EnumResourceNamesA(NULL, WIN32_RESOURCE_MODULE, &CountResource, (LONG_PTR)&modules) == FALSE)
    {
      fprintf(stderr, "Error counting resources: %u\n", GetLastError());
      return GetLastError();
    }

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    maxthreads = sysinfo.dwNumberOfProcessors;
#elif defined(IR_PLATFORM_POSIX)
    maxthreads = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    // Then create the runtime environment with the module count.
    Environment* env = (*exports.CreateEnvironment)(modules, maxthreads, argv[0]);
    env->flags = ENV_NO_INIT; //ENV_MULTITHREADED;
    if(!env)
    {
      fprintf(stderr, "Unknown error creating environment.\n");
      return -1;
    }

    // Then add each module payload to the environment, checking for any fatal errors.
#ifdef IR_PLATFORM_WIN32
    int err = ERR_SUCCESS;
    struct WinPass pass = { &exports, env, &err };

    if(EnumResourceNamesA(NULL, WIN32_RESOURCE_MODULE, &EnumModule, (LONG_PTR)&pass) == FALSE)
    {
      if(GetLastError() != ERROR_RESOURCE_TYPE_NOT_FOUND)
      {
        fprintf(stderr, "Error enumerating modules: %u\n", GetLastError());
        return err;
      }
    }
#elif defined(IR_PLATFORM_POSIX)
#error TODO
#endif

    if(err < 0)
    {
      fprintf(stderr, "Error loading modules: %i\n", err);
      return err;
    }

    // Ensure all modules are loaded, in case we have multithreading enabled
    (*exports.WaitForLoad)(env);

    // Then add each embedding environment payload to the environment.
    // These payloads have a tag, but have no set format. What the tag means depends on the runtime we've loaded.
#ifdef IR_PLATFORM_WIN32
    if(EnumResourceNamesA(NULL, WIN32_RESOURCE_EMBEDDING, &EnumEnvironment, (LONG_PTR)&pass) == FALSE || err < 0)
    {
      if(GetLastError() != ERROR_RESOURCE_TYPE_NOT_FOUND)
      {
        fprintf(stderr, "Error enumerating embedding environments: %u - %i\n", GetLastError(), err);
        return err;
      }
    }

#elif defined(IR_PLATFORM_POSIX)
#error TODO
#endif

    // Add the whitelist values, the resource name being the module and the data being the function
#ifdef IR_PLATFORM_WIN32
    if(EnumResourceNamesA(NULL, WIN32_RESOURCE_WHITELIST, &EnumWhitelist, (LONG_PTR)&pass) == FALSE || err < 0)
    {
      if(GetLastError() != ERROR_RESOURCE_TYPE_NOT_FOUND)
      {
        fprintf(stderr, "Error enumerating whitelist: %u - %i\n", GetLastError(), err);
        return err;
      }
    }

#elif defined(IR_PLATFORM_POSIX)
#error TODO
#endif

    // Attempt to compile. If an error happens, output it and any validation errors to stderr
    err = (*exports.Compile)(env, "out.cache");
    if(err < 0)
    {
      fprintf(stderr, "Compile error: %i\n", err);

      for(ValidationError* err = env->errors; err != 0; err = err->next)
        fprintf(stderr, "Error %i: %s\n", err->code, err->error);

      int i = 0;
      scanf_s("%i", &i);
      return err;
    }

    // Destroy environment now that compilation is complete
    (*exports.DestroyEnvironment)(env);
    assembly = (*exports.LoadAssembly)("out.cache");
  }

  if(!assembly)
    return ERR_FATAL_NULL_POINTER;

  // Load the entry point and execute it
  IR_Entrypoint start = (*exports.LoadFunction)(assembly, 0, IR_INIT_FUNCTION);
  IR_Entrypoint exit = (*exports.LoadFunction)(assembly, 0, IR_EXIT_FUNCTION);
  if(!start)
    return ERR_INVALID_START_FUNCTION;

  (*start)();
  if(exit)
    (*exit)();

  return ERR_SUCCESS;
}

#ifdef IR_PLATFORM_WIN32
struct HINSTANCE__;

// WinMain function, simply a catcher that calls the main function
int __stdcall WinMain(struct HINSTANCE__* hInstance, struct HINSTANCE__* hPrevInstance, char* lpCmdLine, int nShowCmd)
{
  main(0, (char**)hInstance);
}
#endif