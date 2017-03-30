/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/


#ifndef IGNITION_COMMON_PLUGIN_REGISTERMACROS_HH_
#define IGNITION_COMMON_PLUGIN_REGISTERMACROS_HH_

#include <typeinfo>
#include <type_traits>
#include "ignition/common/PluginInfo.hh"


#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define IGN_PLUGIN_VISIBLE __attribute__ ((dllexport))
  #else
    #define IGN_PLUGIN_VISIBLE __declspec(dllexport)
  #endif
#else
  #if __GNUC__ >= 4
    #define IGN_PLUGIN_VISIBLE __attribute__ ((visibility ("default")))
  #else
    #define IGN_PLUGIN_VISIBLE
  #endif
#endif


/// \brief Register a shared library with only one plugin
/// 
/// Adds a function that returns a struct with info about the plugin
#define IGN_COMMON_REGISTER_SINGLE_PLUGIN(class_name, base_class) \
  struct IGN_macro_must_be_used_in_global_namespace; \
  static_assert(std::is_same<IGN_macro_must_be_used_in_global_namespace, \
      ::IGN_macro_must_be_used_in_global_namespace>::value, \
      "Macro must be used in global namespace"); \
  \
  static_assert(std::is_same<base_class, ::base_class>::value, \
      #base_class " must be fully qualified like ::ns::BaseClass"); \
  \
  static_assert(std::is_same<class_name, ::class_name>::value, \
      #class_name " must be fully qualified like ::ns::MyClass"); \
  \
  static_assert(std::is_base_of<base_class, class_name>::value, \
      #class_name " must inherit from " #base_class); \
  \
  static_assert(!std::is_same<class_name, base_class>::value, \
      "Class and Base class must be different"); \
  \
  extern "C" IGN_PLUGIN_VISIBLE const \
  ignition::common::plugin::PluginInfo IGNCOMMONSinglePluginInfo() \
  { \
    ignition::common::plugin::PluginInfo plugin; \
    plugin.name = #class_name; \
    plugin.interface = #base_class; \
    plugin.baseClassHash = typeid(base_class).hash_code(); \
    plugin.factory = []() { return static_cast<void*>(new class_name()); }; \
    return plugin; \
  }; \
  \
  extern "C" IGN_PLUGIN_VISIBLE const \
  std::size_t IGNCOMMONSinglePluginInfoSize = \
    sizeof(ignition::common::plugin::PluginInfo); \
  \
  extern "C" IGN_PLUGIN_VISIBLE const \
  int IGNCOMMONPluginAPIVersion = \
    ignition::common::plugin::PLUGIN_API_VERSION;

/// TODO macro supporting multiple plugins per shared library

#endif
