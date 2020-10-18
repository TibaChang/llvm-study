; ModuleID = 'input.c'
source_filename = "input.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @foo() #0 {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %g = alloca i32, align 4
  %f = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32 3, i32* %c, align 4
  store i32 4, i32* %d, align 4
  store i32 100, i32* %e, align 4
  store i32 9, i32* %g, align 4
  store i32 10, i32* %f, align 4
  %0 = load i32, i32* %a, align 4
  %cmp = icmp eq i32 %0, 1
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %b, align 4
  %2 = load i32, i32* %d, align 4
  %add = add nsw i32 %1, %2
  store i32 %add, i32* %b, align 4
  %3 = load i32, i32* %b, align 4
  %mul = mul nsw i32 %3, 2
  store i32 %mul, i32* %b, align 4
  br label %if.end19

if.else:                                          ; preds = %entry
  store i32 6, i32* %e, align 4
  %4 = load i32, i32* %e, align 4
  %cmp1 = icmp sgt i32 %4, 3
  br i1 %cmp1, label %if.then2, label %if.end18

if.then2:                                         ; preds = %if.else
  store i32 5, i32* %a, align 4
  %5 = load i32, i32* %e, align 4
  %6 = load i32, i32* %c, align 4
  %cmp3 = icmp sgt i32 %5, %6
  br i1 %cmp3, label %if.then4, label %if.else6

if.then4:                                         ; preds = %if.then2
  %7 = load i32, i32* %d, align 4
  %add5 = add nsw i32 6, %7
  store i32 %add5, i32* %a, align 4
  br label %if.end17

if.else6:                                         ; preds = %if.then2
  %8 = load i32, i32* %d, align 4
  %add7 = add nsw i32 999, %8
  store i32 %add7, i32* %e, align 4
  %9 = load i32, i32* %c, align 4
  %10 = load i32, i32* %a, align 4
  %cmp8 = icmp sgt i32 %9, %10
  br i1 %cmp8, label %if.then9, label %if.end16

if.then9:                                         ; preds = %if.else6
  %11 = load i32, i32* %b, align 4
  %12 = load i32, i32* %d, align 4
  %add10 = add nsw i32 %11, %12
  store i32 %add10, i32* %a, align 4
  %13 = load i32, i32* %g, align 4
  %14 = load i32, i32* %f, align 4
  %cmp11 = icmp sgt i32 %13, %14
  br i1 %cmp11, label %if.then12, label %if.else14

if.then12:                                        ; preds = %if.then9
  %15 = load i32, i32* %g, align 4
  %add13 = add nsw i32 %15, 2
  store i32 %add13, i32* %a, align 4
  br label %if.end

if.else14:                                        ; preds = %if.then9
  %16 = load i32, i32* %a, align 4
  %add15 = add nsw i32 %16, 5
  store i32 %add15, i32* %a, align 4
  br label %if.end

if.end:                                           ; preds = %if.else14, %if.then12
  br label %if.end16

if.end16:                                         ; preds = %if.end, %if.else6
  br label %if.end17

if.end17:                                         ; preds = %if.end16, %if.then4
  br label %if.end18

if.end18:                                         ; preds = %if.end17, %if.else
  br label %if.end19

if.end19:                                         ; preds = %if.end18, %if.then
  %17 = load i32, i32* %a, align 4
  %18 = load i32, i32* %c, align 4
  %add20 = add nsw i32 %17, %18
  %19 = load i32, i32* %d, align 4
  %add21 = add nsw i32 %add20, %19
  store i32 %add21, i32* %a, align 4
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call i32 @foo()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)"}
