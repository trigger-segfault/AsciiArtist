/*=================================================================|
|* File:				Declarations.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

//=================================================================|
// DEFINITIONS													   |
//=================================================================/
#pragma region Definitions

/* Defines a namespace and gives it an alias. */
#define DEFINE_ALIAS(aliasName, namespaceName) namespace namespaceName {} namespace aliasName = namespaceName

DEFINE_ALIAS(Pc,		PowerConsole);

DEFINE_ALIAS(PcCtrls,	PowerConsole::Controls);
DEFINE_ALIAS(PcTlbx,	PowerConsole::Controls::Toolbox);
DEFINE_ALIAS(PcIntr,	PowerConsole::Controls::Toolbox::Internal);

DEFINE_ALIAS(PcDraw,	PowerConsole::Drawing);
DEFINE_ALIAS(PcGeo,		PowerConsole::Geometry);
DEFINE_ALIAS(PcHelp,	PowerConsole::Helpers);
DEFINE_ALIAS(PcInput,	PowerConsole::Input);
DEFINE_ALIAS(PcIO,		PowerConsole::IO);


/* Declares typedefs for smart pointers. */
#define DECLARE_SMART_PTRS(className) \
typedef std::unique_ptr<className> className##UPtr; \
typedef std::shared_ptr<className> className##SPtr; \
typedef std::weak_ptr<className> className##WPtr; \
typedef std::unique_ptr<const className> className##CUPtr; \
typedef std::shared_ptr<const className> className##CSPtr; \
typedef std::weak_ptr<const className> className##CWPtr

/* Just declares a class. */
#define DECLARE_CLASS(className) \
class className
/* Just declares a struct. */
#define DECLARE_STRUCT(structName) \
struct structName

/* Declares the class and typedefs for smart pointers. */
#define DECLARE_CLASS_AND_SMART_PTRS(className) \
DECLARE_CLASS(className); \
DECLARE_SMART_PTRS(className)

/* Declares the struct and typedefs for smart pointers. */
#define DECLARE_STRUCT_AND_SMART_PTRS(className) \
DECLARE_STRUCT(className); \
DECLARE_SMART_PTRS(className)

/* Declares smart pointer typedefs within a namespace. */
#define DECLARE_NAMESPACE_AND_SMART_PTRS(namespaceName, className) \
namespace namespaceName { DECLARE_SMART_PTRS(className); }

/* Declares the class and its namespace. */
#define DECLARE_NAMESPACE_AND_CLASS(namespaceName, className) \
namespace namespaceName { DECLARE_CLASS(className); }

/* Declares the struct and its namespace. */
#define DECLARE_NAMESPACE_AND_STRUCT(namespaceName, structName) \
namespace namespaceName { DECLARE_STRUCT(structName); }

/* Declares the class, its namespace, and typedefs for smart pointers. */
#define DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(namespaceName, className) \
namespace namespaceName { DECLARE_CLASS_AND_SMART_PTRS(className); }

/* Declares the struct, its namespace, and typedefs for smart pointers. */
#define DECLARE_NAMESPACE_STRUCT_AND_SMART_PTRS(namespaceName, className) \
namespace namespaceName { DECLARE_STRUCT_AND_SMART_PTRS(className); }

#pragma endregion
//=================================================================|
#endif /* End include guard */
//=================================================================|
