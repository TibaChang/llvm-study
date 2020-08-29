; ModuleID = 'input.c'
source_filename = "input.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @foo(i32 %a, i32 %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  %mul = mul nsw i32 %0, 2
  %1 = load i32, i32* %b.addr, align 4
  %add = add nsw i32 %mul, %1
  ret i32 %add
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @bar(i32 %c, i32 %d) #0 {
entry:
  %c.addr = alloca i32, align 4
  %d.addr = alloca i32, align 4
  store i32 %c, i32* %c.addr, align 4
  store i32 %d, i32* %d.addr, align 4
  %0 = load i32, i32* %c.addr, align 4
  %1 = load i32, i32* %d.addr, align 4
  %sub = sub nsw i32 %0, %1
  ret i32 %sub
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %main_ret = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 123, i32* %a, align 4
  store i32 456, i32* %b, align 4
  store i32 1000, i32* %c, align 4
  store i32 999, i32* %d, align 4
  store i32 0, i32* %main_ret, align 4
  %0 = load i32, i32* %a, align 4
  %1 = load i32, i32* %b, align 4
  %call = call i32 @foo(i32 %0, i32 %1)
  %2 = load i32, i32* %main_ret, align 4
  %add = add nsw i32 %2, %call
  store i32 %add, i32* %main_ret, align 4
  %3 = load i32, i32* %c, align 4
  %4 = load i32, i32* %d, align 4
  %call1 = call i32 @bar(i32 %3, i32 %4)
  %5 = load i32, i32* %main_ret, align 4
  %add2 = add nsw i32 %5, %call1
  store i32 %add2, i32* %main_ret, align 4
  %6 = load i32, i32* %main_ret, align 4
  ret i32 %6
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)"}
