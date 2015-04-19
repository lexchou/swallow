# SIL Instruction References


Conventions used in this documentation:
- All registers used as instruction argument will follow a type definition
- Instruction parameters and return value are quoted with **'{'** and **'}'**, usually types are denoted after it.
- Instruction parameter types used in this document for instruction parameters:
    * **Register** - a virtual register
    * **Offset** - an offset points to a type's member
    * **RegisterOffset** - a register followed with an offset 
    * **Type** - Type declaration
    * **Label** - Branch label in the code
    * **Symbol** - a global symbol
    * **Immediate** - A hard coded immediate number



Conventions used in SIL source code:
- Register starts with % and initialized as an instruction's return value, register will not be allowed to change after it's defined, example : "%20" 
- Type starts with $, example: `$Builtin.Word`;
- Offset has a prefix '#' and started from 1, example: `%1#1` means the first element located by address register %1.
- Member offset uses `#Type.field` to represent, compiler will calculate the real offset instead of a hard coding immediate number, example : `#Int.Value`.
- Function denoted by offset form usually has a component or index field, property usually has several accessor, so for a getter accessor of a property can be written by `"#Hashable.hashValue!getter.1"`.
- Annotations are represented as `[annotation name]`, example: `[transparent]`;
- Annotations in function type don't use bracket, but uses the same form that used in swift, example: `@thin (@inout Int) -> Int`.
- Branch label ends with a **':'**, there could be a tuple next to it, that means the branch also bounded to the function parameter(used by the first label in a function) or enum's assocated data(used in **switch_enum**).

