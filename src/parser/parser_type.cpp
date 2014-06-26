#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include <cstdlib>
#include <stack>
#include <sstream>
#include <iostream>
using namespace Swift;

/*
 “GRAMMAR OF A GENERIC PARAMETER CLAUSE
 
 ‌ generic-parameter-clause → <generic-parameter-listrequirement-clauseopt>
 ‌ generic-parameter-list → generic-parameter | generic-parameter,generic-parameter-list
 ‌ generic-parameter → type-name
 ‌ generic-parameter → type-name:type-identifier
 ‌ generic-parameter → type-name:protocol-composition-type
 ‌ requirement-clause → whererequirement-list
 ‌ requirement-list → requirement | requirement,requirement-list
 ‌ requirement → conformance-requirement | same-type-requirement
 ‌ conformance-requirement → type-identifier:type-identifier
 ‌ conformance-requirement → type-identifier:protocol-composition-type
 ‌ same-type-requirement → type-identifier==type-identifier
 GRAMMAR OF A GENERIC ARGUMENT CLAUSE
 
 ‌ generic-argument-clause → <generic-argument-list>
 ‌ generic-argument-list → generic-argument | generic-argument,generic-argument-list
 ‌ generic-argument → type”
 
 
 “GRAMMAR OF A TYPE
 
 ‌ type → array-type  function-type  type-identifier tuple-type  optional-type  implicitly-unwrapped-optional-type  protocol-composition-type  metatype-type
 GRAMMAR OF A TYPE ANNOTATION
 
 ‌ type-annotation → :attributesopttype
 GRAMMAR OF A TYPE IDENTIFIER
 
 ‌ type-identifier → type-namegeneric-argument-clauseopt type-namegeneric-argument-clauseopt.type-identifier
 ‌ type-name → identifier
 GRAMMAR OF A TUPLE TYPE
 
 ‌ tuple-type → (tuple-type-bodyopt)
 ‌ tuple-type-body → tuple-type-element-list...opt
 ‌ tuple-type-element-list → tuple-type-element  tuple-type-element,tuple-type-element-list
 ‌ tuple-type-element → attributesoptinoutopttype inoutoptelement-nametype-annotation
 ‌ element-name → identifier
 GRAMMAR OF A FUNCTION TYPE
 
 ‌ function-type → type->type
 GRAMMAR OF AN ARRAY TYPE
 
 ‌ array-type → type[]  array-type[]”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF AN OPTIONAL TYPE
 
 ‌ optional-type → type?
 GRAMMAR OF AN IMPLICITLY UNWRAPPED OPTIONAL TYPE
 
 ‌ implicitly-unwrapped-optional-type → type!
 GRAMMAR OF A PROTOCOL COMPOSITION TYPE
 
 ‌ protocol-composition-type → protocol<protocol-identifier-listopt>
 ‌ protocol-identifier-list → protocol-identifier  protocol-identifier,protocol-identifier-list
 ‌ protocol-identifier → type-identifier
 GRAMMAR OF A METATYPE TYPE
 
 ‌ metatype-type → type.Type  type.Protocol
 GRAMMAR OF A TYPE INHERITANCE CLAUSE
 
 ‌ type-inheritance-clause → :type-inheritance-list
 ‌ type-inheritance-list → type-identifier  type-identifier,type-inheritance-list”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
*/