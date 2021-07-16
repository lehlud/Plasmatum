target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"

%plsm_val = type { i8, i8* }

declare i8* @malloc(i64)

declare %plsm_val @print(i64, %plsm_val*)

declare %plsm_val @println(i64, %plsm_val*)

define %plsm_val @0(i64 %0, %plsm_val* %1) {
  %3 = call i8* @malloc(i64 8)
  %4 = bitcast i8* %3 to i64*
  store i64 42, i64* %4, align 8
  %5 = alloca %plsm_val, align 8
  %6 = getelementptr inbounds %plsm_val, %plsm_val* %5, i32 0, i32 0
  %7 = getelementptr inbounds %plsm_val, %plsm_val* %5, i32 0, i32 1
  %8 = bitcast i64* %4 to i8*
  store i8 1, i8* %6, align 1
  store i8* %8, i8** %7, align 8
  %9 = load %plsm_val, %plsm_val* %5, align 8
  ret %plsm_val %9
}

define i8 @main() {
  %1 = call %plsm_val @0(i64 0, %plsm_val* null)
  ret i8 42
}