Like LLVM IR, SIL also provides [unlimited-register based RISC instruction set](https://proofwiki.org/wiki/Definition:Unlimited_Register_Machine), that means it's possible to assign a register to store each instruction's return value.




## Top-level Keywords:
- **sil_stage**
Description:
Declare which stage the sil was generated
Syntax:
```
sil_stage {canonical|raw}
```
Example:
```
sil_stage canonical
```

- **sil_global**
Description:
Define a global variable symbol for exporting
Syntax:
```
sil_global {symbol name} : {symbol type}
```
Example:
```
sil_global @_Tv4test3ONESi : $Int
```

- **sil**
Description:
Define a function symbol
Syntax:
```
sil {private|public_external} {function name} : {function type}
{
  //sil body code
}
```
Example:
```
sil @_TFC4test5Adderd : $@cc(method) @thin (@owned Adder) -> @owned Builtin.NativeObject {
bb0(%0 : $Adder):
  debug_value %0 : $Adder  // let self            // id: %1
  %2 = unchecked_ref_cast %0 : $Adder to $Builtin.NativeObject // user: %3
  return %2 : $Builtin.NativeObject               // id: %3
}
```

- **sil_vtable**
Description:
Define virtual function table for class.
Syntax:
```
sil_vtable {Class name} {
  {function offset : Offset}: {global symbol name : Symbol}
}
```
Example:
```
sil_vtable Adder {
  #Adder.init!initializer.1: _TFC4test5AddercfMS0_FT3valSi_S0_	// test.Adder.init (test.Adder.Type)(val : Swift.Int) -> test.Adder
}
```

- **sil_witness_table**
Description:
Define type's witness table for given protocol
Syntax:
```
sil_witness_table {Type name}: {protocol name} module {module name} {
  base_protocol {base protocol}: {Type name}: {base protocol} module {module name}
    method {Method Offset : Offset}: [symbol name]
}
```
Example:
```
sil_witness_table XXX: Hashable module test {
  base_protocol Equatable: XXX: Equatable module test
  method #Hashable.hashValue!getter.1: @_TTWO4test3XXXSs8HashableFS1_g9hashValueSi	// protocol witness for Swift.Hashable.hashValue.getter : Swift.Int in conformance test.XXX : Swift.Hashable
}
```


## Address calculation
- **address_to_pointer**
Description:
Convert a native type pointer to wrapped pointer type like Builtin.RawPointer
Syntax:
```
{ret} = address_to_pointer {native pointer : Register} : {native pointer type : Type} to {wrapper pointer type : Type}
```
Example:
```
%5 = address_to_pointer %4 : $*Int to $Builtin.RawPointer // user: %7
```

- **pointer_to_address**
Description:
Convert a wrapped pointer value to native type
Syntax:
```
{ret} = pointer_to_address {wrapped pointer : Register} : {wrapped pointer type : Type} to {native pointer type : Type}
```
Example:
```
%8 = pointer_to_address %0 : $Builtin.RawPointer to $*Optional<A> // user: %9
```

- **sil_global_addr**
Description:
Load a global symbol's address
Syntax:
```
{ret} = sil_global_addr {global symbol : Symbol}: {symbol type : Type}
```
Example:
```
%0 = sil_global_addr @_Tv4test3ONESi : $*Int    // user: %4
```

- **ref_element_addr**
Description:
Get object's field address, the object is a class type.
Syntax:
```
{ret} = ref_element_addr {object address : Register} : {object type : Type}, {element offset : Offset}
```
Example:
```
%4 = ref_element_addr %1 : $Adder, #Adder.i     // user: %5
```

- **index_addr**
Description:
Get array element's address by given index, index is specified by another register
Syntax:
```
{ret} = index_addr {array address : RegisterOffset} : {array type : Type}, {index reference : Register} : {index type : Type}
```
Example:
```
%44 = index_addr %41#1 : $*String, %43 : $Builtin.Word // user: %45
```

- **tuple_element_addr**
Description:
Get tuple element's address by element index, index is an immediate number.
Syntax:
```
{ret} = tuple_element_addr {tuple address : Register} : {tuple pointer type}, {index : Immediate}
```
Example:
```
%69 = tuple_element_addr %68 : $*(Int, Int, Float), 0 // user: %73
```



## Memory Access
- **store**
Description:
Store value to target addressed by a pointer.
Syntax:
```
{ret} = store {value : Register} to {target memory : Register} : {target pointer type : Type}
```
Example:
```
store %72 to %69 : $*Int                        // id: %73
```
Equivalent C Code:
```
*((int*)%69) = %72;
```

- **assign**
Description:
Store value to target addressed by a pointer.
TODO: Find the differences between *assign* and *store*
Syntax:
```
{ret} = assign {value : Register} to {target memory : Register} : {target pointer type : Type}
```
Example:
```
assign %8 to %9 : $*Int                         // id: %10
```
Equivalent C Code:
```
*((int*)%9) = %8;
```

- **load**
Description:
Load value from given memory address
Syntax:
```
{ret} = load {source memory : Register} : {source pointer type : Type}
```
Example:
```
%3 = load %2 : $*Int                            // user: %5
```
Equivalent C Code:
```
 %3 = *((int*)%2);
```

- **alloc_ref**
Description:
Allocate a reference type on heap
Syntax:
```
{ret} = alloc_ref {type : Type}
```
Example:
```
%2 = alloc_ref $Adder                           // user: %4
```
Equivalent C++ Code:
```
$Adder* %2 = new $Adder();
```

- **dealloc_ref**
Description:
Destroy a reference type instance
Syntax:
```
{ret} = dealloc_ref {instance : Register} : {instance type : Type}
```
Example:
```
dealloc_ref %4 : $Adder                         // id: %5
```
Equivalent C++ Code:
```
delete %4;
```

- **alloc_box**
Description:
Allocate a pointer to the reference type on stack, this will not allocate the instance but only a pointer to it.
Syntax:
```
{ret} = alloc_box {reference type : Type}
```
Example:
```
%1 = alloc_box $Adder  // var c                 // users: %6, %22, %57
```
Equivalent C Code:
```
$Adder* %1;
```

- **alloc_stack**
Description:
Allocate a value type on stack and return its address
Syntax:
```
{ret} = alloc_stack {type : Type}
```
Example:
```
%8 = alloc_stack $String                        // users: %54, %55, %56
```
Equivalent C Code:
```
$String %8;
```

- **dealloc_stack**
Description:
Deallocate a value from the stack.
Syntax:
```
{ret} = dealloc_stack {value address : RegisterOffset} : {pointer type : Type}
```
Example:
```
dealloc_stack %166#0 : $*@local_storage Int     // id: %172
```

- **alloc_array**
Description:
Allocate a typed array with given size specified by a register.
Syntax:
```
{ret} = alloc_array {type : Type}, {size : Register} : {size : Type}
```
Example:
```
%41 = alloc_array $String, %40 : $Builtin.Word  // users: %42, %44, %47, %49, %52
```
Equivalent C Code:
```
$String $41[$40];
```

- **struct_extract**
Description:
Load struct's field value
Syntax:
```
{ret} = struct_extract {struct address : Register} : {struct type : Type}, {field offset : Offset}
```
Example:
```
%3 = struct_extract %0 : $Float, #Float.value   // user: %5
```
Equivalent C Code:
```
%3 = $0.value;
```

- **tuple_extract**
Description:
Get tuple element's value by element index, index is an immediate number.
Syntax:
```
{ret} = tuple_extract {tuple register : Register} : {tuple type : Type}, {element index : Immediate}
```
Example:
```
%7 = tuple_extract %6 : $(Builtin.Word, Builtin.Int1), 0 // user: %10
```

- **copy_addr**
Description:
Copy data in source address to target address

Syntax:
```
{ret} = copy_addr {src : Register} to {dst : Register} : {dst pointer type : Type}
{ret} = copy_addr [take] {src : Register} to [initialization] {dst : Register} : {dst pointer type : Type}
```
Example:
```
copy_addr %9 to [initialization] %71 : $*Float  // id: %76
```
Equivalent C Code:
```
*%71 = *%9;
```

- **inject_enum_addr**
Description:
Update enum's case value
Syntax:
```
{ret} = inject_enum_addr {pointer to enum : Register} : {enum's pointer type : Type}, {case offset : Offset}
```
Example:
```
inject_enum_addr %0 : $*Optional<T>, #Optional.Some!enumelt.1 // id: %5
```

- **destroy_addr**
Description:
Mark the address register destroyed, will not be able to use in the following context.
Syntax:
```
{ret} = destroy_addr {pointer : Register} : {pointer type : Type}
```
Example:
```
destroy_addr %1 : $*T                           // id: %8
```


## ARC
- **strong_release**
Description:
Decrease an instance's reference counter, when it reaches zero, destroy the instance.
Syntax:
```
{ret} = strong_release {instance pointer : Register} : {instance type : Type} 
```
Example:
```
strong_release %1 : $Adder                      // id: %9
```

- **strong_retain**
Description:
Increase an instance's reference counter.
Syntax:
```
{ret} = strong_retain {instance pointer : Register} : {instance type : Type}
```
Example:
```
strong_retain %22 : $Adder                      // id: %23
```

- **retain_value**
Description:
Increase the reference counter of the instance stored in an optional variable.
Syntax:
```
{ret} = retain_value {opt : Register} : {optional type : Type}
```
Example:
```
retain_value %0 : $Optional<A>                  // id: %4
```

- **release_value**
Description:
Decrease the reference counter of the instance stored in an optional variable.
Example:
```
{ret} = release_value {opt : Register} : {optional type : Type}
```
Example:
```
release_value %0 : $Optional<A>                 // id: %10
```

- **load_weak**
Description:
Load a weak variable from an address register.
Syntax:
```
{ret} = load_weak {var_address : Register} : {target address type : Type}
```
Example:
```
%3 = load_weak %2 : $*@sil_weak Optional<A>     // user: %5
```

- **store_weak**
Description:
Store a weak reference(referenced by optional type) to an address.
Syntax:
```
{ret} = store_weak {weak reference : Register} to {target address : Register} : {target address type : Type}
```
Example:
```
store_weak %0 to %4 : $*@sil_weak Optional<A>   // id: %5
```



## Function access
- **function_ref**
Description:
Return a function's address
Syntax:
```
{ret} = function_ref {function symbol : Symbol} : {function type : Type}
```
Example:
```
%25 = function_ref @_TFSiCfMSiFT22_builtinIntegerLiteralBi2048__Si : $@thin (Builtin.Int2048, @thin Int.Type) -> Int // user: %28
```

- **builtin_function_ref**
Description:
Look up for a built-in function's address, the built-in function was a ["LLVM IR" intrinsic function](http://llvm.org/docs/LangRef.html#intrinsic-functions) to perform low-level operations such as primitive arithmetics.
Syntax:
```
{ret} = builtin_function_ref {builtin function : String} : {function type : Type}
```
Example:
```
%19 = builtin_function_ref "cmp_slt_Word" : $@thin (Builtin.Word, Builtin.Word) -> Builtin.Int1 // user: %21
```

- **witness_method**
Description:
Look up for a class's witness method by it's witness method offset.
Syntax:
```
{ret} = witness_method {type : Type}, {witness method offset : Offset} : {witness method type : Type}
```
Example:
```
%165 = witness_method $Int, #Equatable."=="!1 : $@cc(witness_method) @thin <τ_0_0 where τ_0_0 : Equatable> (@in τ_0_0, @in τ_0_0, @thick τ_0_0.Type) -> Bool // user: %170
```


- **class_method**
Description:
Look up for a class's method from virtual table by its offset
Syntax:
```
 {ret} = class_method {instance : Register} : {instance type : Type}, {method offset : Offset} : {method type : Type}
```
Example:
```
%24 = class_method %22 : $Adder, #Adder.calc!1 : Adder -> (Int) -> Int , $@cc(method) @thin (Int, @owned Adder) -> Int // user: %29
```


- **apply**
Description:
Call a function by its address
Syntax:
```
{ret} = apply {function address: Register} {arguments : Tuple} : {function type : Type}
```
Example:
```
%183 = apply %178(%180, %181, %182, %179) : $@thin (Builtin.RawPointer, Builtin.Word, Builtin.Int1, @thin String.Type) -> @owned String // user: %184
```
Equivalent C Code:
```
%183 = %178(%180, %181, %182, %179);
```



## Value Construction
- **integer_literal**
Description:
Construct an integer literal
Syntax:
```
{ret} = integer_literal {integer type : Type}, {value : Immediate}
```
Example:
```
%182 = integer_literal $Builtin.Int32, -1        // user: %183
```
Equivalent C Code:
```
int32_t %182 = -1;
```


- **float_literal**
Description:
Construct a float literal
Syntax:
```
{ret} = float_literal {float type : Type}, {hexadecimal value : Immediate} 
```
Example:
```
%31 = float_literal $Builtin.FPIEEE80, 0x3FFD999999999999999A // 0.300000000000000000011 // user: %33
```
Equivalent C Code:
```
double %31 = 0.300000000000000000011;
```

- **string_literal**
Description:
Construct a string literal from a specified encoding
Syntax:
```
%180 = string_literal {encoding : Encoding} {data : String}
```
Example:
```
%180 = string_literal utf8 "fallthrough"        // user: %183
```
Equivalent C Code:
```
const char* %180 = "fallthrough";
```

- **metatype**
Description:
Get the meta type of given type
Syntax:
```
{ret} = metatype {type : Type}
```
Example:
```
%179 = metatype $@thin String.Type              // user: %183
```
Equivalent C# Code:
```
Type %179 = typeof(String);
```


- **tuple**
Description:
Construct a tuple
Syntax:
```
{ret} = tuple {data : Tuple}
```
Example:
```
%4 = tuple (%0 : $XXX, %1 : $XXX)               // users: %5, %6
```

- **struct**
Description:
Construct a struct instance with initial data
Syntax:
```
{ret} = struct {struct type : Type} {initial data : Tuple}
```
Example:
```
%10 = struct $MyStruct (%7 : $Builtin.Word)          // user: %11
```
Equivalent C Code:
```
MyStruct %10 = {%7};
```



- **init_enum_data_addr**
Description:
Initialize enum's associated data and return associated data address for writing.
*NOTE*: This will not change the enum variable's case, so it's usually used together with *inject_enum_addr*.
Syntax:
```
{ret} = init_enum_data_addr {enum's address : Register} : {enum's pointer type : Type}, {Enum case's offset : Offset}
```
Example:
```
%3 = init_enum_data_addr %0 : $*Optional<T>, #Optional.Some!enumelt.1 // user: %4
```

- **unchecked_take_enum_data_addr**
Description:
Return enum's associated data address for reading
Syntax:
```
{ret} = unchecked_take_enum_data_addr {enum's address : Register} : {enum's pointer type : Type}, {enum case's offset : Offset}
```
Example:
```
%6 = unchecked_take_enum_data_addr %3#1 : $*Optional<T>, #Optional.Some!enumelt.1 // user: %7
```

- **enum**
Description:
Construct a raw-value enum
Syntax:
```
{ret} = enum {enum type : Type}, {Case's offset : Offset}
```
Example:
```
%1 = enum $XXX, #XXX.B!enumelt                  // user: %2
```





## Control flow
- **return**
Description:
Abort current control flow and return to caller with a return value
Syntax:
```
return {return value : Register} : {return value's type : Type}
```
Example:
```
return %6 : $()                                 // id: %7
```
Equivalent C Code:
```
return %6
```

- **br**
Description:
Abort current control flow and jump to specified label
Syntax:
```
br {label : Label}
```
Example:
```
br bb1                                          // id: %16
```

- **cond_br**
Description:
Jump to *then label* if condition is non-zero otherwise jump to *else label*.
Syntax:
```
{ret} = cond_br {condition : Register}, {then : Label}, {else : Label}
```
Example:
```
cond_br %23, bb2, bb6                           // id: %24
```

- **cond_fail**
Description:
Raise an exception if condition is non-zero.
Syntax:
```
{ret} = cond_fail {condition : Register} : {condition type : Type}
```
Example:
```
cond_fail %63 : $Builtin.Int1                   // id: %64
```
Equivalent C Code:
```
  assert(!%63);
```

- **switch_enum**
Description:
Compare an enum register, and jump to branch if the register equals to the corresponding case, if no branch matched, jump to the default branch.  
Syntax:
```
{ret} = switch_enum {condition : Register} : {condition type : Type}, case {case offset : Offset}: {branch : Label}, case {case offset : Offset}: {branch : Label} 
{ret} = switch_enum {condition : Register} : {condition type : Type}, case {case offset : Offset}: {branch : Label}, default {default branch : Label} 
```
Example:
```
switch_enum %80 : $XXX, case #XXX.A!enumelt: bb7, case #XXX.B!enumelt: bb9 // id: %81
```

- **switch_enum_addr**
Description:
Same as *switch_enum*, but the condition parameter is a pointer to the real value.
Syntax:
```
{ret} = switch_enum_addr {condition : Register} : {condition type : Type}, case {case offset : Offset}: {branch : Label}, case {case offset : Offset}: {branch : Label} 
{ret} = switch_enum_addr {condition : Register} : {condition type : Type}, case {case offset : Offset}: {branch : Label}, default {default branch : Label} 
```
Example:
```
switch_enum_addr %3#1 : $*Optional<T>, case #Optional.Some!enumelt.1: bb1, case #Optional.None!enumelt: bb2 // id: %5
```

- **unreachable**
Description:
Mark the compiler the workflow will never reach this line.
Syntax:
```
{ret} = unreachable
```
Example:
```
unreachable                                     // id: %148
```





## Meta
- **debug_value**
Description:
Generate a meta data for debugging this value
Syntax:
```
debug_value {value : Register} : {type : Type}
```
Example:
```
debug_value %129 : $Int  // let f               // id: %149
```

- **mark_uninitialized**
Description:
  Mark the input as uninitialized, and return the input.
Syntax:
```
{ret} = mark_uninitialized [rootself] {input : Register} : {input type : Type} 
```
Example:
```
%4 = mark_uninitialized [rootself] %1 : $Adder  // users: %9, %11, %13
```

- **mark_function_escape**
Description:
TODO: what is this used for?
Syntax:
```
mark_function_escape {val : Register} : {type : Type}
```
Example:
```
mark_function_escape %0 : $*Int                 // id: %6
```




## Type cast
- **unchecked_ref_cast**
Description:
Cast value from source type to target type without checking.
Syntax:
```
{ret} = unchecked_ref_cast {input : Register} : {source type : Type} to {target type : Type}
```
Example:
```
%2 = unchecked_ref_cast %0 : $Adder to $Builtin.NativeObject // user: %3
```
Equivalent C++ Code:
```
%2 = reinterpret_cast<$Builtin.NativeObject>(%0);
```


## Annotations
- **initialization**
Description:
Mark the target register as initialized, used in IR-level optimization.
```
copy_addr %9 to [initialization] %71 : $*Float  // id: %76
```

- **transparent**
Mark the function can be inlined.
```
sil public_external [transparent] @_TFSiCfMSiFT22_builtinIntegerLiteralBi2048__Si : $@thin (Builtin.Int2048, @thin Int.Type) -> Int {
```

- **semantics**
Description:
**NOTE: I think it's some kinds of alias, need more evidences.**
```
sil [readonly] [semantics "string.makeUTF8"] @_TFSSCfMSSFT21_builtinStringLiteralBp8byteSizeBw7isASCIIBi1__SS : $@thin (Builtin.RawPointer, Builtin.Word, Builtin.Int1, @thin String.Type) -> @owned String
```

- **take**
Description:
**NOTE: I think it's used to mark the target will be read, could be used to place some memory fences here, need more evidences.**
Example:
```
copy_addr [take] %6 to [initialization] %2#1 : $*T // id: %7
```

- **readonly**
Description:
**NOTE: Base on my guess, it's used to mark returned value will has readonly semantic, used for further optimization in linking stage.**
```
sil [readonly] @_TFSa20convertFromHeapArrayU__fMGSaQ__FTBp5ownerBo5countBw_GSaQ__ : $@thin <τ_0_0> (Builtin.RawPointer, @owned Builtin.NativeObject, Builtin.Word, @thin Array<τ_0_0>.Type) -> @owned Array<τ_0_0>
```

- **noinline**
Description:
Disable function inlining when calling the function.
Example:
```
sil [noinline] @_TFSs18_fatalErrorMessageFTVSs12StaticStringS_S_Su_T_ : $@thin @noreturn (StaticString, StaticString, StaticString, UInt) -> ()
```

- **cc**
Description:
Define function type's a**cc**ess type, the type could be witness_method/method, the function's access type also decides how to address the actual function address at runtime.
Syntax:
```
@cc(type)
```
Example:
```
%165 = witness_method $Int, #Equatable."=="!1 : $@cc(witness_method) @thin <τ_0_0 where τ_0_0 : Equatable> (@in τ_0_0, @in τ_0_0, @thick τ_0_0.Type) -> Bool // user: %170
```

- **thin**
Description:
**NOTE: the usage of this annotation is not quite clear. I thought it's used to mark it should be as thin as possible, reduce swift-related meta data"
Example:
```
%15 = function_ref @_TFSig9hashValueSi : $@cc(method) @thin (Int) -> Int // user: %16
```

- **thick**
Description:
**NOTE: If my guess on @thin is right, then @thick should means it will carry more meta data on given type.**
Example:
```
sil @_TFC4main1ACfMS0_FT_S0_ : $@thin (@thick A.Type) -> @owned A 
```

- **out**
Description:
Parameter semantic annotation, used to tell IR-level optimization that this parameter is used for *writing*, the corresponding type must be a pointer type. 
Example:
```
@thin <T> (@out Optional<T>, @in T) -> ()
```

- **in**
Description:
Mark the parameter is used for *reading*.
Example:
```
@thin <T> (@out Optional<T>, @in T) -> ()
```

- **inout**
Description:
Mark the parameter is used for both *reading* and *writing*.
Example:
```
$@thin <T> (@inout Optional<T>) -> Builtin.Int1 
```

- **local_storage**
Description:
Mark the type is used by a stack variable.
Example:
```
  dealloc_stack %2#0 : $*@local_storage T         // id: %12
```

- **owned**
Description:
**NOTE: According to my analysis on SIL code, I think it means the value is owned by caller, callee must manually release its reference.** 
Example:
```
sil @_TFSS37convertFromStringInterpolationSegmentfMSSFSiSS : $@thin (Int, @thin String.Type) -> @owned String
```

- **sil_weak**
Exists in weak optional type declaration, to represent it's a weak reference.
```
%3 = load_weak %2 : $*@sil_weak Optional<A> 
```

