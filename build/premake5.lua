-- premake5.lua
workspace "C++20-Project-Workspace"
   configurations { "simple", "debug", "release" }

project "d2slib"

   kind        "ConsoleApp"
   objdir      "%{cfg.buildcfg}/obj"
   language    "C++"
   cppdialect  "C++20"
   location    "../"
   targetdir   "../bin"
   targetname  "d2slib"

   files {
      "../src/**.cpp"
   }
   includedirs    { "../include" }
   links          { }

   enablewarnings {
      "all",
      "extra",
      "shadow",
      "unused",
      "non-virtual-dtor",
      "overriding-method-mismatch",
      "suggest-override",
      "suggest-destructor-override",
      "overloaded-virtual",
      "inconsistent-missing-override",
      "inconsistent-missing-destructor-override",
      "packed",
      "infinite-recursion",
      "initializer-overrides",
      "over-aligned",
      "overlength-strings"
   }

   filter "configurations:simple"
      defines  { "DEBUG" }
      symbols  "Off"
      disablewarnings { "unused" }

   filter "configurations:debug"
      defines   { "DEBUG" }
      symbols   "On"
      optimize  "Debug"
      targetdir "../bin/dbg"

   filter "configurations:release"
      symbols  "Off"
      defines  { "NDEBUG" }
      optimize "On"
